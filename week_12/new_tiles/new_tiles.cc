// New Tiles
// Week 12
// Aleksander Matusiak

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <deque>
#include <queue>
#include <climits>

#define REP(i, n) for (int i = 0; i < (n); ++i)
#define FOR(i, a, mask) for (int i = (a); i < (mask); ++i)
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

const int H = 103, W = 17, W2 = (1 << W);
int res[H][W2], arr[H][W];

void do_test() {
    int h, w;
    cin >> h >> w;

    int trash;  // variable for reading in unused input

    // Consider special cases separately.
    if (h < 3 || w < 3) {
        REP(i, h) {
            REP(j, w) { cin >> trash; }
        }
        cout << 0 << "\n";
        return;
    }

    // Cut out border - we know that there will be only 0s and we want to reduce
    // execution time.
    h -= 2;
    w -= 2;
    int bitmasks = 1 << w;

    vector<VI> arr(h, VI(w));  // storing input data

    REP(i, w + 2) { cin >> trash; }

    REP(i, h) {
        cin >> trash;
        REP(j, w) { cin >> arr[i][j]; }
        cin >> trash;
    }

    REP(i, w + 2) { cin >> trash; }

    vector<VI> res(h, VI(1 << w));  // res[row][bitmask] - maximum number of
                                    // tiles that we can put on the floor till
                                    // row row, where on the last row we can put
    // only tiles that are allowed by the bitmask

    FOR(i, 1, h) {
        // If we don't want to use the new row at all.
        int no_used_res = 0;
        REP(mask, bitmasks) {
            no_used_res = max(no_used_res, res[i - 1][mask]);
        }
        res[i][0] = no_used_res;

        FOR(mask, 1, bitmasks) {
            res[i][mask] = 0;
            // Leaving a square of the new row empty.
            REP(j, w) {
                if ((1 << j) & mask) {
                    res[i][mask] = max(res[i][mask], res[i][mask - (1 << j)]);
                }
            }

            // Checking if there are only even consecutive ones and counting
            // them.
            bool to_consider = true;  // if we have only consecutive ones and
                                      // mask is consistent with the actuall
                                      // floor plan
            int cnt = 0, cur_cnt = 0;
            REP(j, w) {
                if (mask & (1 << j)) {
                    if (!arr[i][j] || !arr[i - 1][j]) {
                        to_consider = false;
                        break;
                    }
                    cur_cnt++;
                } else {
                    if (cur_cnt % 2) {
                        to_consider = false;
                        break;
                    }
                    cnt += cur_cnt;
                    cur_cnt = 0;
                }
            }
            if (cur_cnt % 2) {
                to_consider = false;
            } else {
                cnt += cur_cnt;
            }

            if (to_consider) {
                // Consider taking all possible tiles allowed by the mask and
                // floor plan. Then, in the previous row, corresponding squares
                // have to be left empty.
                res[i][mask] = max(res[i][mask],
                                   res[i - 1][(1 << w) - 1 - mask] + cnt / 2);
            }
        }
    }

    // For the result we take most permissive bitmask.
    cout << res[h - 1][bitmasks - 1] << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
