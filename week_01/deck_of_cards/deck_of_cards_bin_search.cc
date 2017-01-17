// Deck of cards
// Week 1, Problem of the Week
// Aleksander Matusiak
//
// Slower solution which uses binary search and works in O(nlogn). However, it
// scored 100/100.

#include <algorithm>
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

void do_test() {
    int n, k;
    cin >> n >> k;

    int current = 0, cur_min = -1;
    map<int, int> sums;
    sums.insert(MP(0, -1));
    PII result;

    REP(i, n) {
        int a;
        cin >> a;
        current += a;

        // Searching for the beggining of the interval which ends at i and
        // aproximates the desired sum k in the best way.
        int target = current - k;
        vector<map<int, int>::iterator> to_consider;
        auto it_bound = sums.lower_bound(target);
        if (it_bound != sums.end()) {
            to_consider.PB(it_bound);
        }
        // We might need to consider two possible begginings, one corresponding
        // to sum lower than k, the other one to sum bigger than k.
        if (it_bound != sums.begin()) {
            auto it = map<int, int>::iterator(it_bound);
            it--;
            to_consider.PB(it);
        }

        // Iterating through possible begginings.
        for (auto it : to_consider) {
            int act = abs(it->ST - target);
            int start = it->ND + 1;

            PII potential_result = MP(start, i);
            if (cur_min == -1 ||
                (act < cur_min ||
                 (act == cur_min && potential_result < result))) {
                // Potential result is better than the previously found one.
                result = potential_result;
                cur_min = act;
            }
        }
        // Updating map with prefix sums.
        sums.insert(MP(current, i));
    }

    cout << result.ST << " " << result.ND << "\n";
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
