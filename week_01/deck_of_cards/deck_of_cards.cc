// Deck of cards
// Week 1, Problem of the Week
// Aleksander Matusiak
//
// Proper solution which uses sliding window approach and works in O(n) time.

#include <algorithm>
#include <climits>
#include <deque>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>
#include <cstdlib>

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

    VI cards(n);

    REP(i, n) { cin >> cards[i]; }

    PII result;  // stores indexes of beggining and one element after end for
                 // resulting interval
    int closest_value =
        INT_MAX;  // sum closest to k that has been already found
    int index_l = 0, index_r = 1, current_sum = cards[0];

    // Iterate the left edge of the interval.
    while (index_l < n) {
        // Increase the right edge of the interval till the sum is lower than
        // desired.
        while (index_r < n && current_sum < k) {
            current_sum += cards[index_r];
            index_r++;
        }

        // Because we're interested in the sum that can be lower or greater than
        // k, we may need to go over two possible endings - one corresponding to
        // value greater, another one to value lower than k.
        vector<pair<int, PII> > to_consider;
        to_consider.PB(MP(current_sum, MP(index_l, index_r)));
        if (index_r > index_l + 1) {
            to_consider.PB(
                MP(current_sum - cards[index_r - 1], MP(index_l, index_r - 1)));
        }

        // Move the left edge.
        current_sum -= cards[index_l];
        index_l++;

        // Iterating through possible intervals.
        for (auto el : to_consider) {
            int diff = abs(el.ST - k);
            if (diff < closest_value ||
                (diff == closest_value && el.ND < result)) {
                // Update current result - we found something better.
                closest_value = diff;
                result = el.ND;
            }
        }
    }

    cout << result.ST << " " << result.ND - 1 << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
