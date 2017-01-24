// Light the Stage
// Week 10
// Aleksander Matusiak
//
// Naive solution which iterates over all players and finds which lamp hit it.
// There is one optimization: if, by considering triangulation, we know that the
// player has not been hit at all, we don't traverse all the lamps.
// Surprisingly, this solution also scored maximum number of points.

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <deque>
#include <queue>
#include <map>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Edge_iterator Edge_iterator;
typedef K::Point_2 P;

#define REP(i, n) for (int i = 0; i < (n); ++i)
#define ST first
#define ND second
#define PB push_back
#define MP make_pair
#define deb(a)
#define deb2(a)

using namespace std;

typedef pair<int, int> PII;
typedef vector<PII> VPII;
typedef vector<int> VI;
typedef long long int LL;

const int NIL = -1;

void do_test() {
    int m, n, h;
    cin >> m >> n;

    vector<P> pts(m);
    vector<int> radii(m);
    REP(i, m) {
        cin >> pts[i];
        cin >> radii[i];
    }

    cin >> h;

    std::vector<P> lamps(n);
    REP(i, n) { cin >> lamps[i]; }

    // Creating triangulation for lamps.
    Triangulation t;
    t.insert(lamps.begin(), lamps.end());

    VI hit_lamp(m);   // which lamp firstly hit given person
    int max_hit = 0;  // last round in which some people are hit
    int cnt = m;      // keeps number of players who haven't been hit yet.

    // Iterate over players.
    REP(i, m) {
        P p1, p2;
        p1 = pts[i];
        p2 = t.nearest_vertex(p1)->point();
        int d = radii[i];
        LL min_dist_sq = ((LL)h + d) * ((LL)h + d);

        if (min_dist_sq <= CGAL::squared_distance(p1, p2)) {
            // Player has not been killed by any lamp.
            hit_lamp[i] = NIL;
            continue;
        }

        // Iteratate over lamps to find out which lamp hit given player first.
        REP(j, n) {
            p2 = lamps[j];
            if (min_dist_sq > CGAL::squared_distance(p1, p2)) {
                hit_lamp[i] = j;
                max_hit = max(max_hit, j);
                cnt--;
                break;
            }
        }
    }

    REP(i, m) {
        if (((cnt > 0) &&
             (hit_lamp[i] == NIL)) ||  // some players were left at the end
            ((cnt == 0) &&
             (hit_lamp[i] == max_hit))) {  // all players were hit and this
                                           // player was hit in the last round
            cout << i << " ";
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
