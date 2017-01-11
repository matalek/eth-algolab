// Revenge of the Sith (aka Sith)
// Week 10, Problem of the Week
// Aleksander Matusiak

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <deque>
#include <queue>
#include <map>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_data_structure_2<Vb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
typedef Triangulation::Edge_iterator Edge_iterator;
typedef Triangulation::Face_handle Face_handle;
typedef Triangulation::All_faces_iterator Face_iterator;
typedef Triangulation::Vertex_iterator Vertex_iterator;
typedef Triangulation::Vertex_handle Vertex;
typedef K::Point_2 Point;

using namespace boost;
using namespace std;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

typedef adjacency_list<vecS, vecS, undirectedS> Graph;

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

void do_test() {
    int n, r;
    cin >> n >> r;
    vector<pair<Point, int> > points(n);

    REP(i, n) {
        int x, y;
        cin >> points[i].ST;
        points[i].ND = i;
    }

    // Perform binary search in order to find maximal k.
    int start_b = 2, end_b = (n + 1) / 2;
    while (start_b <= end_b) {
        int k = (start_b + end_b) / 2;

        // We can only select from planets that will not be visited by this time
        // be the Empire.
        Triangulation t;
        t.insert(points.begin() + k, points.end());

        Graph G;  // graph corresponding to possible connections between planets
                  // using the scout vessel
        // In order to construct this graph is enough to go over edges from the
        // Delaunay triangulation.
        for (Edge_iterator ei = t.finite_edges_begin();
             ei != t.finite_edges_end(); ++ei) {
            Triangulation::Face& f = *(ei->first);
            int i = ei->second;
            Vertex v = f.vertex(f.cw(i));
            Vertex w = f.vertex(f.ccw(i));

            // If planets are withing range ot the scout vessel, create edge.
            if (CGAL::squared_distance(v->point(), w->point()) <=
                (LL)r * (LL)r) {
                add_edge(v->info(), w->info(), G);
            }
        }

        // Calculate the size of the maximal component.
        std::vector<int> component(num_vertices(G));
        int num = connected_components(G, &component[0]);
        VI cnt(num);

        REP(i, component.size()) { cnt[component[i]]++; }

        int max_component_size = 0;
        REP(i, num) { max_component_size = max(max_component_size, cnt[i]); }

        // We can visit all the planets that belong to the maximal component and
        // nothing more.
        if (max_component_size >= k) {
            // In order to provide progress, we need to add 1 here. We will keep
            // the invariant that for start_b - 1 we can still do it (visit this
            // many planets).
            start_b = k + 1;
        } else {
            end_b = k - 1;
        }
    }

    cout << start_b - 1 << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
