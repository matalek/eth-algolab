// Octopussy
// Week 2, Problem of the Week
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

const int N = 70000;
int arr[N];
int blocking[N];  // counts number of bombs that have to be disactivated first
int n;

// Calculates minimal time of the explosion for bombs dependent on (placed on
// top of) the bomb at position v. cur keeps track of current minimal time.
void minimize(int v, int cur) {
    if (v >= n) {
        return;
    }

    if (cur < arr[v]) {
        arr[v] = cur;
    } else {
        cur = arr[v];
    }
    minimize(2 * v + 1, cur);
    minimize(2 * v + 2, cur);
}

void do_test() {
    cin >> n;

    REP(i, n) {
        cin >> arr[i];
        blocking[i] = 2;
    }

    minimize(0, arr[0]);

    // Greedy approach - we try to deactivate bombs that are accessible with
    // the shortest time to the explosion depending on this bomb.
    set<PII> to_deactivate;
    // Initiating bombs possible to deactivate by the ground layer.
    for (int i = (n + 1) / 2 - 1; i < n; i++) {
        to_deactivate.insert(MP(arr[i], i));
    }

    int cur_time = 1;
    bool res = true;
    while (true) {
        // Taking bomb which will explode first.
        PII el = *to_deactivate.begin();
        to_deactivate.erase(to_deactivate.begin());
        int v = el.ND;
        int el_time = el.ST;

        if (cur_time > el_time) {
            // We are too late - the bomb has already exploded.
            res = false;
            break;
        }

        if (v) {
            int parent = (v - 1) / 2;
            blocking[parent]--;
            if (blocking[parent] == 0) {
                // Our parrent is possible to deactivate now.
                to_deactivate.insert(MP(arr[parent], parent));
            }
        } else {
            // Success - we managed to deactivate all bombs.
            break;
        }
        cur_time++;
    }

    if (res) {
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
