// Hiking maps
// Week 3
// Aleksander Matusiak

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <deque>

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

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Line_2 L;
typedef K::Segment_2 S;
typedef K::Point_2 P;
typedef K::Triangle_2 T;

const int NIL = -1;

struct Map {
    P points[6];
};

// Checks whether point p is inside given triangular map (represented
// by 6 points).
bool in_triangle(const Map& map, const P& p) {
    REP(i, 3) {
        auto point_orientation =
            CGAL::orientation(map.points[2 * i], map.points[2 * i + 1], p);
        auto triangle_orientation =
            CGAL::orientation(map.points[2 * i], map.points[2 * i + 1],
                              map.points[(2 * i + 2) % 6]);
        if (point_orientation != triangle_orientation &&
            point_orientation != CGAL::COLLINEAR) {
            return false;
        }
    }
    return true;
}

void do_test() {
    int m, n;
    cin >> m >> n;

    vector<P> paths(n);
    vector<Map> maps(n);

    REP(i, m) {
        int a, b;
        cin >> a >> b;
        paths[i] = P(a, b);
    }

    // Reading points describing maps and storing them in a raw form.
    REP(i, n) {
        REP(j, 6) {
            int a, b;
            cin >> a >> b;
            maps[i].points[j] = P(a, b);
        }
    }

    int start = 0, end = 0, cnt = 0, res = NIL;
    VI over(m);           // how many maps are currently over given path
    vector<VI> under(n);  // keeping maps covering given point in the path,
                          // so as not to compute them again.

    // Sliding window approach - we move begging and end of the interval so
    // as to assure that each path is covered by some map.
    while (start < n) {
        while (end < n && cnt < m - 1) {
            Map map = maps[end];
            bool is_beg_in = in_triangle(map, paths[0]);
            REP(i, m - 1) {
                // Considering path: paths[i] -> paths[i + 1]
                bool is_end_in = in_triangle(map, paths[i + 1]);
                if (is_beg_in && is_end_in) {
                    over[i]++;
                    if (over[i] == 1) {
                        cnt++;
                    }
                    under[end].PB(i);
                }
                is_beg_in = is_end_in;
            }
            end++;
        }

        // Updating current result.
        if (cnt == m - 1) {
            if (res == NIL || end - start < res) {
                res = end - start;
            }
        } else {
            // We have reached the end of maps and cannot cover all the paths.
            break;
        }

        // Moving the beggining of the slinding window and updating points
        // covered by this map.
        for (int path : under[start]) {
            over[path]--;
            if (!over[path]) {
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
