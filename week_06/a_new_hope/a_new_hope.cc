// A New Hope
// Week 6, Problem of the Week
// Aleksander Matusiak

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <deque>

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

const int K = 20003;
const int S = 15;

int k, s, m;
VI inside[K][S];  // inside[i][j] - list of stormtroopers supervised by
                  // stormtrooper j inside command center j
VI who[K];  // who[i] - who is a superviser of stormtroopers in command center i
            // (numbers of stormtroopers)
VI supervisors[K][S];  // supervisors[i][j] -  who is a superviser of
                       // stormtrooper j in command center j
VI outside[K];  // outside[i] - keeps list of all command centers where some
                // stortroopers are supervised by a stormtrooper for command
                // center i

enum Supervisors {
    NONE = 0,
    FIRST = 1,
    SECOND = 2,
    BOTH = 3,
};

// Gets supervision type based on presence of the supervisors.
inline Supervisors get_type(bool first_present, bool second_present) {
    return Supervisors(2 * second_present + first_present);
}

int part_res[K][4];  // part_res[i][sup] - partial results when considering i-th
                     // command center and from above command center we can only
                     // take these stromtroopers which correspond to sup (of
                     // type Sypervisors)

// Checks if vector v contains el.
bool contains(VI& v, int el) {
    return find(v.begin(), v.end(), el) != v.end();
}

// Adds element to the vector, if it has not been present before.
// We are using this solution over using sets because in this task such vectors
// will be small.
void add_no_duplicates(VI& v, int el) {
    if (!contains(v, el)) {
        v.PB(el);
    }
}

// Checks if el is an elements of set represented as binary mask.
bool contains(int set, int el) {
    return set & (1 << el);
}

// Checks if there is no supervising relation between stormtroopers in this
// subset.
bool check_independency(int v, int subset) {
    VI elems;
    REP(i, s) {
        if (contains(subset, i)) {
            elems.PB(i);
        }
    }

    REP(i, elems.size()) {
        FOR(j, i + 1, elems.size()) {
            int x = elems[i], y = elems[j];
            if (contains(inside[v][x], y) || contains(inside[v][y], x)) {
                return false;
            }
        }
    }
    return true;
}

// Counts the number of elements in the subset.
int subset_size(int subset) {
    int cnt = 0;
    REP(i, s) {
        if (contains(subset, i)) {
            cnt++;
        }
    }
    return cnt;
}

// Performs computations for command center v and all reachable (with regards to
// supervising relation) command centers.
void traverse(int v) {
    // First perform computations for command centers where subcoordinates for
    // stormtroopers in the current center.
    for (int out : outside[v]) {
        traverse(out);
    }

    // Iterate over evey possible subset of stormtroopers inside the command
    // center.
    REP(subset, 1 << s) {
        if (!check_independency(v, subset)) {
            // Not valid subset. - we have a pair of stormtroopers where one is
            // a superviser of another
            continue;
        }

        int cnt = subset_size(subset);
        for (int out : outside[v]) {
            // Checks whether some of stormtroopers in the current subset are
            // supervisors of stormtrooper in out command center.
            int who_size = who[out].size();
            bool is_first = false, is_second = false;
            is_first = contains(subset, who[out][0]);
            if (who_size == 2) {
                is_second = contains(subset, who[out][1]);
            }

            // We can only take recursive results which corresponds to the
            // supervisors in the current set.
            cnt += part_res[out][get_type(is_first, is_second)];
        }

        // We know now what is the score that we can achieve for given subset.
        // Now we need to assign an appropriate supervising type for this
        // subset.

        bool is_first = false, is_second = false;
        if (v != 0) {  // otherwise we don't care
                       // Iterate over stormtroopers.
            REP(i, s) {
                if (!contains(subset, i)) {
                    continue;
                }

                // Checking if this stormtrooper is supervised by some (and how
                // many) stormtroopes from different command center.
                if (supervisors[v][i].size() == 2) {
                    is_first = is_second = true;
                } else if (supervisors[v][i].size() == 1) {
                    if (who[v][0] == supervisors[v][i][0]) {
                        is_first = true;
                    } else {
                        is_second = true;
                    }
                }
            }
        }
        int type = get_type(is_first, is_second);

        // Now we know which supervisors from outside command center are
        // supervising stormtroopers from our subset. We need to translate it to
        // which supervisers are possible to take in the outside command center.
        VI to_update;
        if (type == BOTH) {
            // We cannot take any supervisor - any of them would create a
            // supervising relation with one of the stormtroopers from our
            // subset.
            to_update.PB(NONE);
        } else if (type != NONE) {
            // We can only take one.
            to_update.PB((type == FIRST) ? SECOND : FIRST);
            to_update.PB(NONE);
        } else {
            // No limitations - we can take as many as we want.
            REP(i, 4) { to_update.PB(i); }
        }

        // Update possible types.
        for (int type : to_update) {
            part_res[v][type] = max(part_res[v][type], cnt);
        }
    }
}

void do_test() {
    cin >> k >> s >> m;

    // Cleaning up global data.
    REP(i, k) {
        REP(j, s) {
            inside[i][j].clear();
            supervisors[i][j].clear();
        }
        REP(j, 4) { part_res[i][j] = 0; }
        who[i].clear();
        outside[i].clear();
    }

    REP(i, m) {
        int u, v, h;
        cin >> u >> v >> h;
        if (u != v) {
            // Create outside connection between distinct command centers.
            add_no_duplicates(outside[u], v);
        }
        REP(j, h) {
            int x, y;
            cin >> x >> y;
            if (u == v) {
                // The same command center.
                add_no_duplicates(inside[u][x], y);
            } else {
                // Different command centers.
                add_no_duplicates(who[v], x);
                add_no_duplicates(supervisors[v][y], x);
            }
        }
    }

    // Performs calculations starting from C_0 command center.
    traverse(0);

    // Command center C_0 has no outside supervisors, so results for each type
    // will be the same - as a result we can take any of them.
    cout << part_res[0][NONE] << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
