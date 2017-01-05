// First hit
// Week 3
// Aleksander Matusiak

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <deque>
#include <random>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

#define REP(i, n) for (int i = 0; i < (n); ++i)
#define ST first
#define ND second
#define PB push_back
#define MP make_pair
#define deb(a) a

using namespace std;

typedef vector<pair<int, int> > VPII;
typedef vector<int> VI;
typedef long long int LL;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Ray_2 L;
typedef K::Segment_2 S;
typedef K::Point_2 P;

double floor_to_double(const K::FT& x) {
    double a = std::floor(CGAL::to_double(x));
    while (a > x)
        a -= 1;
    while (a + 1 <= x)
        a += 1;
    return a;
}

// Struct for representing laser beam. It is able to store a laser ray (as given
// as an input), as well as shortened segment - from starting point till the
// first obstacle.
struct Beam {
    L ray;  // original ray
    S seg;  // segment from start to the first obstacle
    P start;
    bool is_ray;  // whether we are storing ray (otherwise - segment)

    Beam(P start, P direction)
        : ray(start, direction), start(start), is_ray(true){};

    void set_end(P& end) {
        seg = S(start, end);
        is_ray = false;
    }
};

void do_test(int n) {
    LL a, b, c, d;
    cin >> a >> b >> c >> d;
    P start(a, b);
    Beam beam(start, P(c, d));

    LL arr[n][4];
    REP(i, n) {
        REP(j, 4) { cin >> arr[i][j]; }
    }

    // Shuffling input data so as to improve expected time complexity.
    auto engine = std::default_random_engine{};
    shuffle(arr, arr + n, engine);

    REP(i, n) {
        P seg_start(arr[i][0], arr[i][1]), seg_end(arr[i][2], arr[i][3]);
        S seg(seg_start, seg_end);

        // Checking if current segment intersects with current laser beam -
        // ray or segment.
        auto intersect = beam.is_ray ? CGAL::intersection(beam.ray, seg)
                                     : CGAL::intersection(beam.seg, seg);
        if (intersect) {
            auto o = beam.is_ray ? CGAL::intersection(beam.ray, seg)
                                 : CGAL::intersection(beam.seg, seg);

            P new_laser_end;
            if (const P* intersection_point = boost::get<P>(&*o)) {
                // One intersection point.
                new_laser_end = *intersection_point;
            } else if (const S* intersection_seg = boost::get<S>(&*o)) {
                // Intersection is a segment, we need to find the end of
                // the obstacle that is closer to the starting point of the
                // laser beam.
                if (S(start, seg_end).has_on(seg_start)) {
                    new_laser_end = seg_start;
                } else {
                    new_laser_end = seg_end;
                }
            }

            // Setting laser beam to be shorter and end at the encountered
            // obstacle.
            beam.set_end(new_laser_end);
        }
    }

    if (beam.is_ray) {
        // We haven't find any intersection.
        cout << "no\n";
    } else {
        cout << (LL)floor_to_double(beam.seg.target().x()) << " "
             << (LL)floor_to_double(beam.seg.target().y()) << "\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int n;
    while (true) {
        cin >> n;
        if (!n) {
            break;
        }
        do_test(n);
    }
    return 0;
}
