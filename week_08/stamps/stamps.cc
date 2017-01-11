// Portfolios
// Week 8, Problem of the Week
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

#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Gmpzf ET;  // exact floating-point type
typedef CGAL::Quadratic_program<double> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Ray_2 L;
typedef K::Segment_2 S;
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

// Check whether the light going from lamp to stamp is interrupted.
bool interrupted(const vector<S>& walls, P& lamp, P& stamp) {
    for (auto w : walls) {
        if (CGAL::do_intersect(S(lamp, stamp), w)) {
            return true;
        }
    }
    return false;
}

void do_test() {
    int l, s, w;
    cin >> l >> s >> w;

    vector<P> lamps(l), stamps(s);
    vector<S> walls(w);
    VI stamps_M(s);  // keeps maximum total illumination intensity for stamps

    REP(i, l) { cin >> lamps[i]; }

    REP(i, s) { cin >> stamps[i] >> stamps_M[i]; }

    REP(i, w) {
        P p1, p2;
        cin >> p1 >> p2;
        walls[i] = S(p1, p2);
    }

    // Variables (from 0 to l-1): power of lamps.
    // Equations - for each stamp lower and upper bound for total illumations
    // intensity.
    Program lp(CGAL::SMALLER, true, 1, true, 1 << 12);

    // Iterate over stamps and create equaties for intensity boundaries.
    REP(i, s) {
        int min_eq = 2 * i, max_eq = 2 * i + 1;
        REP(j, l) {
            if (interrupted(walls, lamps[j], stamps[i])) {
                continue;
            }
            LL r = CGAL::squared_distance(lamps[j], stamps[i]);
            lp.set_a(j, min_eq, 1.0 / r);
            lp.set_a(j, max_eq, 1.0 / r);
        }

        lp.set_b(min_eq, 1);
        lp.set_r(min_eq, CGAL::LARGER);

        lp.set_b(max_eq, stamps_M[i]);
    }

    Solution sol = CGAL::solve_linear_program(lp, ET());

    if (sol.is_optimal()) {
        cout << "yes\n";
    } else {
        cout << "no\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
