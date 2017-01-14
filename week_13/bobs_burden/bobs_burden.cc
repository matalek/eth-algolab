// Bob's Burden
// Week 13
// Aleksander Matusiak

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <deque>
#include <queue>
#include <climits>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#define REP(i, n) for (int i = 0; i < (n); ++i)
#define ST first
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

typedef adjacency_list<vecS,
                       vecS,
                       directedS,
                       no_property,
                       property<edge_weight_t, int> >
    Graph;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_iterator EdgeIt;
typedef graph_traits<Graph>::out_edge_iterator OutEdgeIt;
typedef property_map<Graph, edge_weight_t>::type WeightMap;

// Converts package position and in/out flag into the vertex number.
int nr(int i, int j, bool in) {
    return 2 * (i * (i + 1) / 2 + j) + in;
}

void do_test() {
    int k;
    cin >> k;
    int n = k * (k + 1) / 2;  // number of packages

    // Verices correspond to packages, but for each package we have 2 vertices -
    // one handling only incomming edges (IN), the other one outgoing (OUT). In
    // this way we are able to assign weights to packages.
    int g_size = 2 * n;
    Graph G(g_size);
    WeightMap weightmap = get(edge_weight, G);
    const int OUT = false, IN = true;

    const VPII moves = {MP(1, 0), MP(0, 1),
                        MP(1, 1)};  // possible moves between packages.
    VI weights(g_size);  // weights for vertices corresponding to packages

    REP(i, k) {
        REP(j, i + 1) {
            // Add edges corresponding to weight of the package.
            int w;
            cin >> w;

            Vertex u = nr(i, j, IN);
            Vertex v = nr(i, j, OUT);

            Edge e;
            bool success;
            tie(e, success) = add_edge(u, v, G);
            weightmap[e] = w;
            weights[u] = w;

            // Add edges corresponding to possible moves between packages.
            REP(l, moves.size()) {
                int x = j + moves[l].ST, y = i + moves[l].ND;
                // Checks if this neighboring package actually exists.
                if (y < k && x <= y) {
                    tie(e, success) = add_edge(v, nr(y, x, IN), G);
                    weightmap[e] = 0;
                    tie(e, success) = add_edge(nr(y, x, OUT), u, G);
                    weightmap[e] = 0;
                }
            }
        }
    }

    VI sum(g_size);  // keeps sum of distances from given vertex to the corners
    const VI corners = {nr(0, 0, IN), nr(k - 1, 0, IN), nr(k - 1, k - 1, IN)};

    // Execute Dijkstra algorithm for each corner in order to compute sum
    // array.
    REP(i, corners.size()) {
        vector<int> distmap(g_size);
        Vertex start = corners[i];
        dijkstra_shortest_paths(G, start,
                                distance_map(make_iterator_property_map(
                                    distmap.begin(), get(vertex_index, G))));
        REP(j, g_size) { sum[j] += distmap[j]; }
    }

    // Find minimum distance to the corners.
    int res = INT_MAX;
    REP(i, k) {
        REP(j, i + 1) {
            // For each vertex corresponding to the package, we have counted its
            // weight 3 times instead of 1.
            int actual_sum = sum[nr(i, j, OUT)] - 2 * weights[nr(i, j, IN)];
            res = min(actual_sum, res);
        }
    }

    cout << res << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
