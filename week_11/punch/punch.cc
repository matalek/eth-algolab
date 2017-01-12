// Punch
// Week 11
// Aleksander Matusiak

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <deque>
#include <queue>
#include <climits>
#include <map>

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
typedef map<int, int> MII;

int res;  // keeps current maximal number of distinct bevarages to achieve
          // cheapest punch

void dfs(int cur,    // current volume to achieve
         MII& used,  // (volumen, cnt) for used beverages
         VI& cheapest,
         MII& vol_price,
         MII& vol_cnt,
         int last  // keeps lastly taken volume
         ) {
    if (cur == 0) {
        // We have taken all necessary beverages. Now count number of distinct
        // beverages.
        int temp = 0;
        for (auto vol : used) {
            // There are vol_cnt[v.ST] bevarages with this volume, so the number
            // of distinct bevarages for this volume cannot exceed it.
            temp += min(vol.ND, vol_cnt[vol.ST]);
        }
        res = max(res, temp);
        return;
    }

    for (auto el : vol_price) {
        int vol = el.ST;
        // We want to limit the traverse tree, so we consider taking volumes in
        // not decreasing order.
        if (vol < last) {
            continue;
        }

        int price = el.ND;
        if ((cur - vol >= 0) &&
            (cheapest[cur - vol] == cheapest[cur] - price)) {
            // Taking this volume still allows us to achieving minimum price.
            if (used.find(vol) == used.end()) {
                used[vol] = 1;
            } else {
                used[vol]++;
            }
            // Perform further search with this volume taken.
            dfs(cur - vol, used, cheapest, vol_price, vol_cnt, vol);
            used[vol]--;
        }
    }
}

void do_test() {
    int n, k;
    cin >> n >> k;

    VI prices_temp(n), volumes_temp(n);  // vectors for reading input
    VI prices, volumes;  // vectors for parsed input, where we remove beverages
                         // with the same volume, but higher price

    map<int, int> vol_price;  // keeps smallest price for given volumen
    map<int, int> vol_cnt;  // keeps the count of beverages with this volume and
                            // the smallest price

    int max_vol = 0;
    REP(i, n) {
        cin >> prices_temp[i] >> volumes_temp[i];
        if (vol_price.find(volumes_temp[i]) == vol_price.end() ||
            vol_price[volumes_temp[i]] > prices_temp[i]) {
            vol_price[volumes_temp[i]] = prices_temp[i];
            vol_cnt[volumes_temp[i]] = 0;
        }
        max_vol = max(max_vol, volumes_temp[i]);
    }

    // Create vectors with removed unnecessary bevarages.
    REP(i, n) {
        if (vol_price[volumes_temp[i]] == prices_temp[i]) {
            volumes.PB(volumes_temp[i]);
            vol_cnt[volumes_temp[i]]++;
            prices.PB(prices_temp[i]);
        }
    }
    n = volumes.size();

    int range = k + 1 + max_vol;  // possible range of volumes that can be
                                  // the cheapest to achieve
    VI cheapest(range, INT_MAX);  // keeps cheepest cost to achieve given volume
    cheapest[0] = 0;
    // Using dynamic programming, update the cheapest vector.
    FOR(i, 1, range) {
        REP(j, n) {
            if ((volumes[j] <= i) && (cheapest[i - volumes[j]] != INT_MAX)) {
                int new_price = cheapest[i - volumes[j]] + prices[j];
                if (new_price < cheapest[i]) {
                    cheapest[i] = new_price;
                }
            }
        }
    }

    int cheapest_res = INT_MAX;
    VI to_check;  // which total volumes can be achieved in a cheapest way
    // Computes cheepest price to satify everybody and total volumes that can
    // achieve it.
    FOR(i, k, range) {
        if (cheapest[i] < cheapest_res) {
            cheapest_res = cheapest[i];
            to_check = VI(1, i);
        } else if (cheapest[i] == cheapest_res) {
            to_check.PB(i);
        }
    }

    res = 0;
    // For each total volume that achieve cheapest price compute the maximal
    // number of distint beverages by performing search over possible volumes to
    // take.
    for (int start : to_check) {
        MII s;
        dfs(start, s, cheapest, vol_price, vol_cnt, 0);
    }

    cout << cheapest_res << " " << res << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
