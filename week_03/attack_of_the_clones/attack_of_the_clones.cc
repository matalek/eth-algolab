// Attack of the clones
// Week 3, Problem of the Week
// Aleksander Matusiak

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <deque>

#define REP(i, n) for (int i = 0; i < (n); ++i)
#define ST first
#define ND second
#define PB push_back
#define MP make_pair
#define deb(a)

using namespace std;

typedef vector<pair<int, int> > VPII;
typedef vector<int> VI;
typedef pair<int, int> PII;

const int N = 50003, NIL = -1;
PII seg[N];
int n, m;

// Traverses the circle, starting at start and finishing at end, and counts
// maximum number of indepent Jedis covering it.
int go_round(int start, int end) {
    vector<PII> s;  // (segment_end, segment_start), both elements relative
                    // to the beggining of the considered interval
    int cnt = 0;
    // Choosing segments which are inside the considered interval.
    REP(i, n) {
        if (start <= end && seg[i].ST <= seg[i].ND && seg[i].ST >= start &&
            seg[i].ST <= end && seg[i].ND >= start && seg[i].ND <= end) {
            // We don't need to go over the beggining of the circle.
            s.PB(MP(seg[i].ND - start, seg[i].ST - start));
        } else if (start > end && ((seg[i].ST - start + m) % m <=
                                   (seg[i].ND - start + m) % m) &&
                   (seg[i].ST >= start ||
                    (seg[i].ST >= 0 && seg[i].ST <= end)) &&
                   (seg[i].ND >= start ||
                    (seg[i].ND >= 0 && seg[i].ND <= end))) {
            // We need to go over the beggining of the circle.
            s.PB(MP((seg[i].ND - start + m) % m, (seg[i].ST - start + m) % m));
        }
    }

    sort(s.begin(), s.end());

    // Traversing segments sorted by their end and greedily taking possible
    // (not overlapping) ones.
    int cur_end = 0;
    for (PII o : s) {
        if (o.ND >= cur_end) {
            cnt++;
            cur_end = o.ST + 1;
        }
    }

    return cnt;
}

void do_test() {
    cin >> n >> m;

    vector<pair<int, bool> > v;  // (position, is_end)
    // Counts number of Jedi available to cover the current position.
    // We start with start (and the end) of the circle.
    int cur = 0;
    REP(i, n) {
        int a, b;
        cin >> a >> b;
        a--;
        b--;
        seg[i] = MP(a, b);
        v.PB(MP(seg[i].ST, false));
        v.PB(MP(seg[i].ND, true));
        if (a > b || b == m - 1 || a == 0) {
            cur++;
        }
    }

    sort(v.begin(), v.end());

    // We want to execute a greedy algorithm where we will traverse the circle
    // and take these Jedis whose segments finish as early as possible. In order
    // to do that, we need to start in an appropriate position. For segments
    // covering this position we need to consider taking each one of them (or
    // none of them). It is guaranteed that there exists a position witch is
    // covered by at most 10 Jedis. Therefore, we will find  the segment with
    // the smallest number of Jedis covering it.

    int start_pos;  // keeps the position with minimum number of Jedis
                    // covering it
    int index = 0;
    int cur_min = NIL;  // keeps minimum number of Jedis at one position

    // Traversing begginings and ends of segments in order of their
    // placement on the circle.
    while (index < 2 * n) {
        int t = v[index].ST;
        // Going over all the segments that start at t.
        while (v[index].ST == t && v[index].ND == false) {
            cur++;
            index++;
        }

        if (cur_min == NIL || cur <= cur_min) {
            cur_min = cur;
            start_pos = t;
        }

        // Going over all the segments that end at t.
        while (v[index].ST == t && v[index].ND == true) {
            cur--;
            index++;
        }
    }

    // Checking the beggining (end) of the circle
    if (cur < cur_min) {
        start_pos = 0;
    }

    // Computing Jedis covering starting possition.
    VI covering;
    REP(i, n) {
        if ((seg[i].ST <= seg[i].ND && seg[i].ST <= start_pos &&
             seg[i].ND >= start_pos) ||
            (seg[i].ST > seg[i].ND &&
             (start_pos >= seg[i].ST || start_pos <= seg[i].ND))) {
            covering.PB(i);
        }
    }

    int res = 0;
    // Considering taking each of the segments covering starting position.
    for (int c : covering) {
        int cur_res;
        if ((seg[c].ND + 1) % m == seg[c].ST) {
            // If segment is covering the whole circle.
            cur_res = 1;
        } else {
            cur_res = 1 + go_round((seg[c].ND + 1) % m, (seg[c].ST) % m);
        }
        res = max(res, cur_res);
    }

    // Considering not taking any of the covering Jedis.
    res = max(res, go_round((start_pos + 1) % m, (start_pos) % m));

    cout << res << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
