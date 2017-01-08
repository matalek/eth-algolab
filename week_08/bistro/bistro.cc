// Bistro
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
typedef K::Point_2 P;

typedef long long int LL;

#define REP(i, n) for (int i = 0; i < (n); ++i)

using namespace std;

bool do_test() {
    int n;
    cin >> n;
    if (!n) {
        return true;
    }

    std::vector<K::Point_2> points(n);
    REP(i, n) { cin >> points[i]; }

    // Construct triangulation with current restaurants.
    Triangulation t;
    t.insert(points.begin(), points.end());

    int m;
    cin >> m;

    REP(i, m) {
        P p1;
        cin >> p1;
        // Calculating distance to the nearest restaurant.
        P p2 = t.nearest_vertex(p1)->point();
        cout << (LL)CGAL::squared_distance(p1, p2) << "\n";
    }

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
