// False coin
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

enum STATUS {
    UNDEFINED = 0,
    LESS_EQ = 1,
    GREATER_EQ = 2,
    EQUAL = 3,
};

const int N = 1003;
int coins[N];
STATUS statuses[N];

// Marks all coins that were weighted with an appropriate status.
void mark(int beg, int cnt, STATUS st) {
    for (int i = beg; i < beg + cnt; i++) {
        int coin = coins[i];
        if (statuses[coin] == EQUAL) {
            // The coin has already been marked as that of a proper weight.
            continue;
        }
        // Two different statuses imply that the coin is of a proper weight.
        if (st == LESS_EQ && statuses[coin] == GREATER_EQ) {
            statuses[coin] = EQUAL;
        } else if (st == GREATER_EQ && statuses[coin] == LESS_EQ) {
            statuses[coin] = EQUAL;
        } else {
            statuses[coin] = st;
        }
    }
}

// Marks all coins that did not take part in the weighing, if the result of
// weighing was not '='. Then these coins have to be of a proper weight.
void mark_others(int n, int coins_cnt) {
    // Marking coins that were present in the weighing.
    vector<bool> present(n);
    REP(i, 2 * coins_cnt) { present[coins[i]] = true; }

    REP(i, n) {
        if (!present[i]) {
            statuses[i] = EQUAL;
        }
    }
}

void do_test() {
    int n, k;
    cin >> n >> k;
    REP(i, n) { statuses[i] = UNDEFINED; }

    REP(w, k) {
        // Reading coins for the weighing.
        int coins_cnt;
        cin >> coins_cnt;
        REP(i, 2 * coins_cnt) {
            cin >> coins[i];
            coins[i]--;
        }

        char result;
        cin >> result;
        // Marking appropriate coins based on the weighing result.
        switch (result) {
            case '=':
                mark(0, 2 * coins_cnt, EQUAL);
                break;
            case '<':
                mark(0, coins_cnt, LESS_EQ);
                mark(coins_cnt, coins_cnt, GREATER_EQ);
                mark_others(n, coins_cnt);
                break;
            case '>':
                mark(0, coins_cnt, GREATER_EQ);
                mark(coins_cnt, coins_cnt, LESS_EQ);
                mark_others(n, coins_cnt);
                break;
            default:
                assert(false && "Not possible");
        }
    }

    // Finding coins for which we can't rule out that they are false.
    int cnt = 0;
    int res;
    REP(i, n) {
        if (statuses[i] != EQUAL) {
            res = i + 1;
            cnt++;
        }
    }
    if (cnt == 1) {
        cout << res << "\n";
    } else {
        cout << 0 << "\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
