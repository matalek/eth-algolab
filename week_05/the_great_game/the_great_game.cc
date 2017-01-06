// The Great Game
// Week 5
// Aleksander Matusiak

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <deque>
#include <climits>

#define REP(i, n) for (int i = 0; i < (n); ++i)
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

enum MEEPLE {
    MY_MEEPLE = 0,
    OPONENTS_MEEPLE = 1,
};

void do_test() {
    int n, m, start1, start2;
    cin >> n >> m >> start1 >> start2;
    start1--;
    start2--;

    vector<VI> transitions(n);   // transitions between positions on board
    VI res[2] = {VI(n), VI(n)};  // res[whos_meeple][i] - if whos_meeple ==
                                 // MY_MEEPLE - minimal time it takes me to move
                                 // my meeple from position to the target.
                                 // Otherwise - maximal time for my oponent to
                                 // move his meeple.

    REP(i, m) {
        int a, b;
        cin >> a >> b;
        a--;
        b--;
        transitions[a].PB(b);
    }

    // n - 1 corresponds to the target position.
    res[MY_MEEPLE][n - 1] = res[OPONENTS_MEEPLE][n - 1] = 0;

    // Dynamic programming - going from right to left, compute best time to
    // reach the target.
    for (int i = n - 2; i >= 0; i--) {
        int my_val = INT_MAX;  // we want to minimilize time if I am making the
                               // move with my meep;e
        int oponents_val = 0;  // and maximalize it, when I am making the move
                               // with oponent's meeple
        for (int w : transitions[i]) {
            my_val = min(my_val, res[OPONENTS_MEEPLE][w]);  // after my move, it
                                                            // will be my
                                                            // oponent's who
                                                            // moves my meeple
            oponents_val =
                max(oponents_val,
                    res[MY_MEEPLE]
                       [w]);  // after my move, the oponent will move his meeple
        }

        res[MY_MEEPLE][i] = my_val + 1;
        res[OPONENTS_MEEPLE][i] = oponents_val + 1;
    }

    int does_holmes_win =
        (res[MY_MEEPLE][start1] <
         res[MY_MEEPLE][start2]) ||  // Holmes wins if he finishes in less turns
        (res[MY_MEEPLE][start1] ==
             res[MY_MEEPLE][start2] &&  // or in the same number of turns, but
                                        // in the last turn Moriarty moves
                                        // Holmes' meeple first
         res[MY_MEEPLE][start1] % 2);

    cout << !does_holmes_win << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
