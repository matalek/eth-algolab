// DHL
// Week 13
// Aleksander Matusiak
//
// Solution that scored 99/100 points, solving one but last challenging test
// set. Time complexity: O(n^3).

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <deque>
#include <queue>
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
typedef vector<VI> VVI;
typedef long long int LL;

void do_test() {
    int n;
    cin >> n;

    VVI stacks(2, VI(n));
    REP(side, 2) {
        REP(i, n) {
            cin >> stacks[side][i];
            // When we substract 1 from each weight then the cost of
            // one loading will be equal to simpy S_a * S_b.
            --stacks[side][i];
        }
    }

    // We will equivalently consider removing parcels from both stacks, instead
    // of loading them there.

    // res[i][j] - minimum cost of unloading parcels from left and right stacks
    // which have i and j parcels respectively
    VVI res(n + 1, VI(n + 1));

    // Compute result for all possible states of stacks.
    FOR(i, 1, n + 1) {
        FOR(j, 1, n + 1) {
            int cur_res = INT_MAX;  // optimal cost of unloading stacks in this
                                    // configuration

            // In optimal solution we take one parcel from one stack and one or
            // more parcels from another. Therefore, consider 2 sides from which
            // we can take only one parcel.
            REP(side, 2) {
                // We are taking one parcel from side side (0 - left, 1 -
                // right). Compute corresponding values dependent on the side.
                const int other = 1 - side;
                const int side_index = side ? j : i;
                const int other_index = side ? i : j;

                // Weight of parcel taken from side side.
                const int one_parcel = stacks[side][side_index - 1];

                // Consider taking from 1 to all possible parcels from another
                // stack.
                int sum = 0;  // current sum of taken parcels from another stack
                FOR(k, 1, other_index + 1) {
                    sum += stacks[other][other_index - k];

                    // How many parcels we are taking from each side.
                    const int take_from_left = side ? k : 1;
                    const int take_from_right = side ? 1 : k;

                    // If there is only one parcel on side side, we need to take
                    // all parcels from other side. Similarly, when there is
                    // more than one parcel, we cannot take all from the other
                    // stack.
                    if (((side_index == 1) && (k == other_index)) ||
                        ((side_index != 1) && (k != other_index))) {
                        // Score for this move is cost of this move and cost
                        // necessary to remove parcels that are left.
                        int potential_cur_res =
                            res[i - take_from_left][j - take_from_right] +
                            one_parcel * sum;
                        cur_res = min(cur_res, potential_cur_res);
                    }
                }
            }

            res[i][j] = cur_res;
        }
    }

    // We have to unload all parcels.
    cout << res[n][n] << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
