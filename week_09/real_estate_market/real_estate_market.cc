// Real Estate
// Week 8
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

enum VertexType {
    BUYER = 0,
    PROPERTY = 1,
    STATE = 2,
};

int n, m, s;

// Converts v-th vertex of given type to the global number of vertex.
int number(VertexType type, int v) {
    int offset;
    switch (type) {
        case BUYER:
            offset = 0;
            break;
        case PROPERTY:
            offset = n;
            break;
        case STATE:
            offset = n + m;
            break;
        default:
            assert(false && "Not possible.");
    }
    return offset + v;
}

void do_test() {
    cin >> n >> m >> s;

    // We have vertices corresponding to buyers, properties, as well as states.
    int g_size = n + m + s + 2, src = g_size - 2, sink = g_size - 1;
    Graph G(g_size);
    EdgeCapacityMap capacitymap = get(edge_capacity, G);
    EdgeWeightMap weightmap = get(edge_weight, G);
    ReverseEdgeMap revedgemap = get(edge_reverse, G);
    ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

    // Add edges corresponding to limits for states.
    REP(i, s) {
        int lim;
        cin >> lim;
        eaG.addEdge(number(STATE, i), sink, lim, 0);
    }

    // Add edges corresponding to properties belonging to states.
    REP(i, m) {
        int state;
        cin >> state;
        state--;
        eaG.addEdge(number(PROPERTY, i), number(STATE, state), 1, 0);
    }

    // Add edges corresponding to bids of buyer. Since we want to maximize
    // profit, we need to have edges with lower weights corresponding to bigger
    // profits. Since we know maximum possible bid, we can introduce offset to
    // make all edges weights positive.
    const int MAX_BID = 100;
    REP(i, n) {
        eaG.addEdge(src, number(BUYER, i), 1, 0);
        REP(j, m) {
            int bid;
            cin >> bid;
            eaG.addEdge(number(BUYER, i), number(PROPERTY, j), 1,
                        MAX_BID - bid);
        }
    }

    successive_shortest_path_nonnegative_weights(G, src, sink);
    int cost = find_flow_cost(G);
    int flow = 0;
    // Iterate over all edges leaving the source to sum up the flow values.
    OutEdgeIt e, eend;
    for (tie(e, eend) = out_edges(vertex(src, G), G); e != eend; ++e) {
        flow += capacitymap[*e] - rescapacitymap[*e];
    }

    // Calculate maximum profit based on previously introduced offset.
    int max_profit = -(cost - MAX_BID * flow);
    cout << flow << " " << max_profit << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
