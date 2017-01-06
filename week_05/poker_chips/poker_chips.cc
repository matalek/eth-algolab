// Poker Chips
// Week 5
// Aleksander Matusiak

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <deque>
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

const int BASE_BIN = 11;
const int NIL = -1;

// Codes given vector of integers (stack sizes) into one scoreue, assuming that
// all scoreues from vector are lower than 2^BASE_BIN
LL code(VI& v, int n) {
    LL res = 0;
    REP(i, n) { res = (res << ((LL)BASE_BIN)) + v[i]; }
    return res;
}

void do_test() {
    int n;
    cin >> n;

    int sizes_sum = 0;
    VI sizes(n);
    REP(i, n) {
        cin >> sizes[i];
        sizes_sum += sizes[i];
    }

    map<LL, int> res;  // for each code of the stack sizes keeps the maximum
                       // scored that can be gained by removing chips from the
                       // original stacks till we achieve this state (top-down
                       // approach)
    set<pair<int, LL>, greater<pair<int, LL> > >
        to_process;  // (sum of chips on all stacks, code of stack sizes)
    set<LL> added;   // codes for stack sizes that we added

    vector<VI> chips(n);
    REP(i, n) {
        chips[i] = VI(sizes[i]);
        REP(j, sizes[i]) { cin >> chips[i][j]; }
    }

    LL starting_code = code(sizes, n);
    to_process.insert(MP(sizes_sum, starting_code));
    added.insert(starting_code);

    // While there exists a configuration that we have not yet processed.
    while (!to_process.empty()) {
        // Take out the configuration with the biggest number of sum of stack
        // sizes.
        auto it = to_process.begin();
        int cur_cnt = it->ST;
        LL cur_code = it->ND;
        int cur_score = res[cur_code];
        to_process.erase(it);

        // cout << cur_cnt << "   " <<  cur_code << " " << cur_score << "\n";

        // Iterate over all posibilites from which stacks we can take a chip.
        FOR(mask, 1, 1 << n) {
            int color = NIL;
            bool correct = true;  // keeps track whether this configuration of
                                  // chips to take is feasible
            int cnt = 0;          // counts number of chosen stacks
            LL new_code = cur_code;

            // Go over all the stacks.
            REP(i, n) {
                // Whether we're taking a chip from this stacks.
                if (mask & (1 << i)) {
                    // Checks whether there are any chips left to take from this
                    // stack.
                    LL right_offset = (LL)BASE_BIN * (LL)(n - i - 1);
                    LL full_stack = ((LL)1 << (LL)(BASE_BIN)) -
                                    1;  // corresponds to code of stack with
                                        // 2^BASE_BIN - 1 size
                    LL stack_size = (cur_code >> right_offset) & full_stack;
                    if (!stack_size) {
                        correct = false;
                        break;
                    }

                    // Checks if we are consistent with the color of previously
                    // taken chips (if any present).
                    int current_color = chips[i][stack_size - 1];
                    if (color != NIL && color != current_color) {
                        correct = false;
                        break;
                    }
                    color = current_color;

                    new_code -= ((LL)1) << right_offset;
                    cnt++;
                }
            }

            if (correct) {
                // Updates score.
                int new_score = cur_score;
                if (cnt > 1) {
                    new_score += (1 << (cnt - 2));
                }

                if (added.find(new_code) != added.end()) {
                    // If we have already added this configuration for further
                    // configuration, only update scoreue.
                    res[new_code] = max(res[new_code], new_score);
                } else {
                    // Otherwise also add it to process later.
                    res[new_code] = new_score;
                    to_process.insert(MP((cur_cnt - cnt), new_code));
                    added.insert(new_code);
                }
            }
        }
    }

    cout << res[0] << "\n";
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
