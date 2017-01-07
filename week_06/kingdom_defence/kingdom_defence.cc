// Kingdom Defence
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
    Graph G(g_size);  // vertices - cities, edges - paths between them
    EdgeCapacityMap capacitymap = get(edge_capacity, G);
    ReverseEdgeMap revedgemap = get(edge_reverse, G);
    ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, revedgemap);

    long desired_flow = 0;
    VI soldiers_in(n), soldiers_out(n);

    REP(i, n) {
        int a, b;
        cin >> soldiers_in[i] >> soldiers_out[i];
    }

    REP(i, m) {
        int a, b, c_min, c_max;
        cin >> a >> b >> c_min >> c_max;

        // We can simulate the minimum number of soldiers requirement by already
        // moving this amount of soldiers along this path.
        eaG.addEdge(a, b, c_max - c_min);
        soldiers_in[a] -= c_min;
        soldiers_in[b] += c_min;
    }

    REP(i, n) {
        if (soldiers_in[i] > 0) {
            // Soldiers available in this city are represented as an incoming
            // edge from the source.
            eaG.addEdge(src, i, soldiers_in[i]);
        } else {
            // Minimum number of soldiers requirement caused that we need to, in
            // fact, provide more soldiers to this point.
            eaG.addEdge(i, sink, -soldiers_in[i]);
            desired_flow += -soldiers_in[i];
        }
        // We need at the end given number of soldiers in the city - we model it
        // as outgoint edge to the sink.
        eaG.addEdge(i, sink, soldiers_out[i]);
        desired_flow += soldiers_out[i];
    }

    // Each flow unit corresponds to one soldier.
    long flow = push_relabel_max_flow(G, src, sink);

    // Checking if all cities had all the soldiers they needed.
    if (flow == desired_flow) {
        cout << "yes\n";
    } else {
        cout << "no\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
