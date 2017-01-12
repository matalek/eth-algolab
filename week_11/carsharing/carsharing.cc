// Carsharing
// Week 11
// Aleksander Matusiak

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <deque>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

#define REP(i, bookings_cnt) for (int i = 0; i < (bookings_cnt); ++i)
#define stations_cnt first
#define ND second
#define PB push_back
#define MP make_pair
#define deb(a)

using namespace std;
using namespace boost;

typedef pair<int, int> PII;
typedef vector<PII> VPII;
typedef vector<int> VI;
typedef long long int LL;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<
    vecS,
    vecS,
    directedS,
    no_property,
    property<edge_capacity_t,
             long,
             property<edge_residual_capacity_t,
                      long,
                      property<edge_reverse_t,
                               Traits::edge_descriptor,
                               property<edge_weight_t, long> > > > >
    Graph;
typedef property_map<Graph, edge_capacity_t>::type EdgeCapacityMap;
typedef property_map<Graph, edge_weight_t>::type EdgeWeightMap;
typedef property_map<Graph, edge_residual_capacity_t>::type ResidualCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type ReverseEdgeMap;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::out_edge_iterator OutEdgeIt;  // Iterator

// Custom Edge Adder Class, that holds the references to the graph, capacity
// map, weight map and reverse edge map.
class EdgeAdder {
    Graph& G;
    EdgeCapacityMap& capacitymap;
    EdgeWeightMap& weightmap;
    ReverseEdgeMap& revedgemap;

   public:
    EdgeAdder(Graph& G,
              EdgeCapacityMap& capacitymap,
              EdgeWeightMap& weightmap,
              ReverseEdgeMap& revedgemap)
        : G(G),
          capacitymap(capacitymap),
          weightmap(weightmap),
          revedgemap(revedgemap) {}

    void addEdge(int u, int requests, long c, long w) {
        Edge e, reverseE;
        tie(e, tuples::ignore) = add_edge(u, requests, G);
        tie(reverseE, tuples::ignore) = add_edge(requests, u, G);
        capacitymap[e] = c;
        weightmap[e] = w;
        capacitymap[reverseE] = 0;
        weightmap[reverseE] = -w;
        revedgemap[e] = reverseE;
        revedgemap[reverseE] = e;
    }
};

struct request {
    int start_station, end_station, departure_time, arrival_time, profit;
};

// Converts given station number and time_point of given station into vertex
// number, based on prefix sums of number of time points.
int to_number(int station, int time_point, VI& times_mov) {
    return times_mov[station] + time_point;
}

void do_test() {
    int bookings_cnt, stations_cnt;
    cin >> bookings_cnt >> stations_cnt;

    VI starting_cars(stations_cnt);

    int sum_cars = 0;
    REP(i, stations_cnt) {
        cin >> starting_cars[i];
        sum_cars += starting_cars[i];
    }

    vector<VI> times(bookings_cnt);
    vector<request> requests(bookings_cnt);
    int max_time = 0;  // maximal point in time of some arrival
    REP(i, bookings_cnt) {
        request r;
        cin >> r.start_station >> r.end_station >> r.departure_time >>
            r.arrival_time >> r.profit;
        r.start_station--;
        r.end_station--;
        requests[i] = r;
        times[r.start_station].PB(r.departure_time);
        times[r.end_station].PB(r.arrival_time);
        max_time = max(r.arrival_time, max_time);
    }

    VI times_mov(stations_cnt);  // keeps prefix sums of number of relevant
                                 // points in time for each station. Used later
                                 // for calculating vertex offset
    int cnt_times = 0;  // keeping total number of relevant points in time
    REP(i, stations_cnt) {
        // Clean up times vector - possible times of departure and arrivals at
        // given station.
        times[i].PB(0);
        times[i].PB(max_time);
        sort(times[i].begin(), times[i].end());
        times[i].erase(unique(times[i].begin(), times[i].end()),
                       times[i].end());
        times_mov[i] = (i == 0) ? 0 : (times_mov[i - 1] + times[i - 1].size());
        cnt_times += times[i].size();
    }

    // Vertices will correspond to pairs (station, time). Possible times for
    // given station are the times of some car's departure or arrival from/to
    // this station plus 2 edge times (to limit the number of vertices). Flow
    // units will correspond to cars moving between stations in time.
    int g_size = cnt_times + 2, src = g_size - 2, sink = g_size - 1;
    Graph G(g_size);
    EdgeCapacityMap capacitymap = get(edge_capacity, G);
    EdgeWeightMap weightmap = get(edge_weight, G);
    ReverseEdgeMap revedgemap = get(edge_reverse, G);
    ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

    // Introduce constants designating maximal possible values to enable having
    // edges only with positive weights.
    const int MAX_CARS = 1000;
    const int MAX_PROFIT = 100;

    // Add edges corresponding to initial cars at the station and not limit
    // number of cars at the end.
    REP(i, stations_cnt) {
        eaG.addEdge(src, to_number(i, 0, times_mov), starting_cars[i], 0);
        eaG.addEdge(to_number(i, times[i].size() - 1, times_mov), sink,
                    MAX_CARS, 0);
    }

    // Add edges corresponding to cars not being rent. Each such edge will have
    // weight proportional to time difference and maximal possible profit (in
    // order to avoid negative weights).
    REP(j, stations_cnt) {
        REP(i, times[j].size() - 1) {
            eaG.addEdge(to_number(j, i, times_mov),
                        to_number(j, i + 1, times_mov), MAX_CARS,
                        MAX_PROFIT * (times[j][i + 1] - times[j][i]));
        }
    }

    REP(i, bookings_cnt) {
        request r = requests[i];
        // Find points of time corresponding to departure and arrival times.
        int departure =
            lower_bound(times[r.start_station].begin(),
                        times[r.start_station].end(), r.departure_time) -
            times[r.start_station].begin();
        int arrival = lower_bound(times[r.end_station].begin(),
                                  times[r.end_station].end(), r.arrival_time) -
                      times[r.end_station].begin();
        // Add edge corresponding to renting the car and returning it to another
        // station. The weight for this edge will, once again, be dependent on
        // difference in times, but this time we also include profit for renting
        // this car.
        eaG.addEdge(
            to_number(r.start_station, departure, times_mov),
            to_number(r.end_station, arrival, times_mov), 1,
            MAX_PROFIT * (r.arrival_time - r.departure_time) - r.profit);
    }

    // Find minimum cost of maximal flow (where all cars are at the end at some
    // station). This maximum flow will equal sum_cars.
    successive_shortest_path_nonnegative_weights(G, src, sink);
    int cost = find_flow_cost(G);
    // Apply offset to calculate maximum profit.
    int max_cost = MAX_PROFIT * sum_cars * max_time;
    int max_profit = max_cost - cost;

    cout << max_profit << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
