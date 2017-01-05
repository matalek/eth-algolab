// Theev
// Week 4, Problem of the Week
// Aleksander Matusiak

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <deque>

#define REP(i, n) for (int i = 0; i < (n); ++i)
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ST first
#define ND second
#define PB push_back
#define MP make_pair
#define deb(a)

using namespace std;

typedef vector<pair<int, int> > VPII;
typedef long long int LL;
typedef pair<LL, LL> PII;
typedef vector<int> VI;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;
typedef K::Ray_2 L;
typedef K::Segment_2 S;
typedef K::Point_2 P;

const int N = 10003;
P points[N];
PII my_points[N];
int n;

inline LL squared_distance(PII& a, PII& b) {
    return (a.ST - b.ST) * (a.ST - b.ST) + (a.ND - b.ND) * (a.ND - b.ND);
}

// Checks whether for given square of radius we can find an appropriate location
// for the new transmitter so that both transmitter with this radius cover all
// cities.
bool check(LL radius_sq) {
    // Creating set of points that are not in reange of the original
    // transmitter.
    vector<P> other;
    FOR(i, 1, n) {
        if (squared_distance(my_points[0], my_points[i]) > radius_sq) {
            other.PB(points[i]);
        }
    }

    Min_circle mc(other.begin(), other.end(), true);
    Traits::Circle c = mc.circle();
    return c.squared_radius() <= ((double)radius_sq);
}

void do_test() {
    cin >> n;
    REP(i, n) {
        int a, b;
        cin >> a >> b;
        points[i] = P((double)a, (double)b);
        my_points[i] = MP(a, b);
    }

    // Performing binary search in order to find the smallest radius.
    LL bstart = 0, bend = (LL)2 << 60;
    while (bstart != bend) {
        LL radius_sq = (bstart + bend) / 2;
        if (check(radius_sq)) {
            bend = radius_sq;
        } else {
            bstart = radius_sq + 1;
        }
    }
    cout << bend << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    for (int t = 0; t < T; t++) {
        do_test();
    }
    return 0;
}
