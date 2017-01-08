// Germs
// Week 7
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

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Edge_iterator Edge_iterator;
typedef Triangulation::Vertex_iterator Vertex_iterator;
typedef Triangulation::Vertex_handle Vertex;
typedef Triangulation::Vertex_circulator Vertex_circulator;
typedef K::Point_2 P;

typedef long long int LL;

#define REP(i, n) for (int i = 0; i < (n); ++i)

using namespace std;

// Converts given distance to the time it takes to reach this distance.
LL dist_to_time(double dist) {
    dist = sqrt(dist / 4);
    if (dist < 0.5)
        return 0;
    return ceil(sqrt(dist - 0.5));
}

bool do_test() {
    int n;
    cin >> n;
    if (!n) {
        return true;
    }

    int l, b, r, to;
    cin >> l >> b >> r >> to;

    vector<P> bacteria(n);
    REP(i, n) { cin >> bacteria[i]; }

    // Triangulation for bacteria positions.
    Triangulation t;
    t.insert(bacteria.begin(), bacteria.end());

    // Iterate over points in triangulation (bacteria).
    int i = 0;
    vector<LL> times(n);
    for (auto v = t.vertices_begin(); v != t.vertices_end(); v++) {
        P p = v->point();
        int x = p.x(), y = p.y();

        // Calculate time that it takes to collide with the wall.
        times[i] = min(min(r - x, x - l), min(to - y, y - b));
        times[i] *= times[i] * 4;

        // Travers the neighboring vertices in order to find closest one.
        Vertex_circulator vc = t.incident_vertices(v), done(vc);
        if (vc != 0) {
            do {
                if (!t.is_infinite(vc)) {
                    // Update times so that it will take into the account that
                    // current bacteria can collide with bacteria corresponding
                    // to vc.
                    times[i] = min(times[i],
                                   (LL)CGAL::to_double(
                                       CGAL::squared_distance(p, vc->point())));
                }
            } while (++vc != done);
        }
        i++;
    }

    // Sort times of death in orfer to find appropriate statistics.
    sort(times.begin(), times.end());

    // Print out desired statistics.
    cout << dist_to_time(times[0]) << " " << dist_to_time(times[n / 2]) << " "
         << dist_to_time(times[n - 1]) << "\n";

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
