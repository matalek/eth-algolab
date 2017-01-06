// Light pattern
// Week 5
// Aleksander Matusiak

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <deque>

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

const int N = 100003;

// Calculates whether the state for the position i has to be changed in order to
// satisfy the pattern requirements.
bool check_position(int i, bool current_state, int k, int x) {
    i = i % k;
    int p = k - i - 1;
    return !((x & (1 << p)) ^ (current_state << p));
}

void do_test() {
    int n, k, x;
    cin >> n >> k >> x;

    int res[N][2];  // res[i][correct] - minimal amount of time to make all
                    // light bulbs in the interval [0, i] light up correctly or
                    // incorrectly (depending on correct value)

    REP(i, n) {
        bool state;
        cin >> state;
        bool is_correct = check_position(i, state, k, x);
        res[i][is_correct] =
            (i == 0 ? 0 : res[i - 1][is_correct]);  // the bulb is already in
                                                    // the state we want it to
                                                    // be
        res[i][!is_correct] = (i == 0 ? 0 : res[i - 1][!is_correct]) +
                              1;  // we have to switch the bulb

        // When we are at the edge of the pattern, we can also switch all the
        // bulbs before.
        if ((i % k) == (k - 1)) {
            res[i][0] = min(res[i][0], res[i][1] + 1);
            res[i][1] = min(res[i][1], res[i][0] + 1);
        }
    }

    cout << res[n - 1][1] << "\n";
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
