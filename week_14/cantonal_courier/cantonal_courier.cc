// Cantonal Courier
// Week 14, Christmas Challenge
// Aleksander Matusiak

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <deque>
#include <queue>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;
using namespace boost;

#define REP(i, n) for (int i = 0; i < (n); ++i)
#define ST first
#define ND second
#define PB push_back
#define MP make_pair
#define deb(a)

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
                      property<edge_reverse_t, Traits::edge_descriptor> > > >
    Graph;

typedef property_map<Graph, edge_capacity_t>::type EdgeCapacityMap;
typedef property_map<Graph, edge_residual_capacity_t>::type ResidualCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type ReverseEdgeMap;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;

// Custom Edge Adder Class, that holds the references to the graph, capacity map
// and reverse edge map.
class EdgeAdder {
    Graph& G;
    EdgeCapacityMap& capacitymap;
    ReverseEdgeMap& revedgemap;

   public:
    EdgeAdder(Graph& G,
              EdgeCapacityMap& capacitymap,
              ReverseEdgeMap& revedgemap)
        : G(G), capacitymap(capacitymap), revedgemap(revedgemap) {}

    void addEdge(int from, int to, long capacity) {
        Edge e, reverseE;
        bool success;
        tie(e, success) = add_edge(from, to, G);
        tie(reverseE, success) = add_edge(to, from, G);
        capacitymap[e] = capacity;
        capacitymap[reverseE] = 0;
        revedgemap[e] = reverseE;
        revedgemap[reverseE] = e;
    }
};

void do_test() {
    int zones, jobs;
    cin >> zones >> jobs;

    // We have a bipartite graph given as an input - vertices correspond to jobs
    // and zones. Edges between them correspond to necessity to have a ticket
    // for given zone in order to be able to execute this job.
    //
    // When we decide to take some jobs, we also need to buy tickets for
    // appropriate zones. Alternatively, we can consider those tickets that we
    // don't have to buy. Then we need to find such subset of jobs and for that
    // subset of independent zones, where we want to maximize total sum of jobs
    // and zones (and from that we substract the total sum of tickets). So we
    // need to find maximal independent set, where each vertex has some weight.
    //
    // In order to find this maximal independent set with regards to weights,
    // we can consider vertex v with weight w to be in fact v vertices, each
    // with weight w and the same edges as the original vertex. Then we just
    // simply have to take maximum independent set (without weights).
    //
    // For that we use classical algorithm for finding maximum matching when we
    // model it as a flow, and later calculate minimum vertex cover and maximum
    // independent set accordingly. But instead of taking multiple vertices with
    // the same edges, we just increase incoming and outgoing capacities from
    // source/to sink.

    const int g_size = zones + jobs + 2, src = g_size - 2, sink = g_size - 1;
    Graph G(g_size);
    EdgeCapacityMap capacitymap = get(edge_capacity, G);
    ReverseEdgeMap revedgemap = get(edge_reverse, G);
    ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, revedgemap);

    // Add edges from zones to sink.
    REP(i, zones) {
        int cost;
        cin >> cost;
        eaG.addEdge(jobs + i, sink, cost);
    }

    // Add edges from source to jobs.
    int sum_rewards = 0;
    REP(i, jobs) {
        int reward;
        cin >> reward;
        sum_rewards += reward;
        eaG.addEdge(src, i, reward);
    }

    // Add edges corresponding to necessary zone tickets for each jobs.
    const int INF = 5003;  // more than possible ticket price
    REP(i, jobs) {
        int n;
        cin >> n;
        REP(j, n) {
            int zone;
            cin >> zone;
            // We add edge with infinite capacity, cause we don't want to have
            // any constraints on them.
            eaG.addEdge(i, jobs + zone, INF);
        }
    }

    long flow = push_relabel_max_flow(G, src, sink);

    // We have that flow = maximum matching = minimum vertex cover. The number
    // of all vertices (including multiple vertices) is sum_rewards +
    // sum_ticket_prices. Therefore maximum indepent set's size is sum_rewards +
    // sum_ticket_prices - flow. But we have to remember that we still need to
    // substract sum of all tickets (as explained earlier), so in the end we get
    // sum_rewards - flow.
    cout << sum_rewards - flow << endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
