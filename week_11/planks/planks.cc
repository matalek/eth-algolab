// Planks
// Week 11, Problem of the Week
// Aleksander Matusiak

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <deque>
#include <queue>

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

// Counts sum of taken planks for given mask.
LL cnt_sum(VI& planks, int mask) {
    int cur = 1;
    LL sum = 0;
    REP(i, planks.size()) {
        if (mask & cur) {
            sum += planks[i];
        }
        cur <<= 1;
    }
    return sum;
}

// Creates a vector which counts sums with the same weight. It's element has
// form (w, cnt), which means that there are cnt sums with weight w.
VPII vector_count(VI& sums) {
    sort(sums.begin(), sums.end());
    VPII res;

    int last = sums[0], cnt = 1;
    FOR(i, 1, sums.size()) {
        if (last == sums[i]) {
            cnt++;
        } else {
            res.PB(MP(last, cnt));
            last = sums[i];
            cnt = 1;
        }
    }
    res.PB(MP(last, cnt));
    return res;
}

// Counts the number of possibilities of splitting planks into 2 sides, each
// with sum to_achieve.
LL count(VI& planks, int to_achieve) {
    int n = planks.size();

    // Equivalently, we need to count subsets of planks that sum up to
    // to_achieve. We will do it by splitting planks into 2 sets and for each
    // possible sum k from the first group, we will try to find sum to_achieve -
    // k in the second group.

    // Split planks into two equal groups.
    VI s1, s2;
    REP(i, n / 2) { s1.PB(planks[i]); }
    FOR(i, n / 2, n) { s2.PB(planks[i]); }

    // Create vectors of possible sums for each group.
    VI l1, l2;
    REP(mask, 1 << s1.size()) { l1.PB(cnt_sum(s1, mask)); }
    REP(mask, 1 << s2.size()) { l2.PB(cnt_sum(s2, mask)); }

    // Accumulating the same sums to enable to easily count possible splits.
    VPII lp1 = vector_count(l1), lp2 = vector_count(l2);

    LL res = 0;
    for (PII el : lp2) {
        // For the given sum try to find correponding sum in the second set.
        int target = to_achieve - el.ST;
        auto it = lower_bound(lp1.begin(), lp1.end(), MP(target, 0));
        if (it != lp1.end() && it->ST == target) {
            // Multiply number of possibilites to achieve given sum in the frist
            // group by number of possibilities to achieve corresponding sum in
            // the second group.
            res += (LL)el.ND * (LL)it->ND;
        }
    }
    return res;
}

void do_test() {
    int n;
    cin >> n;
    VI planks(n);
    LL total_sum = 0;
    REP(i, n) {
        cin >> planks[i];
        total_sum += planks[i];
    }

    if (total_sum % 4) {
        // We already cannot make all sides equal.
        cout << 0 << "\n";
        return;
    }

    LL to_achieve = total_sum / 4;
    LL res = 0;

    // Go over all possible mask designating which planks to take in order to
    // construct half of the square.
    REP(mask, 1 << n) {
        // In order to reduce computing time we assume that first plank has to
        // be in this mask.
        if (mask & 1 || cnt_sum(planks, mask) != to_achieve * 2) {
            continue;
        }

        // Create vectors for planks in the group corresponding mask (first) and
        // in the other group (second).
        VI first, second;
        int cur = 1;
        REP(i, n) {
            if (mask & cur) {
                first.PB(planks[i]);
            } else {
                second.PB(planks[i]);
            }
            cur <<= 1;
        }

        // Compute numbers of splitting planks in each group into 2 sides and
        // multiply the results (they are independent).
        LL r1 = count(first, to_achieve), r2 = count(second, to_achieve);
        res += r1 * r2;
    }

    // Divide by number of possible permutations while taking into account that
    // we already assumed that first plank is in the specific half of the
    // square.
    cout << res / 12 << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
