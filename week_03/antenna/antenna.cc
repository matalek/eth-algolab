// Antenna
// Week 3
// Aleksander Matusiak

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <deque>

#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

#define REP(i, n) for (int i = 0; i < (n); ++i)
#define ST first
#define ND second
#define PB push_back
#define MP make_pair
#define deb(a) a

using namespace std;

typedef vector<pair<int, int> > VPII;
typedef vector<int> VI;
typedef long long int LL;

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;
typedef K::Point_2 P;

// Rounds x up to the nearest integer.
double ceil_to_double(const K::FT& x) {
    double a = std::ceil(CGAL::to_double(x));
    while (a < x)
        a += 1;
    while (a - 1 >= x)
        a -= 1;
    return a;
}

void do_test(int n) {
    vector<P> points(n);
    REP(i, n) {
        double x, y;
        cin >> x >> y;
        points[i] = P(x, y);
    }

    // Calculating minimal enclosing circle for the points.
    Min_circle mc(points.begin(), points.end(), true);
    Traits::Circle c = mc.circle();
    cout << (LL)ceil_to_double(CGAL::sqrt(c.squared_radius())) << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    while (true) {
        int n;
        cin >> n;
        if (!n) {
            break;
        }
        do_test(n);
    }
    return 0;
}
