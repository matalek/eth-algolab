// Hit
// Week 3
// Aleksander Matusiak

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <deque>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

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

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Ray_2 L;
typedef K::Segment_2 S;
typedef K::Point_2 P;

void do_test(int n) {
    LL a, b, c, d;
    cin >> a >> b >> c >> d;
    L ray(P(a, b), P(c, d));

    bool res = false;
    REP(i, n) {
        cin >> a >> b >> c >> d;
        if (res) {
            continue;
        }
        S seg(P(a, b), P(c, d));
        if (CGAL::do_intersect(ray, seg)) {
            res = true;
        }
    }

    if (res) {
        cout << "yes\n";
    } else {
        cout << "no\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int n;
    while (true) {
        cin >> n;
        if (!n) {
            break;
        }
        do_test(n);
    }
    return 0;
}
