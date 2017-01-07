// Knights
// Week 7, Problem of the Week
// Aleksander Matusiak

#include <iostream>
#include <vector>
#include <algorithm>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;
using namespace boost;

#define REP(i, n) for (int i = 0; i < (n); ++i)
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
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
typedef graph_traits<Graph>::edge_iterator edge_iter;

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

// Possible transitions in the cave.
const int dir_x[] = {-1, 0, 1, 0};
const int dir_y[] = {0, 1, 0, -1};

int point_number(int x, int y, bool in, int n) {
    int res = y * n + x;
    res = 2 * res + !in;
    return res;
}

void do_test() {
    int n, m, k, c;
    cin >> n >> m >> k >> c;
    if (n == 0 || m == 0) {
        cout << 0 << "\n";
        return;
    }

    // Graph corresponds to hallways and intersections inside the cave. We split
    // each intersection into 2 vertices to model that only C knights can run
    // through it.
    int g_size = m * n * 2 + 2, src = g_size - 2, sink = g_size - 1;
    Graph G(g_size);
    EdgeCapacityMap capacitymap = get(edge_capacity, G);
    ReverseEdgeMap revedgemap = get(edge_reverse, G);
    ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, revedgemap);

    const bool IN = true;
    const bool OUT = false;

    REP(i, n) {
        REP(j, m) {
            // Adding boundary on number of knights passing through the
            // intersection.
            eaG.addEdge(point_number(i, j, IN, n), point_number(i, j, OUT, n),
                        c);
            // Adding connections between intersections.
            REP(k, 4) {
                int x = i + dir_x[k];
                int y = j + dir_y[k];
                if (x >= 0 && x < n && y >= 0 && y < m) {
                    eaG.addEdge(point_number(i, j, OUT, n),
                                point_number(x, y, IN, n), 1);
                }
            }
        }
    }

    // Adding exits on the left and right sides.
    FOR(i, 1, n - 1) {
        // Depending whether from edge cells exits are from one or two sides.
        if (m != 1) {
            eaG.addEdge(point_number(i, 0, OUT, n), sink, 1);
            eaG.addEdge(point_number(i, m - 1, OUT, n), sink, 1);
        } else {
            eaG.addEdge(point_number(i, 0, OUT, n), sink, 2);
        }
    }

    // Adding exits on top and bottom.
    FOR(i, 1, m - 1) {
        // Depending whether from edge cells exits are from one or two sides.
        if (n != 1) {
            eaG.addEdge(point_number(0, i, OUT, n), sink, 1);
            eaG.addEdge(point_number(n - 1, i, OUT, n), sink, 1);
        } else {
            eaG.addEdge(point_number(0, i, OUT, n), sink, 2);
        }
    }

    // Adding exits from corners.
    if (n == 1 && m == 1) {
        eaG.addEdge(point_number(0, 0, OUT, n), sink, 4);
    } else if (n == 1) {
        eaG.addEdge(point_number(0, 0, OUT, n), sink, 3);
        eaG.addEdge(point_number(0, m - 1, OUT, n), sink, 3);
    } else if (m == 1) {
        eaG.addEdge(point_number(0, 0, OUT, n), sink, 3);
        eaG.addEdge(point_number(n - 1, 0, OUT, n), sink, 3);
    } else {
        eaG.addEdge(point_number(0, 0, OUT, n), sink, 2);
        eaG.addEdge(point_number(n - 1, 0, OUT, n), sink, 2);
        eaG.addEdge(point_number(0, m - 1, OUT, n), sink, 2);
        eaG.addEdge(point_number(n - 1, m - 1, OUT, n), sink, 2);
    }

    REP(i, k) {
        int x, y;
        cin >> x >> y;
        // Providing unit flows for points where knights stand.
        eaG.addEdge(src, point_number(x, y, IN, n), 1);
    }

    // Flow unit correponds to one knight. Maximum flow will correspond to
    // maximal number of knights who manage to escape.
    long flow = push_relabel_max_flow(G, src, sink);

    cout << flow << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    for (int t = 0; t < T; t++) {
        do_test();
    }
    return 0;
}
