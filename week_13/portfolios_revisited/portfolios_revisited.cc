// Portoflios Revisited
// Week 13
// Aleksander Matusiak

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <deque>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef CGAL::Gmpzf ET;  // exact floating-point type
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

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

double floor_to_double(const CGAL::Quotient<ET> x) {
    double a = std::floor(CGAL::to_double(x));
    while (a > x)
        a -= 1;
    while (a + 1 <= x)
        a += 1;
    return a;
}

bool do_test() {
    int n, m;
    cin >> n >> m;
    if (!n && !m) {
        return true;
    }

    // Variables - amounts of assets.
    // Equations - 1 for cost and 1 for return.
    // We will be minimizing variance and checking if it's possible to be below
    // given value.
    Program qp(CGAL::SMALLER, true, 0, false, 0);
    const int COST_EQ = 0;
    const int RETURN_EQ = 1;

    // For each asset - add appropriate coefficients to equations.
    REP(i, n) {
        int c, r;
        cin >> c >> r;
        qp.set_a(i, COST_EQ, c);
        qp.set_a(i, RETURN_EQ, r);
    }

    // Reading covariances.
    REP(i, n) {
        REP(j, n) {
            int v;
            cin >> v;
            if (j <= i) {
                qp.set_d(i, j, 2 * v);  // passing doubled coefficient
            }
        }
    }

    REP(i, m) {
        int cost, variance;
        cin >> cost >> variance;

        // Cost of the portfolio must be smaller than given value.
        qp.set_b(COST_EQ, cost);

        // We want to make sure that return is at least some value.
        qp.set_r(RETURN_EQ, CGAL::LARGER);

        // Perform binary search for maximal expected portfolio return.
        LL bbeg = 0, bend = ((LL)1) << 36;
        while (bbeg != bend) {
            LL mid = (bbeg + bend) / 2;

            // Add restriction for return value.
            qp.set_b(RETURN_EQ, (double)mid);

            Solution s = CGAL::solve_quadratic_program(qp, ET());

            // Check if minimized variance is not greater than that acceptable.
            bool check = false;
            if (s.status() == CGAL::QP_OPTIMAL) {
                CGAL::Quotient<ET> res = s.objective_value();
                if (to_double(res) <= variance) {
                    check = true;
                }
            }

            if (check) {
                // In order to provide progress we must increase bbeg. We will
                // keep the invariant that bbeg - 1 return can be achieved.
                bbeg = mid + 1;
            } else {
                bend = mid;
            }
        }

        // bbeg + 1 is the smallest return that cannot be achieved, so bbeg - 2
        // is the maximal that can be achieved.
        cout << bbeg - 1 << "\n";
    }

    return false;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    bool finish;
    do {
        finish = do_test();
    } while (!finish);
    return 0;
}
