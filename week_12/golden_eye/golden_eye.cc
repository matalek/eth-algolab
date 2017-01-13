// Golden Eye
// Week 12
// Aleksander Matusiak

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <deque>
#include <queue>
#include <map>
#include <climits>

#define REP(i, n) for (int i = 0; i < (n); ++i)
#define ST first
#define ND second
#define PB push_back
#define MP make_pair
#define deb(v_start)

using namespace std;

typedef pair<int, int> PII;
typedef vector<PII> VPII;
typedef vector<int> VI;
typedef long long int LL;

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
typedef Triangulation::Edge_iterator Edge_iterator;
typedef Triangulation::Point Point;
typedef Triangulation::Vertex_handle Vertex;

using namespace boost;
using namespace std;

#include <boost/pending/disjoint_sets.hpp>
typedef boost::disjoint_sets_with_storage<> Uf;

// Custom data structure to easily represent edges in triangulation, necessary
// later to perform Find&Union algorithm.
struct Edge {
    int v_start, v_end;
    LL length_sq;

    Edge(LL length_sq, int v_start, int v_end)
        : v_start(v_start), v_end(v_end), length_sq(length_sq) {}
};

inline bool operator<(const Edge& e, const Edge& f) {
    return e.length_sq < f.length_sq;
}

// Counts what is the maximum distance that has to be covered by jammers in
// order to connect appropriate jammers corresponding to missions which are to
// executed.
LL find_max_dist_connections(int n,
                             Triangulation& t,
                             vector<Edge>& edges,
                             vector<pair<Point, Point> >& missions,
                             vector<bool>& to_take) {
    Uf network(n);
    LL res = 0;
    auto it =
        edges.begin();  // we will start adding edges from the shortes ones

    REP(i, missions.size()) {
        auto m = missions[i];
        if (!to_take[i]) {
            continue;
        }
        auto handler_start = t.nearest_vertex(m.ST);
        auto handler_end = t.nearest_vertex(m.ND);

        // Add edges which has not yet been added till we connect jammers
        // corresponding to start and end of the mission.
        while (it != edges.end() && (network.find_set(handler_start->info()) !=
                                     network.find_set(handler_end->info()))) {
            res = max(res, it->length_sq);
            network.union_set(it->v_start, it->v_end);
            it++;
        }
    }
    return res;
}

void do_test() {
    int n, m;
    LL power;
    cin >> n >> m >> power;

    vector<pair<Point, int> > jammers(n);  // (position, index)
    vector<pair<Point, Point> > missions(m);

    REP(i, n) {
        cin >> jammers[i].ST;
        jammers[i].ND = i;
    }

    REP(i, m) { cin >> missions[i].ST >> missions[i].ND; }

    // Create triangulation for jammers, which contains Minimum Euclidian
    // Spanning Tree.
    Triangulation t;
    t.insert(jammers.begin(), jammers.end());

    // Convert triangulation to vector of custom edges, which will enable to
    // later easily use it.
    vector<Edge> edges;
    for (Edge_iterator ei = t.finite_edges_begin(); ei != t.finite_edges_end();
         ++ei) {
        Triangulation::Face& f = *(ei->first);
        int i = ei->second;
        Vertex v = f.vertex(f.cw(i));
        Vertex w = f.vertex(f.ccw(i));
        Point pv = v->point();
        Point pw = w->point();

        edges.PB(Edge(CGAL::squared_distance(pv, pw), v->info(), w->info()));
    }

    // Sort edges by length (squared).
    sort(edges.begin(), edges.end());

    Uf network(n);  // Find&Union structure representing network of jammers

    // Connect in network all jammers for which path between them is within
    // power of one of them. Then the agent can easily travel between them.
    for (Edge e : edges) {
        if (e.length_sq <= power) {
            network.union_set(e.v_start, e.v_end);
        }
    }

    // We use the observation that optimal path is firstly to get to the closest
    // jammer in the straight line, then travel between jammers using straight
    // segments between them until we reach jammer closest to the end point.
    // Finally, travel from this jammer to the end point in the straight line.

    int cnt = 0;        // counter for executed missions.
    LL max_dist_a = 0;  // maximum distance between agents and one of the
                        // jammers for optimal path and under assumption that
                        // all missions are executed (as in task 2)
    LL max_dist_b = 0;  // as above but under assumption that only initially
                        // executable missions are executed (as in task 3)
    vector<bool> executed(m);  // keeps whether given mission could be executed
                               // with initial power supply
    // Go over all missions to check whether they can be executed.
    REP(i, m) {
        auto mission = missions[i];
        auto handler_start = t.nearest_vertex(mission.ST);
        auto handler_end = t.nearest_vertex(mission.ND);

        // Calculate squares of double distances, to compute necessary power.
        LL dist_start =
            CGAL::squared_distance(handler_start->point(), mission.ST) * 4;
        LL dist_end =
            CGAL::squared_distance(handler_end->point(), mission.ND) * 4;

        // Check whether we can get from starting and ending points to the
        // nearest jammers and if these jammers are connected in our network.
        if (dist_start <= power && dist_end <= power &&
            (network.find_set(handler_start->info()) ==
             network.find_set(handler_end->info()))) {
            // It's possible to execute the mission under given requirements.
            cnt++;
            cout << "y";
            // Update maximum distance for task 3 to include the necessity to
            // get to the nearest jammer.
            max_dist_b = max(max_dist_b, dist_start);
            max_dist_b = max(max_dist_b, dist_end);
            executed[i] = true;
        } else {
            cout << "n";
            executed[i] = false;
        }
        // Update maximum distance for task 2 to include the necessity to get to
        // the nearest jammer.
        max_dist_a = max(max_dist_a, dist_start);
        max_dist_a = max(max_dist_a, dist_end);
    }
    cout << "\n";

    // We still need to update maximum distances for task 2 and 3 to include
    // creating connections between jammers.

    // Computing for task 2 - taking all the missions.
    vector<bool> to_take = vector<bool>(m, true);
    max_dist_a = max(max_dist_a,
                     find_max_dist_connections(n, t, edges, missions, to_take));
    cout << max_dist_a << "\n";

    // Computing for task 3 - taking only initially executable missions.
    max_dist_b = max(
        max_dist_b, find_max_dist_connections(n, t, edges, missions, executed));
    cout << max_dist_b << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
