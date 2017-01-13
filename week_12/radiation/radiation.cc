// Radiation
// Week 12
// Aleksander Matusiak

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <deque>
#include <queue>

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

#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef CGAL::Gmpzf ET;  // exact floating-point type
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct POINT {
    int x, y, z;
};

// Computes value for the given point for one element of the polynomial.
double apply(POINT& p, int j1, int j2, int j3) {
    return pow(p.x, j1) * pow(p.y, j2) * pow(p.z, j3);
}

// Checks whether it's possible to separate cells using polynomial of degree d.
bool check(int d, int h, int t, vector<POINT>& healthy, vector<POINT>& tumor) {
    Program lp(CGAL::SMALLER, false, 0, false, 0);  // true, 2000);
    int eps_var;  // last variable's number, which will correspond to the
                  // minimal distance from the polynomial

    // For each type - healthy and tumor cells.
    REP(type, 2) {
        // For each point create an equation.
        REP(i, type ? h : t) {
            int nr_eq = (type ? t : 0) + i;
            int var_nr = 0;
            // For coefficient of the polynomial of degree d we have one
            // variable. Create the equation which has it's variables and
            // corresponding to cell laying on an appropriate side of the
            // polynomial.
            REP(j1, d + 1) {
                REP(j2, d + 1) {
                    REP(j3, d + 1) {
                        if (j1 + j2 + j3 <= d) {
                            lp.set_a(var_nr, nr_eq,
                                     apply(type ? healthy[i] : tumor[i], j1, j2,
                                           j3));
                            var_nr++;
                        } else {
                            break;
                        }
                    }
                }
            }

            // Add variable, which will correspond to the fact that given point
            // can be placed at some distance of the polynomial. We will want to
            // make sure that it has positive value, so that no point will lie
            // on the polynomial itself.
            eps_var = var_nr;
            lp.set_a(eps_var, nr_eq, type ? 1 : -1);

            lp.set_b(nr_eq, 0);
            lp.set_r(nr_eq, type ? CGAL::SMALLER : CGAL::LARGER);
        }
    }

    // We want to maximilize eps_var (in particular - check whether it can be
    // greatear than 0), so we minimilze it's nagetion.
    lp.set_c(eps_var, -1);

    // Add Bland's pivot rule to make the code faster.
    CGAL::Quadratic_program_options options;
    options.set_pricing_strategy(CGAL::QP_BLAND);

    Solution s = CGAL::solve_linear_program(lp, ET(), options);

    // Checks if there exists a solution in which distance from the polynomial
    // for all points is greater than 0.
    if (s.is_optimal()) {
        auto opt = s.variable_values_begin();
        return (*(opt + eps_var) > 0);
    }

    // If the solution is unbounded the appropriate polynomial exists (distance
    // from polynomial can be greater than 0).
    return s.is_unbounded();
}

void do_test() {
    int h, t;
    cin >> h >> t;
    vector<POINT> healthy(h), tumor(t);

    REP(i, h) { cin >> healthy[i].x >> healthy[i].y >> healthy[i].z; }

    REP(i, t) { cin >> tumor[i].x >> tumor[i].y >> tumor[i].z; }

    const int MAX_D = 30;

    int dbeg = 0, dend = MAX_D;
    bool was_ok = true;

    dend = 1;
    bool was_not_ok;
    while ((was_not_ok = !check(dend, h, t, healthy, tumor)) && dend < MAX_D) {
        dbeg = dend;
        dend *= 2;
    }

    if (was_not_ok && !check(MAX_D, h, t, healthy, tumor)) {
        cout << "Impossible!\n";
        return;
    }

    dend = min(dend, MAX_D);

    // Perform binary search in order to
    while (dbeg != dend) {
        int d = (dbeg + dend) / 2;

        if (check(d, h, t, healthy, tumor)) {
            dend = d;
            was_ok = true;
        } else {
            dbeg = d + 1;
            was_ok = false;
        }
    }

    if (dbeg == MAX_D && !was_ok) {
        cout << "Impossible!\n";
    } else {
        cout << dbeg << "\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
