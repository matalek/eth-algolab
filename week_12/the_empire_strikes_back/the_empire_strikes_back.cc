// The Empire Strikes Back
// Week 12, Problem of the Week (test exam)
// Aleksander Matusiak

#include <iostream>
#include <vector>
#include <climits>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

using namespace std;

#define REP(i, n) for (int i = 0; i < (n); i++)
#define PB push_back

typedef vector<int> VI;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Point Point;

typedef long long int LL;

#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpzf.h>

typedef CGAL::Gmpzf ET;  // exact floating-point type
typedef CGAL::Quadratic_program<double> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void do_test() {
    int particles_cnt, shots_cnt, bounty_cnt, energy;
    cin >> particles_cnt >> shots_cnt >> bounty_cnt >> energy;

    vector<Point> particles(particles_cnt), shots(shots_cnt),
        bounty(bounty_cnt);
    VI densities(particles_cnt);
    REP(i, particles_cnt) { cin >> particles[i] >> densities[i]; }

    vector<LL> range_sq(
        shots_cnt);  // keeps computed optimal squared ranges for shots
    REP(i, shots_cnt) {
        cin >> shots[i];
        range_sq[i] = LLONG_MAX;
    }

    REP(i, bounty_cnt) { cin >> bounty[i]; }

    if (bounty_cnt > 0) {
        Triangulation t;
        t.insert(bounty.begin(), bounty.end());

        REP(i, shots_cnt) {
            // We would like the range of the shot to be as large as possible.
            // So we set it to the squared distance to the closest bounty hunter
            // (since our shot's range cannot include him).
            auto nearest_bounty = t.nearest_vertex(shots[i])->point();
            range_sq[i] = (LL)CGAL::squared_distance(shots[i], nearest_bounty);
        }
    }

    // Variables correspond to shots powers, equations - requirements for
    // destroying the particles. Each shot's power cannot be larger than
    // available energy
    Program lp(CGAL::LARGER, true, 0, true, energy);

    REP(i, particles_cnt) {
        // For each particle create an inequality corresponding to minimal power
        // required to destroy it.
        REP(j, shots_cnt) {
            LL d = CGAL::squared_distance(shots[j], particles[i]);
            if (d < range_sq[j]) {
                // Particle is within the range of the shot, so this shot
                // contributes to the destruction of this particle.
                LL pow = max((LL)1, d);
                lp.set_a(j, i, (double)1.0 / (double)pow);
            }
        }
        lp.set_b(i, densities[i]);
    }

    // Minimizing the sum of all shots energies.
    REP(j, shots_cnt) { lp.set_c(j, 1); }

    Solution sol = CGAL::solve_linear_program(lp, ET());

    // Checks whether the energy required to destroy all particles is not than
    // the energy we have.
    if ((sol.is_optimal()) && (sol.objective_value() <= energy)) {
        cout << "y\n";
    } else {
        cout << "n\n";
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    REP(i, T) { do_test(); }
    return 0;
}
