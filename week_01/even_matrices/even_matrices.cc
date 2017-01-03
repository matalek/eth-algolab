// Even matrices
// Week 1
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

#define REP(i, n) for (int i = 0; i < (n); ++i)
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ST first
#define ND second
#define PB push_back
#define MP make_pair
#define deb(a) a

using namespace std;

typedef pair<int, int> PII;
typedef vector < PII > VPII;
typedef vector<int> VI;
typedef long long int LL;

const int N = 203;
// sum[i][j] == sum of all elements in the rectangle (0,0) - (i, j)
int sum[N][N];

void do_test() {
    int n, res = 0;
    cin >> n;

    // Reading input matrix and calculating sum array.
    REP(i, n) {
        REP(j, n) {
            int a;
            cin >> a;
            sum[i][j] = (i > 0 ? sum[i-1][j] : 0) + a;
        }
    }

    // For each stripe of rows.
    REP(i1, n) {
        FOR(i2, i1, n) {
            // Keeping track of even and odd sums of rectangles in form
            // (i1 + 1, 0) - (i2, j).
            int odds = 0, evens = 1, cur = 0;
            // For each column.
            REP(j, n) {
                cur += sum[i2][j] - (i1 > 0 ? sum[i1 - 1][j] : 0);
                // We are relying on the fact that sum of each interval for
                // columns is equal to the difference between appropriate
                // prefix sums.
                if (cur % 2) {
                    res += odds;
                    odds++;
                } else {
                    res += evens;
                    evens++;
                }
            }
        }
    }

    cout << res << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) {
        do_test();
    }
    return 0;
}
