// Sweepers
// Week 13
// Aleksander Matusiak

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <deque>
#include <queue>
#include <climits>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <boost/graph/connected_components.hpp>

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

// Simple undirected graph type for executing simple graph algorithms.
typedef adjacency_list<vecS, vecS, undirectedS> Graph;

// Directed graph type appropriate for executing flow algorithms on it.
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
    FlowGraph;
typedef property_map<FlowGraph, edge_capacity_t>::type EdgeCapacityMap;
typedef property_map<FlowGraph, edge_residual_capacity_t>::type
    ResidualCapacityMap;
typedef property_map<FlowGraph, edge_reverse_t>::type ReverseEdgeMap;
typedef graph_traits<FlowGraph>::vertex_descriptor Vertex;
typedef graph_traits<FlowGraph>::edge_descriptor Edge;

// Custom Edge Adder Class, that holds the references to the graph, capacity map
// and reverse edge map.
class EdgeAdder {
    FlowGraph& basic_g;
    EdgeCapacityMap& capacitymap;
    ReverseEdgeMap& revedgemap;

   public:
    EdgeAdder(FlowGraph& basic_g,
              EdgeCapacityMap& capacitymap,
              ReverseEdgeMap& revedgemap)
        : basic_g(basic_g), capacitymap(capacitymap), revedgemap(revedgemap) {}

    void addEdge(int from, int to, long capacity) {
        Edge e, reverseE;
        bool success;
        tie(e, success) = add_edge(from, to, basic_g);
        tie(reverseE, success) = add_edge(to, from, basic_g);
        capacitymap[e] = capacity;
        capacitymap[reverseE] = 0;
        revedgemap[e] = reverseE;
        revedgemap[reverseE] = e;
    }
};

void do_test() {
    int n, m, s;
    cin >> n >> m >> s;

    // Create graph for explicitly representing given rooms and corridors.
    Graph basic_g(n);

    // Create graph so that we can perform flow algorithm, which includes adding
    // additional source and sink vertices.
    int g_size = n + 2, src = g_size - 2, sink = g_size - 1;
    FlowGraph network_g(g_size);
    EdgeCapacityMap capacitymap = get(edge_capacity, network_g);
    ReverseEdgeMap revedgemap = get(edge_reverse, network_g);
    ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, network_g);
    EdgeAdder eaG(network_g, capacitymap, revedgemap);

    VI in(n), out(n);  // counts number of incoming and outgoing sweepers
                       // (sweepers available at the beginning and sweepers that
                       // have to be there at the end)
    VI degree(n);      // counts the degree of each vertex

    // Read initial positions of sweepers and add edges from source to them.
    REP(i, s) {
        int a;
        cin >> a;
        in[a]++;
        eaG.addEdge(src, a, 1);
    }

    // Read positions of exits and add edges from them to sink.
    REP(i, s) {
        int a;
        cin >> a;
        out[a]++;
        eaG.addEdge(a, sink, 1);
    }

    // Read corridors.
    REP(i, m) {
        int a, b;
        cin >> a >> b;
        degree[a]++;
        degree[b]++;

        // Add edge in our basic graph.
        add_edge(a, b, basic_g);

        // Add edges in both directions to the network graph.
        eaG.addEdge(a, b, 1);
        eaG.addEdge(b, a, 1);
    }

    // Check whether it's possible for all sweepers to reach some exit without
    // any corridor being used twice.
    int flow = push_relabel_max_flow(network_g, src, sink);
    if (flow != s) {
        cout << "no\n";
        return;
    }

    // If achieving maximal flow is possible we want to check whether it's
    // possible to create Euler's cycle, including additional edges
    // corresponding to initial sweepers positions and exits.

    // We can consider each component separately, so compute the components.
    std::vector<int> component(num_vertices(basic_g));
    int num = connected_components(basic_g, &component[0]);

    bool res = true;

    REP(i, n) {
        // Check if each vertex has even degree (including initial positions and
        // exits, which cna be treated as another edge)
        if ((degree[i] + in[i] + out[i]) % 2) {
            res = false;
            continue;
        }
    }

    // Check if for every component  with edges there is some sweeper to clean
    // it up.
    vector<bool> something(num);  // if there is some sweeper in the component
    REP(i, n) {
        if (in[i] || out[i]) {
            something[component[i]] = true;
        }
    }

    REP(i, n) {
        if (degree[i] > 0 && !something[component[i]]) {
            // The component containing i vertex has some edges but no sweepers
            // available, so they will be left uncleaned.
            res = false;
            break;
        }
    }

    // If all the conditions mentioned aboved are fulfilled, it is possible for
    // sweepers to sweep each corridor exactly once and get to the exits.
    if (res) {
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
