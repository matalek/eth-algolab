// Odd Route
// Week 10
// Aleksander Matusiak

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <deque>
#include <queue>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;
using namespace boost;

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

#define REP(i, n) for (int i = 0; i < (n); ++i)
#define ST first
#define ND second
#define PB push_back
#define MP make_pair
#define deb(a) a

typedef pair<int, int> PII;
typedef vector<PII> VPII;
typedef vector<int> VI;
typedef long long int LL;

int to_number(int i, bool edges, bool weight) {
    return 4 * i + 2 * edges + weight;
}

void do_test() {
    int n, m;
    cin >> n >> m;

    int s, t;
    cin >> s >> t;

    // For each vertex in the orginal graph we will have 4 vertices in the new
    // graph that will hold additional information about oddity of the total
    // weight and number of edges of the path from s vertex.
    int g_size = 4 * n;
    Graph G(g_size);
    WeightMap weightmap = get(edge_weight, G);

    REP(i, m) {
        Edge e;
        bool success;
        int u, v, c;
        cin >> u >> v >> c;

        // Create outgoing edges from 4 coresponding vertices.
        REP(count_oddity, 2) {
            bool odd_weight = c % 2;
            REP(weight_oddity, 2) {
                // We always change oddity of number of edges in the path  and
                // sometimes oddity of the otal weight (depending on the current
                // edge weight)
                tie(e, success) = add_edge(
                    to_number(u, count_oddity, weight_oddity),
                    to_number(v, !count_oddity, weight_oddity ^ odd_weight),
                    G);
                weightmap[e] = c;
            }
        }
    }

    vector<int> distmap(g_size);
    // In starting vertex we have even total weight and even number of edges.
    Vertex start = to_number(s, false, false);
    dijkstra_shortest_paths(G, start,
                            distance_map(make_iterator_property_map(
                                distmap.begin(), get(vertex_index, G))));

    // We are interested in paths that have odd total weight and odd number of
    // edges.
    int dist = distmap[to_number(t, true, true)];
    if (dist == INT_MAX) {
        cout << "no\n";
    } else {
        cout << dist << "\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
