// Evolution
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
typedef vector<PII> VPII;
typedef vector<int> VI;
typedef long long int LL;

const int N = 50003;

VI g[N];
VPII queries[N];
string names[N];
string results[N];
int age[N];
bool vis[N];
bool any_in[N];
VPII anc;  // stack of the ancestors

int n, m;

// Performs binary search on the ancestors stack in order to find the ancestor
// of a given age (the closest one).
int find_age(int age) {
    int n = anc.size();
    int sbeg = 0, send = n - 1;

    while (sbeg != send) {
        int mid = (sbeg + send) / 2;
        if (anc[mid].ST <= age) {
            send = mid;
        } else {
            sbeg = mid + 1;
        }
    }

    return anc[sbeg].ND;
}

// Performs DFS from vertex v and answers saved queries.
void dfs(int v) {
    vis[v] = true;
    // Pushing current vertex on ancestors stack.
    anc.PB(MP(age[v], v));

    // Answering queries.
    for (PII query : queries[v]) {
        int age = query.ST;
        int a = find_age(age);
        results[query.ND] = names[a];
    }

    // Visiting descendants.
    for (int w : g[v]) {
        if (!vis[w]) {
            dfs(w);
        }
    }

    anc.pop_back();
}

void do_test() {
    cin >> n >> m;

    map<string, int> numbers;  // keeping indexes for species names

    // Reading in names of species and clearing global data.
    REP(i, n) {
        cin >> names[i] >> age[i];
        g[i].clear();
        queries[i].clear();
        numbers[names[i]] = i;
        any_in[i] = false;
    }

    // Building the graph.
    REP(i, n - 1) {
        string a, b;
        cin >> a >> b;
        int v = numbers[a], w = numbers[b];
        g[w].PB(v);
        any_in[v] = true;
    }

    // Reading queries.s
    REP(i, m) {
        string a;
        int b;
        cin >> a >> b;
        queries[numbers[a]].PB(MP(b, i));
    }

    // Finding root of the tree.
    int start;
    REP(i, n) {
        vis[i] = false;
        if (!any_in[i]) {
            start = i;
        }
    }

    // Starting DFS from the root.
    anc.clear();
    dfs(start);

    REP(i, m) { cout << results[i] << " "; }
    cout << "\n";
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
