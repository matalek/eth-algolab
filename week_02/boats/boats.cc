// Boats
// Week 2
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
typedef vector<PII> VPII;
typedef vector<int> VI;
typedef long long int LL;

const int INF = 10000000;

void do_test() {
    int n;
    cin >> n;
    VPII v(n);  // (ring position, length)
    REP(i, n) { cin >> v[i].ND >> v[i].ST; }

    // Sorting based on the ring position.
    sort(v.begin(), v.end());

    int cnt = 1;
    int last_end = -INF, cur_end = INF;

    REP(i, n) {
        int p = v[i].ST;
        int l = v[i].ND;

        if (cur_end <= p) {
            // Greedy approach - we can take our current boat, because last one
            // has its end before current ring.
            last_end = cur_end;
            cnt++;
            // We want to place our boat as far left as possible.
            int my_end = max(last_end + l, p);
            cur_end = my_end;
        } else {
            // If our boat ends before the last boat, we remove the last one
            // and take ours.
            int my_end = max(last_end + l, p);
            cur_end = min(cur_end, my_end);
        }
    }

    cout << cnt << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
