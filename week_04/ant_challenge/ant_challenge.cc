// Ant challenge
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
#define deb(a)

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
typedef long long int LL;

const int INF = 2000000003;

void do_test() {
    int n, e, s, v_start, v_end;
    cin >> n >> e >> s >> v_start >> v_end;

    vector<Graph> species_graphs(s);
    vector<WeightMap> species_weightmaps(s);
    REP(i, s) {
        species_graphs[i] = Graph(n);
        species_weightmaps[i] = get(edge_weight, species_graphs[i]);
    }

    Graph total_graph(n);  // graph consisting of edges with shortest time
    WeightMap weightmap = get(edge_weight, total_graph);

    REP(i, e) {
        Vertex v, w;
        cin >> v >> w;

        // Adding appropriate edges for each species.
        Edge e;
        bool success;
        REP(j, s) {
            int c;
            cin >> c;

            tie(e, success) = add_edge(v, w, species_graphs[j]);
            species_weightmaps[j][e] = c;
        }

        // Adding appropriate edge in the total graph, with weight initialized
        // to INF, because we don't know to which species network will this
        // edge belong.
        tie(e, success) = add_edge(v, w, total_graph);
        weightmap[e] = INF;
    }

    REP(i, s) {
        int hive;
        cin >> hive;

        // Network for each species is corresponding the minimum spanning tree
        // computed by Prim's algorithm. Since each edge has unique weight, the
        // MSTs computed using Prim's and Kruskal's algorithms are the same, so
        // for the sake of simplicity we will use Kruskal's algorithm.
        vector<Edge> mst;
        kruskal_minimum_spanning_tree(species_graphs[i], back_inserter(mst));
        vector<Edge>::iterator mstbeg, mstend = mst.end();
        for (mstbeg = mst.begin(); mstbeg != mstend; ++mstbeg) {
            Vertex u = source(*mstbeg, species_graphs[i]);
            Vertex v = target(*mstbeg, species_graphs[i]);
            Edge e = edge(u, v, total_graph).first;
            // We know that edge (u, v) belongs to the network of one species,
            // so we can update this edge in the total graph.
            weightmap[e] = min(weightmap[e], weightmap[*mstbeg]);
        }
    }

    // Computing shortest path between 2 given vertices in the total graph.
    vector<LL> distmap(n);
    dijkstra_shortest_paths(
        total_graph, v_start,
        distance_map(make_iterator_property_map(
            distmap.begin(), get(vertex_index, total_graph))));
    cout << distmap[v_end] << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
