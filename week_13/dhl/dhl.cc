// DHL
// Week 13
// Aleksander Matusiak
//
// Solution that scored 100/100 points, solving also challenging test set. Time
// complexity: O(n^2).

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
typedef vector<VVI> VVVI;
typedef long long int LL;

// Returns sum a + b handling a special case when one or both of them correspond
// to infinity (INT_MAX).
int add_safely(int a, int b) {
    if (a == INT_MAX || b == INT_MAX) {
        return INT_MAX;
    }
    return a + b;
}

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
    // which have i and j parcels respectively. It will be equal INT_MAX if
    // there is no possible unloading.
    VVI res(n + 1, VI(n + 1));

    // res_par[one_side][i][j] - minimum cost of unloading parcels from left and
    // right stacks which have i and j parcels respectively when we assume that
    // in the next move from one_side we take only one parcel. It will be equal
    // INT_MAX if there is no possible unloading.
    VVVI res_par(2, VVI(n + 1, VI(n + 1)));

    // Compute result for all possible states of stacks.
    FOR(i, 1, n + 1) {
        FOR(j, 1, n + 1) {
            // The cost that we will have to pay whatever move we make.
            int current_level_cost = stacks[0][i - 1] * stacks[1][j - 1];

            // In optimal solution we take one parcel from one stack and one or
            // more parcels from another. Therefore, consider 2 sides from which
            // we can take only one parcel.
            REP(side, 2) {
                // We are taking one parcel from side side (0 - left, 1 -
                // right). Compute corresponding values dependent on the side.
                const int other = 1 - side;
                const int side_index = side ? j : i;
                const int other_index = side ? i : j;

                // We can take only one parcel also from the other stack.
                int res_take_one =
                    add_safely(current_level_cost, res[i - 1][j - 1]);

                // Or we can take more parcels. This move can be considered as
                // paying cost for current level and then following optimal
                // strategy for another configuration under assumption that we
                // have to take one element from fixed side. We can therefore
                // use here computed partial results where we assume that one
                // element is taken from particular side.
                int recursive_left_size = side ? i - 1 : i;
                int recursive_right_size = side ? j : j - 1;
                int res_take_more = add_safely(
                    current_level_cost,
                    res_par[side][recursive_left_size][recursive_right_size]);

                if (side_index != 1 && other_index == 1) {
                    // There is no possible move where we take only one parcel
                    // from side side.
                    res_par[side][i][j] = INT_MAX;
                } else if (side_index == 1) {
                    // We have to take all from the other stack; in particular -
                    // more than one.
                    res_par[side][i][j] = res_take_more;
                } else {
                    // We have no restrictions, so we choose optimal move.
                    res_par[side][i][j] = min(res_take_one, res_take_more);
                }
            }

            // We can take one parcel from left or from right side.
            res[i][j] = min(res_par[0][i][j], res_par[1][i][j]);
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
