// Moving books
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

void do_test() {
    int n, m;
    cin >> n >> m;

    VI friends(n);
    multiset<int, greater<int> > weigths;

    REP(i, n) {
        cin >> friends[i];
    }

    REP(i, m) {
        int a;
        cin >> a;
        weigths.insert(a);
    }

    sort(friends.begin(), friends.end());

    int cur_friend = 0;
    int time = 2;
    bool something = false; // if something has been moved in the current turn
    VI new_friends; // friends working in the current turn

    while(weigths.size()) {
        // Turn finishes - all friends that still have enough strength return
        // upstairs.
        if (cur_friend == friends.size()) {
            if (!something) break;
            time += 3;
            cur_friend = 0;
            friends = new_friends;
            new_friends = VI();
            something = false;
        }

        int strength = friends[cur_friend];
        // Looking for the heaviest box that the friend can carry.
        auto it = weigths.lower_bound(strength);
        if (it != weigths.end()) {
            weigths.erase(it);
            // Friend might still be useful in the future.
            new_friends.push_back(strength);
            something = true;
        }
        // Otherwise we are not adding the friend to new_friends, cause
        // he won't be able to carry any more boxes.

        cur_friend++;
    }

    if (something) {
        cout << time << "\n";
    } else {
        cout << "impossible\n";
    }
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
