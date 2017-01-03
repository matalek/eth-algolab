// Search snippet
// Week 2
// Aleksander Matusiak

#include <algorithm>
#include <cassert>
#include <climits>
#include <deque>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>

#define REP(i, n) for (int i = 0; i < (n); ++i)
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
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

const int N = 2050, NIL = -1;
int m[N], occurences[N];
VPII words;
int cnt, interval_start, interval_end, res, n;

// Moves interval's end as long as we have every word occuring at least once
// or we encounter the end of all words.
void fill_up() {
    while (cnt < n && interval_end < words.size()) {
        auto a = words[interval_end].second;
        occurences[a]++;
        if (occurences[a] == 1) {
            cnt++;
        }
        interval_end++;
    }
}

// Checks whether every words occurs at least once. If yes, it updates current
// resulting interval length.
void check() {
    if (cnt < n) return;
    int diff = words[interval_end - 1].first - words[interval_start].first + 1;
    if (res == NIL || diff < res) {
        res = diff;
    }
}

void do_test() {
    cin >> n;
    REP(i, n) {
        cin >> m[i];
        occurences[i] = 0;
    }

    words.clear();
    REP(i, n) {
        REP(j, m[i]) {
            int a;
            cin >> a;
            words.push_back(make_pair(a, i));
        }
    }

    // Sort words occurences based on the position where it occures.
    sort(words.begin(), words.end());

    // The solution is based on the sliding window approach. We call fill_up
    // to make sure that, if possible, every words occur at least once in
    // the inverval (interval_start, interval_end). We then check whether
    // we have improved the result.
    cnt = 0; res = NIL; interval_start = 0; interval_end = 0;
    fill_up();
    check();

    while (interval_start < words.size() && interval_end < words.size()) {
        // Moving the interval start to the right.
        auto a = words[interval_start].second;
        occurences[a]--;
        if (occurences[a] == 0) {
            cnt--;
        }
        interval_start++;
        fill_up();
        check();
    }

    cout << res << "\n";
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
