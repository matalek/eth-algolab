// Light at the Museum
// Week 5
// Aleksander Matusiak

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <deque>
#include <map>
#include <climits>

#define REP(i, n) for (int i = 0; i < (n); ++i)
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
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

typedef map<vector<int>, int> MiniFlicks;  // for different light configurations
                                           // keeps minimal number of flicks to
                                           // achieve it

const int N = 33, M = 33;
int target[N];          // target number of turned on lights in each room
int switches[N][M][2];  // what lights does the switch change (as in the input),
                        // 0 - on, 1 - off
int n, m;

// Considers switches in the interval [start, end] and returns map mapping
// number of lights switched in each room to the minimal number of flicked
// switches.
MiniFlicks possibilities(int start, int end) {
    int len = end - start + 1;
    MiniFlicks res;
    // Iterate over all possible flicks of switches.
    REP(flicks, 1 << len) {
        VI lights(m);  // how many lights are switched on in the given room
        int cnt = 0;   // counts number of flicked switches
        FOR(j, start, end + 1) {  // iterate over switches
            bool flicked =
                (flicks & (1 << (j - start)));  // is given switch flicked
            if (flicked) {
                cnt++;
            }
            REP(k, m) {  // iterate over rooms
                if (flicked) {
                    lights[k] += switches[j][k][1];
                } else {
                    lights[k] += switches[j][k][0];
                }
            }
        }

        // Update the minimal number of flicked switches for current lights
        // configuration.
        auto it = res.find(lights);
        if (it != res.end()) {
            it->ND = min(it->ND, cnt);
        } else {
            res.insert(MP(lights, cnt));
        }
    }

    return res;
}

void do_test() {
    cin >> n >> m;
    REP(i, m) { cin >> target[i]; }

    REP(i, n) {
        REP(j, m) {
            int a, b;
            cin >> a >> b;
            switches[i][j][0] = a;
            switches[i][j][1] = b;
        }
    }

    // Splits the switches into two equal parts and computes minimal number of
    // flicks for possible lights configurations.
    int lim = n / 2;
    MiniFlicks half1 = possibilities(0, lim),
               half2 = possibilities(lim + 1, n - 1);

    int res = INT_MAX;
    for (auto el : half1) {
        // Taking into consideration how many lights are switched in each room
        // by the first half of switches, we need to find corresponding lights
        // configuration in the second half of the switches.
        VI cur_target(m);
        REP(i, m) { cur_target[i] = target[i] - el.ST[i]; }

        auto it = half2.find(cur_target);
        if (it != half2.end()) {
            // Found, update result to total number of flicks - sum of those in
            // the first and the second half.
            res = min(res, el.ND + it->ND);
        }
    }

    if (res == INT_MAX) {
        cout << "impossible\n";
    } else {
        cout << res << "\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
