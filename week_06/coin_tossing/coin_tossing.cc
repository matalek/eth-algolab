// Coin Tossing
// Week 6
// Aleksander Matusiak

#include <iostream>
#include <vector>
#include <algorithm>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;
using namespace boost;

#define REP(i, n) for (int i = 0; i < (n); ++i)
#define ST first
#define ND second
#define PB push_back
#define MP make_pair
#define deb(a)

typedef pair<int, int> PII;
typedef vector<PII> VPII;
typedef vector<int> VI;
typedef long long int LL;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<
    vecS,
    vecS,
    directedS,
    no_property,
    property<edge_capacity_t,
             long,
             property<edge_residual_capacity_t,
                      long,
                      property<edge_reverse_t, Traits::edge_descriptor> > > >
    Graph;

typedef property_map<Graph, edge_capacity_t>::type EdgeCapacityMap;
typedef property_map<Graph, edge_residual_capacity_t>::type ResidualCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type ReverseEdgeMap;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;

// Custom Edge Adder Class, that holds the references to the graph, capacity map
// and reverse edge map.
class EdgeAdder {
    Graph& G;
    EdgeCapacityMap& capacitymap;
    ReverseEdgeMap& revedgemap;

   public:
    EdgeAdder(Graph& G,
              EdgeCapacityMap& capacitymap,
              ReverseEdgeMap& revedgemap)
        : G(G), capacitymap(capacitymap), revedgemap(revedgemap) {}

    void addEdge(int from, int to, long capacity) {
        Edge e, reverseE;
        bool success;
        tie(e, success) = add_edge(from, to, G);
        tie(reverseE, success) = add_edge(to, from, G);
        capacitymap[e] = capacity;
        capacitymap[reverseE] = 0;
        revedgemap[e] = reverseE;
        revedgemap[reverseE] = e;
    }
};


void do_test() {
    int n, m;
    cin >> n >> m;

    int g_size = n + 2, src = g_size - 2, sink = g_size - 1;
    Graph G(g_size);  // vertices are players + source and sink; edges will
                     // correspond to the matches with an unknow result
    EdgeCapacityMap capacitymap = get(edge_capacity, G);
    ReverseEdgeMap revedgemap = get(edge_reverse, G);
    ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, revedgemap);

	VI to_score(n); // how many points must given player score in order to satisfy given data
	VI cnt_edges(n);
    REP(i, m) {
        int a, b, c;
        cin >> a >> b >> c;
        if (c == 1) {
            to_score[a]--;
        } else if (c == 2) {
            to_score[b]--;
        } else {  // c == 0
        	// Unknown result - add edges to the graph.
            eaG.addEdge(a, b, 1);
            eaG.addEdge(b, a, 1);
            cnt_edges[a]++;
            cnt_edges[b]++;
        }
    }

    bool res = true;
    // Correspond to desired flows when it comes to total number of won matches
    // and lost matches respectively.
    long desired_flow_1 = 0, desired_flow_2 = 0;

    REP(i, n) {
        int s;
        cin >> s;
        to_score[i] += s;
        if (to_score[i] > cnt_edges[i] || to_score[i] < 0) {
            // Player won't be able to achieve desired score even if he won all
            // unknown matches or he has already won too many matches.
            res = false;
        } else {
            // Edges from source to the player correspond to winning matches,
            // edges from player to the sink correspond to lost matches.
            eaG.addEdge(src, i, to_score[i]);
            desired_flow_1 += to_score[i];
            eaG.addEdge(i, sink, cnt_edges[i] - to_score[i]);
            desired_flow_2 += cnt_edges[i] - to_score[i];
        }
    }

    if (!res || desired_flow_1 != desired_flow_2) {
        // We already can't satisfy score for some player or the total number of
        // points to score is impossible.
        cout << "no\n";
        return;
    }

    // Each flow unit corresponds to one match. It's path src -> a -> b -> sink
    // corresponds to the match a with b, where a player won.
    long flow = push_relabel_max_flow(G, src, sink);

    if (flow == desired_flow_1) {
        cout << "yes\n";
    } else {
        cout << "no\n";
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
