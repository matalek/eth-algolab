// Important bridges
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
#include <boost/graph/biconnected_components.hpp>

#define REP(i, n) for (int i = 0; i < (n); ++i)
#define ST first
#define ND second
#define PB push_back
#define MP make_pair
#define deb(a)

using namespace boost;
using namespace std;

namespace boost {
struct edge_component_t {
    enum { num = 555 };
    typedef edge_property_tag kind;
} edge_component;
}

typedef adjacency_list<vecS,
                       vecS,
                       undirectedS,
                       no_property,
                       property<edge_component_t, std::size_t> >
    Graph;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_iterator EdgeIt;
typedef graph_traits<Graph>::out_edge_iterator OutEdgeIt;
typedef property_map<Graph, edge_component_t>::type ComponentMap;

typedef vector<pair<int, int> > VPII;
typedef vector<int> VI;

void do_test() {
    int n, m;
    cin >> n >> m;
    Graph g(n);

    REP(i, m) {
        Vertex v, w;
        cin >> v >> w;
        Edge e;
        bool success;
        tie(e, success) = add_edge(v, w, g);
    }

    // Caclulating biconnected components and saving results in the map of
    // customly created properties for edges.
    ComponentMap component = get(edge_component, g);
    std::size_t num_comps = biconnected_components(g, component);

    VI cnt(num_comps);  // how many edges belong in each component
    EdgeIt ebeg, eend;
    for (tie(ebeg, eend) = edges(g); ebeg != eend; ++ebeg) {
        cnt[component[*ebeg]]++;
    }

    VPII results;
    for (tie(ebeg, eend) = edges(g); ebeg != eend; ++ebeg) {
        // Critical bridges are those in components with only one edge.
        if (cnt[component[*ebeg]] == 1) {
            Vertex u = source(*ebeg, g);
            Vertex v = target(*ebeg, g);
            results.PB(MP(min(u, v), max(u, v)));
        }
    }

    // Printing out the results in an appropriate format.
    sort(results.begin(), results.end());
    cout << results.size() << "\n";
    for (auto p : results) {
        cout << p.ST << " " << p.ND << "\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
