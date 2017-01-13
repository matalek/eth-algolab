// Corbusier
// Week 12
// Aleksander Matusiak

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <deque>
#include <queue>

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

void do_test() {
    int n, remainder, modulo;
    cin >> n >> remainder >> modulo;

    VI possible(modulo);  // keeps in how many ways it's possible to achieve
                          // each remainder by taking some subset of disks
    possible[0] = 1;

    // Dynamic programming - each disk can be taken or not into the subset. We
    // then update array possible which accumulates sub-results.
    REP(i, n) {
        int a;
        cin >> a;

        VI new_possible = possible;  // temporary array to hold new values
                                     // achieved by using a.
        REP(j, modulo) {
            if (possible[j]) {
                // We take disk with height a to our subset.
                new_possible[(j + a) % modulo]++;
            }
        }
        possible = new_possible;
    }

    // Since we need to take some disks, we need to distinguish between
    // remainder 0, which can be achieved by not taking any disks, and other
    // remainders, which can only be achieved by taking some disks.
    if ((remainder == 0 && possible[remainder] > 1) ||
        (remainder != 0 && possible[remainder])) {
        cout << "yes\n";
    } else {
        cout << "no\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
