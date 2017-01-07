// Portfolios
// Week 7
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

typedef CGAL::Gmpz ET;  // exact integral type
typedef CGAL::Quadratic_program<int> Program;
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

    // Variables: amounts of assets.
    // 0. equation - cost.
    // 1. equation - return.
    Program qp(CGAL::SMALLER, true, 0, false, 0);
    const int COST_EQ = 0;
    const int RETURN_EQ = 1;

    REP(i, n) {
        int c, r;
        cin >> c >> r;
        qp.set_a(i, COST_EQ, c);
        qp.set_a(i, RETURN_EQ, r);
    }

    // Minimizing variance.
    REP(i, n) {
        REP(j, n) {
            int v;
            cin >> v;
            if (j <= i) {
                qp.set_d(i, j, 2 * v);
            }
        }
    }

    REP(i, m) {
        int C, R, V;
        cin >> C >> R >> V;

        // Setting boundaries for current test case.
        qp.set_b(COST_EQ, C);
        qp.set_b(RETURN_EQ, R);
        qp.set_r(RETURN_EQ, CGAL::LARGER);

        Solution s = CGAL::solve_quadratic_program(qp, ET());
        if (s.status() == CGAL::QP_OPTIMAL) {
            CGAL::Quotient<ET> res = s.objective_value();
            if (to_double(res) <= V) {
                cout << "Yes.\n";
            } else {
                cout << "No.\n";
            }
        } else {
            cout << "No.\n";
        }
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
