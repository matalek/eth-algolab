// Satelites
// Week 9
// Aleksander Matusiak

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <queue>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

#define REP(i, n) for (int i = 0; i < (n); ++i)
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ST first
#define ND second
#define PB push_back
#define MP make_pair
#define deb(a) a

using namespace std;

typedef pair<int, int> PII;
typedef vector<PII> VPII;
typedef vector<int> VI;
typedef long long int LL;

using namespace boost;

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
typedef graph_traits<Graph>::out_edge_iterator OutEdgeIt;

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
    int g, s, l;
    cin >> g >> s >> l;

    // Graph will contain connections between ground stations and
    // satellites. We will need to find minimal vertex cover in this bipartite
    // graph.
    int g_size = g + s + 2, src = g_size - 2, sink = g_size - 1;
    Graph G(g_size);
    EdgeCapacityMap capacitymap = get(edge_capacity, G);
    ReverseEdgeMap revedgemap = get(edge_reverse, G);
    ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, revedgemap);

    // Edges for ground stations.
    REP(i, g) { eaG.addEdge(src, i, 1); }

    // Edges for satellites.
    REP(i, s) { eaG.addEdge(g + i, sink, 1); }

    // Edges representing connections between ground stations and satellitles
    REP(i, l) {
        int a, b;
        cin >> a >> b;
        eaG.addEdge(a, g + b, 1);
    }

    // Find a min cut via maxflow.
    int flow = push_relabel_max_flow(G, src, sink);

    // BFS to find vertex set S in order to find verticess belonging to minimum
    // vertex cover.
    vector<int> vis(g_size, false);
    std::queue<int> Q;
    vis[src] = true;
    Q.push(src);
    while (!Q.empty()) {
        const int u = Q.front();
        Q.pop();
        OutEdgeIt ebeg, eend;
        for (tie(ebeg, eend) = out_edges(u, G); ebeg != eend; ++ebeg) {
            const int v = target(*ebeg, G);
            // Only follow edges with spare capacity.
            if (rescapacitymap[*ebeg] == 0 || vis[v]) {
                continue;
            }
            vis[v] = true;
            Q.push(v);
        }
    }

    VI res1, res2;
    // From vertices corresponding to ground stations take these which have not
    // been visited.
    REP(i, g) {
        if (!vis[i]) {
            res1.PB(i);
        }
    }

    // For satellites take these which have been visited.
    FOR(i, g, g + s) {
        if (vis[i]) {
            res2.PB(i - g);
        }
    }

    cout << res1.size() << " " << res2.size() << "\n";
    for (int el : res1) {
        cout << el << " ";
    }
    for (int el : res2) {
        cout << el << " ";
    }
    if (res1.size() || res2.size()) {
        cout << "\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
