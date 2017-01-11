// Casino Royale
// Week 9, Problem of the Week
// Aleksander Matusiak

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <deque>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

#define REP(i, n) for (int i = 0; i < (n); ++i)
#define ST first
#define ND second
#define PB push_back
#define MP make_pair
#define deb(a) a

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
typedef graph_traits<Graph>::out_edge_iterator OutEdgeIt;

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

    void addEdge(int u, int v, long c, long w) {
        Edge e, reverseE;
        tie(e, tuples::ignore) = add_edge(u, v, G);
        tie(reverseE, tuples::ignore) = add_edge(v, u, G);
        capacitymap[e] = c;
        weightmap[e] = w;
        capacitymap[reverseE] = 0;
        weightmap[reverseE] = -w;
        revedgemap[e] = reverseE;
        revedgemap[reverseE] = e;
    }
};

void do_test() {
    int n, m, l;
    cin >> n >> m >> l;

    // Verticess are train stations, edges between them correspond to
    // travelling between stations. Each flow unit corresponds to a place on
    // the train.
    int g_size = n + 1, src = n, sink = n - 1;  // sink - last station.
    Graph G(n + 1);
    EdgeCapacityMap capacitymap = get(edge_capacity, G);
    EdgeWeightMap weightmap = get(edge_weight, G);
    ReverseEdgeMap revedgemap = get(edge_reverse, G);
    ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

    // Entering and leaving train - already limited seats.
    eaG.addEdge(src, 0, l, 0);

    // Seats can be empty between the stations. We use offset in order to have
    // positive weights only.
    const int MAX_Q = 128;
    REP(i, n - 1) { eaG.addEdge(i, i + 1, l, MAX_Q); }

    // Add edges corresponding to an agent traveling during his mission.
    REP(i, m) {
        int a, b, q;
        cin >> a >> b >> q;
        // If we choose this edge, we redirect flow unit from being an empty
        // sit to being occupied by the agent. This allows us to decrease the
        // price of the flow by q (priority of the mission).
        eaG.addEdge(a, b, 1, (b - a) * MAX_Q - q);
    }

    // The maximum flow will be equal to l (seats are not removed from the
    // train).
    successive_shortest_path_nonnegative_weights(G, src, sink);
    int cost = find_flow_cost(G);

    // Calculate result as difference between the situation when no agent
    // travels and our computed minimum cost.
    int res = (n - 1) * l * MAX_Q - cost;
    cout << res << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
