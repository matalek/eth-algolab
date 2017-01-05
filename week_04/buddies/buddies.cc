// Buddies
// Week 4
// Aleksander Matusiak

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <deque>
#include <climits>
#include <cassert>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

#define REP(i, n) for (int i = 0; i < (n); ++i)
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ST first
#define ND second
#define PB push_back
#define MP make_pair
#define deb(a)

using namespace boost;
using namespace std;

typedef adjacency_list<vecS,
                       vecS,
                       undirectedS,
                       no_property,
                       property<edge_weight_t, int> >
    Graph;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_iterator EdgeIt;
typedef graph_traits<Graph>::out_edge_iterator OutEdgeIt;
typedef property_map<Graph, edge_weight_t>::type WeightMap;

typedef vector<pair<int, int> > VPII;
typedef vector<int> VI;

void do_test() {
    int n, c, f;
    cin >> n >> c >> f;
    Graph g(n);  // representing connections between students who share more
                 // than f common characteristics
    vector<vector<string> > characteristics(n);

    REP(i, n) {
        REP(j, c) {
            string characteristic;
            cin >> characteristic;
            characteristics[i].PB(characteristic);
        }
        sort(characteristics[i].begin(), characteristics[i].end());
    }

    REP(i, n) {
        FOR(j, i + 1, n) {
            // Calculating the intersection between characteristics set.
            // Possible to do, because vectors represented characteristics
            // are sorted.
            vector<string> common_characteristics(c);
            auto it = set_intersection(
                characteristics[i].begin(), characteristics[i].end(),
                characteristics[j].begin(), characteristics[j].end(),
                common_characteristics.begin());
            int common_characteristics_cnt =
                it - common_characteristics.begin();

            // Students share more than f common characteristics - add edge
            // representing the possible connection between them.
            if (common_characteristics_cnt > f) {
                Edge e;
                bool success;
                tie(e, success) = add_edge(i, j, g);
            }
        }
    }

    // Computing maximum matching.
    vector<Vertex> mate(n);
    edmonds_maximum_cardinality_matching(g, &mate[0]);
    int matched_students = matching_size(g, &mate[0]) * 2;

    if (matched_students == n) {
        cout << "not optimal\n";
    } else {
        cout << "optimal\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
