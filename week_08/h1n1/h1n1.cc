// H1N1
// Week 8
// Aleksander Matusiak

#include <algorithm>
#include <cassert>
#include <climits>
#include <deque>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<int, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
typedef Triangulation::Edge_iterator Edge_iterator;
typedef Triangulation::Face_handle Face_handle;
typedef Triangulation::All_faces_iterator Face_iterator;
typedef K::Point_2 P;

typedef long long int LL;

#define REP(i, n) for (int i = 0; i < (n); ++i)

using namespace std;

LL d;
Triangulation t;
int iteration;

// Checks whether we can escape the area from the given face.
bool dfs(Face_handle& f) {
    // Mark that we have visited this face in this iteration.
    f->info() = iteration;
    if (t.is_infinite(f)) {
        // We managed to escape.
        return true;
    }

    // Each face has 3 edges, through which we can go to other faces.
    REP(i, 3) {
        auto s = t.segment(f, i);
        // Check whether the edge is long enough so that we can pass through the
        // middle of it.
        if (s.squared_length() >= d * 4) {
            auto w = f->neighbor(i);
            // If we have not visited the other face, execute DFS there.
            if (w->info() < iteration && dfs(w)) {
                return true;
            }
        }
    }

    return false;
}

bool do_test() {
    int n;
    cin >> n;
    if (!n) {
        return true;
    }

    std::vector<K::Point_2> infected(n);
    REP(i, n) { std::cin >> infected[i]; }

    // Construct triangulation for infected people.
    t.clear();
    t.insert(infected.begin(), infected.end());

    // Info for each face will hold information whether we have traversed it in
    // the current interation. In order not to iterate over all faces after each
    // DFS, for each face we keep in which iteration we have visited it.
    for (Face_iterator f = t.all_faces_begin(); f != t.all_faces_end(); ++f) {
        f->info() = 0;
    }

    int m;
    cin >> m;

    REP(i, m) {
        iteration = i + 1;
        P p1;
        cin >> p1 >> d;
        P p2 = t.nearest_vertex(p1)->point();

        // If the nearest infected person is already closer than sqrt(d), we are
        // already doomed.
        if (CGAL::squared_distance(p1, p2) < d) {
            cout << "n";
            continue;
        }

        // Otherwise we execute DFS in order to find a way out.
        Face_handle mine = t.locate(p1);
        if (dfs(mine)) {
            cout << "y";
        } else {
            cout << "n";
        }
    }
    cout << "\n";

    return false;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    while (true) {
        if (do_test()) {
            break;
        }
    }
    return 0;
}
