// Hiking maps
// Week 3
// Aleksander Matusiak
//
// Solutions that scored 80/100 points, because it explicitly constructs
// the triangle, instead of using given representation in a smart way.

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <deque>
#include <climits>

#define REP(i, n) for (int i = 0; i < (n); ++i)
#define ST first
#define ND second
#define PB push_back
#define MP make_pair
#define deb(a)

using namespace std;

typedef vector<pair<int, int> > VPII;
typedef vector<int> VI;
typedef long long int LL;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Line_2 L;
typedef K::Segment_2 S;
typedef K::Point_2 P;
typedef K::Triangle_2 T;

// Returns whether given point is inside or on boundary of the triangle.
bool in_triangle(const T& t, const P& p) {
    auto o = t.bounded_side(p);
    return o == CGAL::ON_BOUNDED_SIDE || o == CGAL::ON_BOUNDARY;
}

void do_test() {
    int m, n;
    cin >> m >> n;
    int a, b, c, d, e, f;

    vector<P> paths(m);
    REP(i, m) {
        cin >> a >> b;
        paths[i] = P(a, b);
    }

    // Read in maps.
    vector<T> maps(n);
    REP(i, n) {
        // Read in points which describe the map.
        P points[6];
        REP(j, 6) {
            cin >> a >> b;
            points[j] = P(a, b);
        }

        // Construct lines correspondint to boundaries of the triangle.
        L edges[3];
        REP(j, 3) { edges[j] = L(points[2 * j], points[2 * j + 1]); }

        // Intersect these lines to create triangle vertices.
        P vertices[3];
        REP(j, 3) {
            auto o = CGAL::intersection(edges[j], edges[(j + 1) % 3]);
            vertices[j] = *boost::get<P>(&*o);
        }

        // Create a triangle from the vertices.
        maps[i] = T(vertices[0], vertices[1], vertices[2]);
    }

    int start = 0, end = 0;  // considered interval
    int cnt = 0;             // current number of covered path
    int res = INT_MAX;       // minimal length of interval covering all paths
    VI over(m);              // how many maps are covering given path
    vector<VI> under(
        n);  // for given map keeps vector of paths that this map is covering

    // Perform sliding window search.
    while (start < n) {
        // Move end as long as we manage to cover all the paths or reach the
        // end.
        while (end < n && cnt < m - 1) {
            T map = maps[end];

            // Go over all the paths and check whether this map is covering them.
            bool next = in_triangle(map, paths[0]);
            REP(i, m - 1) {
                bool tmp = in_triangle(map, paths[i + 1]);
                if (next && tmp) {
                    over[i]++;
                    if (over[i] == 1) {
                        cnt++;
                    }
                    under[end].PB(i);
                }
                next = tmp;
            }
            end++;
        }

        // Check if we are covering all the paths.
        if (cnt == m - 1) {
            res = min(res, end - start);
        } else {
            // We have reached the end.
            break;
        }

        // Remove map corresponding to the beggining of the interval.
        for (int path : under[start]) {
            over[path]--;
            if (!over[path]) {
                // No more maps are covering this path.
                cnt--;
            }
        }
        start++;
    }

    cout << res << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    REP(i, t) { do_test(); }
    return 0;
}
