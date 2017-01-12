// Clues
// Week 11
// Aleksander Matusiak

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <deque>
#include <queue>
#include <climits>
#include <random>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_data_structure_2<Vb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
typedef Triangulation::Edge_iterator Edge_iterator;
typedef Triangulation::Vertex_iterator Vertex_iterator;
typedef Triangulation::Vertex_handle Vertex;
typedef Triangulation::Vertex_circulator Vertex_circulator;
typedef K::Point_2 Point;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

using namespace boost;
using namespace std;

typedef adjacency_list<vecS, vecS, undirectedS> Graph;
typedef graph_traits<Graph>::out_edge_iterator OutEdgeIt;

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

// Traverses graph and returns whether it's possible to perform 2-coloring of
// this graph.
bool dfs(Graph& G, VI& colors, int v, int color) {
    colors[v] = color;

    OutEdgeIt ebeg, eend;
    for (tie(ebeg, eend) = out_edges(v, G); ebeg != eend; ++ebeg) {
        int w = target(*ebeg, G);
        if (colors[w] == 0) {
            // Not yet visited - visit it.
            if (!dfs(G, colors, w, -color)) {
                return false;
            }
        } else {
            // Already visited - check if color is OK.
            if (colors[w] != -color) {
                return false;
            }
        }
    }
    return true;
}

void do_test() {
    int n, m, r;
    cin >> n >> m >> r;
    vector<pair<Point, int> > stations(n);

    REP(i, n) {
        cin >> stations[i].ST;
        stations[i].ND =
            i;  // as additional information we keep index of the station
    }

    bool network_possible = true;  // if it's possible to create a network of
                                   // radio stations which fulfills the
                                   // frequency restriction

    // Vertices correspond to stations and edges between them correspond to two
    // stations being in range of each other.
    Graph G(n);

    Triangulation t;
    t.insert(stations.begin(), stations.end());

    // Go over vertices in triangulation.
    for (auto v = t.vertices_begin(); v != t.vertices_end(); v++) {
        Point p = v->point();
        int i = v->info();

        // Traverse neighboring vertices.
        Vertex_circulator vc = t.incident_vertices(v), done(vc);
        if (vc != 0) {
            do {
                if (!t.is_infinite(vc)) {
                    if (CGAL::squared_distance(p, vc->point()) <=
                        (LL)r * (LL)r) {
                        // These 2 vertices are withing range, so add edge
                        // between them.
                        add_edge(i, vc->info(), G);

                        // The vertices that are within range of v can also be
                        // neighbors of neigboring vertices (but not further
                        // neighbors). Therefore, go over the neighbors of this
                        // neighbor.
                        Vertex_circulator vc2 = t.incident_vertices(vc),
                                          done2(vc2);
                        if (vc2 != 0) {
                            do {
                                if (!t.is_infinite(vc2)) {
                                    // We have encountered a triangle of
                                    // vertices which are all in the range of
                                    // each other, so it's not possible to
                                    // create a network.
                                    if ((vc2->point() != p) &&
                                        (CGAL::squared_distance(vc->point(),
                                                                vc2->point()) <=
                                         (LL)r * (LL)r) &&
                                        (CGAL::squared_distance(p,
                                                                vc2->point()) <=
                                         (LL)r * (LL)r)) {
                                        network_possible = false;
                                        break;
                                    }
                                }
                            } while (++vc2 != done2);
                        }

                        if (!network_possible) {
                            break;
                        }
                    }
                }
            } while (++vc != done);

            if (!network_possible) {
                break;
            }
        }
    }

    // Check if it's possible to perform 2-coloring of the graph.
    VI colors(n);
    REP(i, n) {
        if (colors[i] == 0) {
            network_possible = network_possible && dfs(G, colors, 0, 1);
        }
        if (!network_possible) {
            break;
        }
    }

    // Compute components of the graph.
    std::vector<int> component(num_vertices(G));
    if (network_possible) {
        connected_components(G, &component[0]);
    }

    // Go over the clues.
    REP(i, m) {
        Point a, b;
        cin >> a >> b;
        if (!network_possible) {
            cout << "n";
            continue;
        }

        Vertex va = t.nearest_vertex(a), vb = t.nearest_vertex(b);
        Point pa = va->point(), pb = vb->point();
        int ia = va->info(), ib = vb->info();

        // Checks if nearest stations are within the range.
        bool next_to_station =
            (CGAL::squared_distance(a, pa) <= (LL)r * (LL)r) &&
            (CGAL::squared_distance(b, pb) <= (LL)r * (LL)r);

        if ((CGAL::squared_distance(a, b) <=
             (LL)r * (LL)r) ||  // no stations necessary
            ((component[ia] == component[ib]) &&
             next_to_station)) {  // neighboring stations have to be in the same
                                  // component
            cout << "y";
        } else {
            cout << "n";
        }
    }
    cout << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
