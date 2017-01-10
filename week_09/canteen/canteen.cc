// Canteen
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

void do_test() {
    int n;
    cin >> n;

    // Vertices are corresponding to days and edges between them correspond
    // to storing food overnight. Each flow unit corresponds to one meal. We
    // model amount and cost of meals for each day as edges from source to
    // the vertex, and demand and price set by university by edges from the
    // vertex to the sink.
    int g_size = n + 2, src = g_size - 2, sink = g_size - 1;
    Graph G(g_size);
    EdgeCapacityMap capacitymap = get(edge_capacity, G);
    EdgeWeightMap weightmap = get(edge_weight, G);
    ReverseEdgeMap revedgemap = get(edge_reverse, G);
    ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

    // Producing meals.
    REP(i, n) {
        int a, c;
        cin >> a >> c;
        eaG.addEdge(src, i, a, c);
    }

    // Add edges corresponding to students buying meals. We want to minimilze
    // lostt (possible negative), so the natural approach is to model it as
    // edges with negative weights. However, knowing the boundary for price we
    // can make edges to have possitive weights.
    const int MAX_PRICE = 20;
    int students = 0;
    REP(i, n) {
        int s, p;
        cin >> s >> p;
        students += s;
        eaG.addEdge(i, sink, s, MAX_PRICE - p);
    }

    // Storing overnight.
    REP(i, n - 1) {
        int v, e;
        cin >> v >> e;
        eaG.addEdge(i, i + 1, v, e);
    }

    successive_shortest_path_nonnegative_weights(G, src, sink);
    int cost = find_flow_cost(G);
    int flow = 0;
    // Iterate over all edges leaving the source to sum up the flow values.
    OutEdgeIt e, eend;
    for (tie(e, eend) = out_edges(vertex(src, G), G); e != eend; ++e) {
        flow += capacitymap[*e] - rescapacitymap[*e];
    }

    // Maximum flow corresponds to maximum number of students who bought meals.
    if (flow != students) {
        cout << "im";
    }
    cout << "possible ";

    // We know how many students were satisfied so we can apply offset to
    // correspond to making edges non-negative before.
    int max_profit = -(cost - MAX_PRICE * flow);
    cout << flow << " " << max_profit << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
