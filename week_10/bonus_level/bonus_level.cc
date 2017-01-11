// Bonus Level
// Week 10
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

const int MAX_COST = 100;
const bool IN = true;
const bool OUT = false;

// For given shelf and input flag, retuns corresponding vertex number.
int to_number(int i, int j, bool in, int n) {
    return 2 * (i * n + j) + in;
}

void do_test() {
    int n;
    cin >> n;

    // Vertices will correspond to shelves, but we split each shelf into in and
    // out vertex. This enables us to set restrictions and coins for each shelf.
    // Other rdges correspond to possible movements between shelves. We have to
    // consider 2 paths: from starting vertex to the princess and from the
    // princess tostarting vertex, but this is equivalent to considering 2 paths
    // from starting vertex to the princess.
    int g_size = 2 * n * n + 1, src = g_size - 1,
        sink = to_number(n - 1, n - 1, OUT, n);
    Graph G(g_size);
    EdgeCapacityMap capacitymap = get(edge_capacity, G);
    EdgeWeightMap weightmap = get(edge_weight, G);
    ReverseEdgeMap revedgemap = get(edge_reverse, G);
    ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

    eaG.addEdge(src, to_number(0, 0, IN, n), 2, 0);

    int c, start_v;
    REP(i, n) {
        REP(j, n) {
            cin >> c;

            // Adding costs and flow restrictions for shelves (vertices).
            int nr_in = to_number(i, j, IN, n),
                nr_out = to_number(i, j, OUT, n);
            int capacity;
            // For staring shelf and shelf with princess - we need to be in this
            // vertices twice (2 flow units available, because of 2 paths).
            if (i == 0 && j == 0) {
                start_v = c;  // save cost for starting vertex for further use
                capacity = 2;
            } else if (i == n - 1 && j == n - 1) {
                capacity = 2;
            } else {
                // For other vertices - only once.
                capacity = 1;
            }
            eaG.addEdge(nr_in, nr_out, capacity, MAX_COST - c);

            // Add edges from vertices on top and on the left.
            if (i > 0) {
                eaG.addEdge(to_number(i - 1, j, OUT, n), nr_in, 1, 0);
            }
            if (j > 0) {
                eaG.addEdge(to_number(i, j - 1, OUT, n), nr_in, 1, 0);
            }
        }
    }

    successive_shortest_path_nonnegative_weights(G, src, sink);
    int cost = find_flow_cost(G);

    // Calculating proper cost taking into the account offset.
    int max_coins = -(cost - MAX_COST * 4 * n);
    max_coins -= MAX_COST +
                 start_v;  // we have counted coins for the starting shelf twice
    max_coins -= MAX_COST +
                 c;  // we have counted coins for the shelf with princess twice
                     // (it was considered last)

    cout << max_coins << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
