// Inball
// Week 7
// Aleksander Matusiak

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>

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
    int d, n;
    cin >> n;
    if (!n) {
        return true;
    }
    cin >> d;

    // Variables: coordinates of the center of the ball (from 0 to d - 1),
    // another one - radius of
    // the ball.
    Program lp(CGAL::SMALLER, false, 0, false, 0);
    const int radius = d;

    // Iterating over constraints.
    REP(i, n) {
        int norm = 0;
        REP(j, d) {
            int a;
            cin >> a;
            //
            lp.set_a(j, i, a);
            norm += a * a;
        }
        norm = (int)sqrt(norm);  // norm is guaranteed to be an integer
        lp.set_a(radius, i, norm);

        int b;
        cin >> b;
        lp.set_b(i, b);
    }

    // CGAL enables us to minimilize the value, so in order to maximilize
    // radius, we will minimalize -radius.
    lp.set_c(radius, -1);

    // Restriction: radius >= 0.
    lp.set_l(radius, true, 0);

    Solution s = CGAL::solve_linear_program(lp, ET());

    if (s.status() == CGAL::QP_INFEASIBLE) {
        cout << "none\n";
    } else if (s.status() == CGAL::QP_UNBOUNDED) {
        cout << "inf\n";
    } else {
        CGAL::Quotient<ET> res = s.objective_value();
        cout << (int)floor_to_double(-res) << "\n";
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
