// Building a graph (aka First steps BGL)
// Week 4
// Aleksander Matusiak

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <deque>
#include <climits>
#include <cassert>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

#define REP(i, n) for (int i = 0; i < (n); ++i)
#define ST first
#define ND second
#define PB push_back
#define MP make_pair
#define deb(a) a

using namespace boost;
using namespace std;

typedef adjacency_list<vecS,
                       vecS,
                       undirectedS,
                       no_property,
                       property<edge_weight_t, int> >
    Graph;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_iterator EdgeIt;
typedef graph_traits<Graph>::out_edge_iterator OutEdgeIt;
typedef property_map<Graph, edge_weight_t>::type WeightMap;

typedef vector<pair<int, int> > VPII;
typedef vector<int> VI;

void do_test() {
    int n, m;
    cin >> n >> m;
    Graph G(n);
    WeightMap weightmap = get(edge_weight, G);

    REP(i, m) {
        int a, b, c;
        Vertex v, w;
        cin >> a >> b >> c;
        v = a;
        w = b;
        Edge e;
        bool success;
        tie(e, success) = add_edge(v, w, G);
        weightmap[e] = c;
    }

    // Computing the total weight of its minimum spanning tree.
    vector<Edge> mst;
    kruskal_minimum_spanning_tree(G, back_inserter(mst));
    int totalweight = 0;
    // Go through the minimum spanning tree with an iterator.
    vector<Edge>::iterator mstbeg, mstend = mst.end();
    for (mstbeg = mst.begin(); mstbeg != mstend; ++mstbeg) {
        totalweight += weightmap[*mstbeg];
    }
    cout << totalweight << " ";

    // Computing the distance from node 0 to the node furthest from it.
    vector<int> distmap(n);
    Vertex start = 0;
    dijkstra_shortest_paths(G, start,
                            distance_map(make_iterator_property_map(
                                distmap.begin(), get(vertex_index, G))));
    int maxdist = 0;
    REP(i, n) {
        if (distmap[i] < INT_MAX) {  // if i is reachable
            if (distmap[i] > maxdist) {
                maxdist = distmap[i];
            }
        }
    }
    cout << maxdist << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
