// Diet
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
#define deb(a) a

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

    // Food - variables from 0, till m - 1.
    // Equations - from 0 till (2 * n - 1); 2 for each nutrient.
    Program lp(CGAL::SMALLER, true, 0, false, 0);

    REP(i, n) {
        int mini, maxi;
        cin >> mini >> maxi;
        // Setting restrictions for each ingredient.
        lp.set_b(2 * i, mini);
        lp.set_r(2 * i, CGAL::LARGER);
        lp.set_b(2 * i + 1, maxi);
    }

    REP(i, m) {
        int price;
        cin >> price;
        // We want to minimize the total price.
        lp.set_c(i, price);

        REP(j, n) {
            // Setting amounts of each nutrient.
            int eq1 = 2 * j, eq2 = 2 * j + 1;
            int amount;
            cin >> amount;
            lp.set_a(i, eq1, amount);
            lp.set_a(i, eq2, amount);
        }
    }

    Solution s = CGAL::solve_linear_program(lp, ET());

    if (s.status() == CGAL::QP_INFEASIBLE) {
        cout << "No such diet.\n";
    } else {
        CGAL::Quotient<ET> res = s.objective_value();
        cout << (int)floor_to_double(res) << "\n";
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
