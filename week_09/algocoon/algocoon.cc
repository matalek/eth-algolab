// Algocoon (aka Algocoön Group)
// Week 8
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
    int n, m;
    cin >> n >> m;

    Graph G(n);  // graph representing sculptures and limbs
    EdgeCapacityMap capacitymap = get(edge_capacity, G);
    ReverseEdgeMap revedgemap = get(edge_reverse, G);
    ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, revedgemap);

    REP(i, m) {
        int a, b, c;
        cin >> a >> b >> c;
        eaG.addEdge(a, b, c);
    }

    // We need to find minimal minimum cut (iterating over possible sources and
    // sinks). We can limit the number of necessary flow algorithms to execute
    // by concetrating on cases where source or sink is one set vertex. All
    // other minimum cuts will be included in this considered subset.

    int min_cut = INT_MAX, src, sink;

    // Assuming sculpture 0 belongs to my set.
    FOR(i, 1, n) {
        int flow = push_relabel_max_flow(G, 0, i);
        if (flow < min_cut) {
            min_cut = flow;
            src = 0;
            sink = i;
        }
    }

    // Assuming sculpture 0 does not belong to my set.
    FOR(i, 1, n) {
        int flow = push_relabel_max_flow(G, i, 0);
        if (flow < min_cut) {
            min_cut = flow;
            src = i;
            sink = 0;
        }
    }

    // Repeating max flow algorithm for optimal source and sink so that we can
    // later check which sculptures belong to which set.
    push_relabel_max_flow(G, src, sink);

    // Execute BFS to find sculptures that I will take.
    VI vis(n, false);  // visited flags
    std::queue<int> Q;
    vis[src] = true;
    Q.push(src);
    while (!Q.empty()) {
        const int u = Q.front();
        Q.pop();
        OutEdgeIt ebeg, eend;
        for (tie(ebeg, eend) = out_edges(u, G); ebeg != eend; ++ebeg) {
            const int v = target(*ebeg, G);
            // Only follow edges with spare capacity. Other edges correspond to
            // limbs that will be cut.
            if (rescapacitymap[*ebeg] == 0 || vis[v]) {
                continue;
            }
            vis[v] = true;
            Q.push(v);
        }
    }

    cout << min_cut << "\n";
    VI res;
    for (int i = 0; i < n; ++i) {
        if (vis[i]) {
            res.PB(i);
        }
    }
    cout << res.size() << " ";
    for (int el : res) {
        cout << el << " ";
    }
    cout << endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
