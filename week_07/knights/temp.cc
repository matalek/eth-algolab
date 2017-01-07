

// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
// Namespaces
using namespace std;
using namespace boost;

#define REP(i, n) for (int i = 0; i < (n); ++i)
#define ST first
#define ND second
#define PB push_back
#define MP make_pair
#define deb(a)

typedef pair<int, int> PII;
typedef vector < PII > VPII;
typedef vector<int> VI;
typedef long long int LL;

// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS, no_property,
    property<edge_capacity_t, long,
        property<edge_residual_capacity_t, long,
            property<edge_reverse_t, Traits::edge_descriptor> > > > Graph;
// Interior Property Maps
typedef property_map<Graph, edge_capacity_t>::type      EdgeCapacityMap;
typedef property_map<Graph, edge_residual_capacity_t>::type ResidualCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type       ReverseEdgeMap;
typedef graph_traits<Graph>::vertex_descriptor          Vertex;
typedef graph_traits<Graph>::edge_descriptor            Edge;
typedef graph_traits<Graph>::edge_iterator edge_iter;


// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class EdgeAdder {
    Graph &G;
    EdgeCapacityMap &capacitymap;
    ReverseEdgeMap  &revedgemap;

public:
    // to initialize the Object
    EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
        G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

    // to use the Function (add an edge)
    void addEdge(int from, int to, long capacity) {
        Edge e, reverseE;
        bool success;
        tie(e, success) = add_edge(from, to, G);
        tie(reverseE, success) = add_edge(to, from, G);
        capacitymap[e] = capacity;
        capacitymap[reverseE] = 0;
        revedgemap[e] = reverseE;
        revedgemap[reverseE] = e;
        // deb(cout << from << " " << to << " " << capacity << "\n";)
        cout << from << " " << to << endl;
    }
};

int dir1[] = {-1, 0, 1, 0};
int dir2[] = { 0, 1, 0, -1};

int n, m;

int point_number(int x, int y, bool in) {
    int res = y * n + x;
    res =  2 * res + !in;
    return res;
}

void do_test() {
    int k, c;
    cin >> n >> m >> k >> c;
    if (n == 0 || m == 0) {
        cout << 0 << "\n";
        return;
    }
    Graph G(m * n * 2 + 2);
    EdgeCapacityMap capacitymap = get(edge_capacity, G);
    ReverseEdgeMap revedgemap = get(edge_reverse, G);
    ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, revedgemap);

    const bool in = true;
    const bool out = false;

    int src = m * n * 2;
    int sink = m * n * 2 + 1;

    REP(i, n) {
        REP(j, m) {
            eaG.addEdge(point_number(i, j, in), point_number(i, j, out), c);
            REP(k, 4) {
                int x = i + dir1[k];
                int y = j + dir2[k];
                if (x >= 0 && x < n && y >= 0 && y < m) {
                    eaG.addEdge(point_number(i, j, out), point_number(x, y, in), 1);
                    // eaG.addEdge(point_number(x, y, out), point_number(i, j, in), 1);
                }
            }
        }
    }



    for(int i = 1; i < n - 1; i++) {
        if (m != 1) {
            eaG.addEdge(point_number(i, 0, out), sink, 1);
            eaG.addEdge(point_number(i, m - 1, out), sink, 1);
        } else {
            eaG.addEdge(point_number(i, 0, out), sink, 2);
        }
    }

    for(int i = 1; i < m - 1; i++) {
        if (n != 1) {
            eaG.addEdge(point_number(0, i, out), sink, 1);
            eaG.addEdge(point_number(n - 1, i, out), sink, 1);
        }
        else {
            eaG.addEdge(point_number(0, i, out), sink, 2);
        }
    }

    if (n == 1 && m == 1) {
        eaG.addEdge(point_number(0, 0, out), sink, 4);
    } else if (n == 1) {
        eaG.addEdge(point_number(0, 0, out), sink, 3);
        eaG.addEdge(point_number(0, m - 1, out), sink, 3);
    } else if (m == 1) {
        eaG.addEdge(point_number(0, 0, out), sink, 3);
        eaG.addEdge(point_number(n - 1, 0, out), sink, 3);
    } else {
        eaG.addEdge(point_number(0, 0, out), sink, 2);
        eaG.addEdge(point_number(n - 1, 0, out), sink, 2);
        eaG.addEdge(point_number(0, m - 1, out), sink, 2);
        eaG.addEdge(point_number(n - 1, m - 1, out), sink, 2);
    }


    REP(i, k) {
        int x, y;
        cin >> x >> y;
        eaG.addEdge(src, point_number(x, y, in), 1);
    }


    long flow = push_relabel_max_flow(G, src, sink);

    cout << flow << "\n";

    edge_iter ei, ei_end;
    deb(for (tie(ei, ei_end) = edges(G); ei != ei_end; ++ei)
      std::cout << source(*ei, G) << " " << target(*ei, G) << " " <<  rescapacitymap[*ei] << endl;)
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
