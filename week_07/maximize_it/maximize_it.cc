// Maximize it!
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

double ceil_to_double(const CGAL::Quotient<ET> x) {
    double a = std::ceil(CGAL::to_double(x));
    while (a >= x + 1)
        a -= 1;
    while (a < x)
        a += 1;
    return a;
}

// Solves first optimization problem.
void do_test_1(int a, int b) {
    Program qp(CGAL::SMALLER, true, 0, false, 0);
    const int x = 0;
    const int y = 1;

    qp.set_a(x, 0, 1);
    qp.set_a(y, 0, 1);
    qp.set_b(0, 4);

    qp.set_a(x, 1, 4);
    qp.set_a(y, 1, 2);
    qp.set_b(1, a * b);

    qp.set_a(x, 2, -1);
    qp.set_a(y, 2, 1);
    qp.set_b(2, 1);

    // We want to maximize, so we need to negate coefficients.
    qp.set_c(y, -b);
    qp.set_d(x, x, 2 * a);  // doubled coefficient

    Solution s = CGAL::solve_quadratic_program(qp, ET());

    if (s.status() == CGAL::QP_INFEASIBLE) {
        cout << "no\n";
    } else if (s.status() == CGAL::QP_UNBOUNDED) {
        cout << "unbounded\n";
    } else {
        CGAL::Quotient<ET> res = s.objective_value();
        cout << (int)floor_to_double(-res) << "\n";
    }
}

// Solves second optimization problem.
void do_test_2(int a, int b) {
    Program qp(CGAL::LARGER, false, 0, false, 0);
    const int x = 0;
    const int y = 1;
    const int z_sq = 2;  // corresponds to z^2

    qp.set_a(x, 0, 1);
    qp.set_a(y, 0, 1);
    qp.set_b(0, -4);

    qp.set_a(x, 1, 4);
    qp.set_a(y, 1, 2);
    qp.set_a(z_sq, 1, 1);
    qp.set_b(1, -(a * b));

    qp.set_a(x, 2, -1);
    qp.set_a(y, 2, 1);
    qp.set_b(2, -1);

    qp.set_a(x, 3, 1);
    qp.set_b(3, 0);
    qp.set_r(3, CGAL::SMALLER);

    qp.set_a(y, 4, 1);
    qp.set_b(4, 0);
    qp.set_r(4, CGAL::SMALLER);

    qp.set_a(z_sq, 5, 1);
    qp.set_b(5, 0);

    qp.set_c(y, b);
    qp.set_d(x, x, 2 * a);    // doubled coefficient
    qp.set_d(z_sq, z_sq, 2);  // doubled coefficient

    Solution s = CGAL::solve_quadratic_program(qp, ET());

    if (s.status() == CGAL::QP_INFEASIBLE) {
        cout << "no\n";
    } else if (s.status() == CGAL::QP_UNBOUNDED) {
        cout << "unbounded\n";
    } else {
        CGAL::Quotient<ET> res = s.objective_value();
        cout << (int)ceil_to_double(res) << "\n";
    }
}

bool do_test() {
    int p, a, b;
    cin >> p;
    if (!p) {
        return true;
    }
    cin >> a >> b;
    if (p == 1) {
        do_test_1(a, b);
    } else {
        do_test_2(a, b);
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
