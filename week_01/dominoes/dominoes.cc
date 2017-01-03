// Dominoes
// Week 1
// Aleksander Matusiak

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
typedef vector < PII > VPII;
typedef vector<int> VI;
typedef long long int LL;

void do_test() {
	int n;
	cin >> n;
	int arr[n];
	REP(i, n) {
		cin >> arr[i];
	}

	// Range of all falling tiles.
	int range = 1;
	int result = n;
	REP(i, n) {
		if (range <= i) {
			// The last falling domino tiles has not reached the current
			// domino tile.
			result = i;
			break;
		}
		range = max(range, i + arr[i]);
	}

	cout << result << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) {
        do_test();
    }
    return 0;
}
