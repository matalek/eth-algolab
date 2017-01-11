// Graypes
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

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Edge_iterator Edge_iterator;
typedef Triangulation::Vertex_iterator Vertex_iterator;
typedef Triangulation::Vertex_handle Vertex;
typedef K::Point_2 P;

typedef long long int LL;

#define REP(i, n) for (int i = 0; i < (n); ++i)

using namespace std;

const double NIL = -1;

bool do_test() {
    int n;
    cin >> n;
    if (!n) {
        return true;
    }

    Triangulation t;
    double res = NIL;

    // We want to find the closest distance between graypes. In order to do
    // that, we will choose the smallest value from the closest distance from
    // each point to the graypes that were already considered.
    REP(i, n) {
        P p1;
        cin >> p1;
        if (i > 0) {
            P p2;
            p2 = t.nearest_vertex(p1)->point();
            double temp = CGAL::sqrt(CGAL::squared_distance(p1, p2)) * 100 / 2;
            if (res == NIL || temp < res) {
                res = temp;
            }
        }

        // Add this graype to triangulation, so that it can later for shortest distance.
        t.insert(p1);
    }

    cout << ceil(res) << "\n";
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
