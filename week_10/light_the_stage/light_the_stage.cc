// Light the Stage
// Week 10
// Aleksander Matusiak
//
// Solution which uses binary search with 2 optimizations: sometimes increasing
// triangulation instead of creating it from scratch and removing players that
// we already know have not been hit lastly.

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

using namespace std;

typedef pair<int, int> PII;
typedef vector<PII> VPII;
typedef vector<int> VI;
typedef long long int LL;

// Returns players who have not been hit by the lamps from the given
// triangulation.
VI get_left(Triangulation& t, VI& players, vector<P>& pts, VI& radii, int h) {
    // Iterate over players.
    VI left_players;
    for (int i : players) {
        P p1, p2;
        p1 = pts[i];
        p2 = t.nearest_vertex(p1)->point();
        int d = radii[i];
        LL min_dist_sq = ((LL)h + d) * ((LL)h + d);

        if (min_dist_sq <= CGAL::squared_distance(p1, p2)) {
            // Player has not been killed by any lamp.
            left_players.PB(i);
        }
    }
    return left_players;
}

// Prints vector of integers with separating spaces.
void print(VI& v) {
    for (int el : v) {
        cout << el << " ";
    }
    cout << "\n";
}

void do_test() {
    int m, n, h;
    cin >> m >> n;

    vector<P> pts(m);
    VI radii(m);
    REP(i, m) {
        cin >> pts[i];
        cin >> radii[i];
    }

    cin >> h;

    std::vector<P> lamps(n);
    REP(i, n) { cin >> lamps[i]; }

    // Triangulation corresponding to currently considered lamps.
    Triangulation t;

    VI res_players(m);  // vectors of resulting players - players left
    REP(i, m) { res_players[i] = i; }

    // Consider the case when there are some players left after all the rounds.
    t.insert(lamps.begin(), lamps.end());
    VI left_players = get_left(t, res_players, pts, radii, h);
    if (left_players.size()) {
        print(left_players);
        return;
    }

    // Perform binary search in order to find which lamp hit last players.
    int bbeg = 0, bend = n;
    int last_considered = n;  // previosly considered last lamp
    while (bbeg != bend) {
        int mid = (bbeg + bend) / 2;

        if (last_considered < mid) {
            // We already have some points added in the triangulation. We now
            // just need to add missing points.
            t.insert(lamps.begin() + last_considered, lamps.begin() + mid);
        } else {
            // We need to build the triangulation from scratch.
            t = Triangulation();
            t.insert(lamps.begin(), lamps.begin() + mid);
        }

        VI left_players = get_left(t, res_players, pts, radii, h);

        // Check if some players were left/
        if (left_players.size()) {
            // We will no longer need to consider players that have already been
            // hit.
            res_players = left_players;
            bbeg = mid + 1;
        } else {
            bend = mid;
        }
        last_considered = mid;
    }

    print(res_players);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
