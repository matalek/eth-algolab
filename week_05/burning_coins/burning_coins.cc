// Burning Coins
// Week 5
// Aleksander Matusiak

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
#define deb(a) a

using namespace std;

typedef pair<int, int> PII;
typedef vector<PII> VPII;
typedef vector<int> VI;
typedef long long int LL;

const int N = 2503;
int sum[N];     // prefix sums
int res[N][N];  // res[i][j] - largest amount of money that can be won by
                // starting player for interval [i, j]

// Calculates sum of coins in the interval [i, j].
int cal_sum(int i, int j) {
    return sum[j] - (i == 0 ? 0 : sum[i - 1]);
}

void do_test() {
    int n;
    cin >> n;

    int cur_sum = 0;
    REP(i, n) {
        int v;
        cin >> v;
        res[i][i] = v;
        cur_sum += v;
        sum[i] = cur_sum;
    }

    // Dynamic programming - firstly iterate over the length of considered
    // interval - recursive calculations will depend only on results for
    // intervals with smaller length.
    FOR(len, 2, n + 1) {
        // Later iterate over the beggining of the interval.
        REP(i, n - len + 1) {
            int j = i + len - 1;
            // We can take coin from one of the sides.
            res[i][j] = cal_sum(i, j) - min(res[i + 1][j], res[i][j - 1]);
        }
    }

    cout << res[0][n - 1] << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
