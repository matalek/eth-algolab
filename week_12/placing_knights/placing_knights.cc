// Placing Knights
// Week 12
// Aleksander Matusiak

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <deque>
#include <queue>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

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
typedef vector<VI> VVI;
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

// Converts row and column number into the vertex number.
int to_nr(int i, int j, int n) {
    return i * n + j;
}

// Returns color (0 or 1) of the given board field.
inline bool color(int i, int j) {
    return (i % 2) == (j % 2);
}

void do_test() {
    int n;
    cin >> n;

    VVI board(n, VI(n));
    REP(i, n) {
        REP(j, n) { cin >> board[i][j]; }
    }

    // Graph directly corresponds to the board - vertices are board squares and
    // edges correspond to mossible moves of the knight.
    // This graph will be bipartite. If we consider normal coloring of the chess
    // board, then knights can only move between squares with different colors.
    // We will use this in order to compute maximal matching.
    const int size_g = n * n + 2, src = size_g - 2, sink = size_g - 1;
    Graph G(size_g);
    EdgeCapacityMap capacitymap = get(edge_capacity, G);
    ReverseEdgeMap revedgemap = get(edge_reverse, G);
    ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, revedgemap);

    // Arrays representing possible moves of night in x and y directions.
    const int move_x[] = {-1, -1, 1, 1, -2, -2, 2, 2};
    const int move_y[] = {-2, 2, -2, 2, -1, 1, -1, 1};

    int cnt = 0;  // counts number of present fields
    REP(i, n) {
        REP(j, n) {
            // Depending on the color, construct appropriate edges - vertices
            // with one color will be connected to the source, with other one -
            // to the sink. Directions of all edges will be from source to sink
            // (in order to compute maximum matching using maximum flow
            // algorithms).
            if (color(i, j)) {
                // Add edge from source if given field is available.
                if (board[i][j]) {
                    eaG.addEdge(src, to_nr(i, j, n), 1);
                    cnt++;
                }

                // Create edges corresponding to knights moves.
                REP(k, 8) {
                    int x = i + move_x[k], y = j + move_y[k];
                    if (x >= 0 && x < n && y >= 0 && y < n) {
                        eaG.addEdge(to_nr(i, j, n), to_nr(x, y, n), 1);
                    }
                }
            } else {
                // Add edge to sink if given field is available.
                if (board[i][j]) {
                    eaG.addEdge(to_nr(i, j, n), sink, 1);
                    cnt++;
                }
            }
        }
    }

    // Compute the maximum matching.
    int flow = push_relabel_max_flow(G, src, sink);

    // We are interested in the maximal independent set of vertices. This
    // problem is complementary to already computed maximum matching.
    cout << cnt - flow << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
