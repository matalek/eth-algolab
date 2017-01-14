// The Phantom Menace
// Week 13
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

// Converts given astronomical object number and in/out flag into vertex number.
int v_nr(int v, bool in) {
    return 2 * v + in;
}

void do_test() {
    int n, m, s, d;
    cin >> n >> m >> s >> d;

    // Vertices are corresponding to astronomical objects, but each object is
    // split into 2 vertices - one which handles incomming edges (IN), another
    // one with handles outgoing (OUT). We want to find minimum cut in this
    // graph.
    int g_size = 2 * n + 2, src = g_size - 2, sink = g_size - 1;
    Graph G(g_size);
    EdgeCapacityMap capacitymap = get(edge_capacity, G);
    ReverseEdgeMap revedgemap = get(edge_reverse, G);
    ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, revedgemap);

    const bool IN = true, OUT = false;
    const int INF = 1001;  // greater than the number of astronomical objects

    // Construct edges between
    REP(i, n) { eaG.addEdge(v_nr(i, IN), v_nr(i, OUT), 1); }

    REP(i, m) {
        int a, b;
        cin >> a >> b;
        // We want to place battleships only on astronomical objects, so we
        // don't want connections between them to be in the minimal cut.
        // Therefore, we give them maximal capacity.
        eaG.addEdge(v_nr(a, OUT), v_nr(b, IN), INF);
    }

    // Add edges corresponding to multiple starting locations.
    REP(i, s) {
        int v;
        cin >> v;
        eaG.addEdge(src, v_nr(v, IN), INF);
    }

    // Add edges corresponding to multiple destinations.
    REP(i, d) {
        int v;
        cin >> v;
        eaG.addEdge(v_nr(v, OUT), sink, INF);
    }

    // Minimal cut is equal to maximal flow, so calculate it.
    long flow = push_relabel_max_flow(G, src, sink);
    cout << flow << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
