// Avalanche (aka On Her Majesty's Secret Service)
// Week 5
// Aleksander Matusiak

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <deque>
#include <climits>
#include <cassert>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

#define REP(i, n) for (int i = 0; i < (n); ++i)
#define ST first
#define ND second
#define PB push_back
#define MP make_pair
#define deb(a) a

using namespace boost;
using namespace std;

typedef adjacency_list<vecS,
                       vecS,
                       undirectedS,
                       no_property,
                       property<edge_weight_t, int> >
    GraphU;
typedef adjacency_list<vecS,
                       vecS,
                       directedS,
                       no_property,
                       property<edge_weight_t, int> >
    GraphD;
typedef graph_traits<GraphD>::edge_descriptor EdgeD;
typedef graph_traits<GraphD>::vertex_descriptor VertexD;
typedef graph_traits<GraphU>::edge_descriptor EdgeU;
typedef graph_traits<GraphU>::vertex_descriptor VertexU;
typedef property_map<GraphD, edge_weight_t>::type WeightMapD;

typedef pair<int, int> PII;
typedef vector<PII> VPII;
typedef vector<int> VI;
typedef long long int LL;

const int A = 103;

int agent_pos[A], shelters_pos[A];
int n, m, agents, shelters, cap, enter_time;

// Graph and weights of edges for slopes and lifts (as read on the input).
GraphD G;
WeightMapD weightmap;

// Checks whether within given time and extra time enter_time, all agents can
// enter the shelters.
bool check(int t) {
    // We will simulate capacity of the shelter by increasing number of vertices
    // corresponding to shelters.
    int shelters_total = cap * shelters;

    // Bipartite graph corresponding to possibilites of agents hiding in
    // shelters (in the given time).
    GraphU conn(agents + shelters_total);

    REP(i, agents) {
        // Calculating distances of shelters from given agent.
        VI distmap(n);
        VertexD start = agent_pos[i];
        dijkstra_shortest_paths(G, start,
                                distance_map(make_iterator_property_map(
                                    distmap.begin(), get(vertex_index, G))));

        REP(j, shelters) {
            // Iterating over shelters simulating capacity.
            REP(k, cap) {
                // Agents queue when entering the shelter. Thefore, when
                // entering shelter with higher capacity some agents must arrive
                // appropriately earlier.
                if (distmap[shelters_pos[j]] <= t - (k + 1) * enter_time) {
                    EdgeU e;
                    bool success;
                    tie(e, success) =
                        add_edge(i, agents + k * shelters + j, conn);
                }
            }
        }
    }

    // Find maximum matching in such constructed graph.
    vector<VertexU> mate(agents + shelters_total);
    edmonds_maximum_cardinality_matching(conn, &mate[0]);

    return matching_size(conn, &mate[0]) == agents;
}

void do_test() {
    cin >> n >> m >> agents >> shelters >> cap >> enter_time;
    G = GraphD(n);
    weightmap = get(edge_weight, G);

    int sum_edges = 0;
    REP(i, m) {
        int time;
        char type;
        VertexD v, w;
        cin >> type >> v >> w >> time;

        EdgeD e;
        bool success;
        tie(e, success) = add_edge(v, w, G);
        weightmap[e] = time;
        sum_edges += time;

        if (type == 'L') {
            // Ski lift - add edge also in the reverse direction.
            tie(e, success) = add_edge(w, v, G);
            weightmap[e] = time;
        }
    }

    REP(i, agents) { cin >> agent_pos[i]; }

    REP(i, shelters) { cin >> shelters_pos[i]; }

    // Binary search - searching for minimal time which allows all agents to
    // enter the shelters. To found time we also need to add enter time, which
    // corresponds to last agent (maybe out of 2) entering the shelter.
    int bstart = 0, bend = sum_edges + 2 * enter_time;
    while (bstart != bend) {
        int p = (bstart + bend) / 2;
        if (check(p)) {
            bend = p;
        } else {
            bstart = p + 1;
        }
    }

    cout << bend << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
