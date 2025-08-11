#include <iostream>
#include<vector>
#include<cmath>
#include<algorithm>

using namespace std;

struct Robot {
    int type;
    long double pos, vel, acc;
};

const long double EPS = 1e-14L;

vector<long double> solve_quadratic(long double a, long double b, long double c) {
    // solve a*x^2 + b*x + c = 0 for real roots, return vector (0..2) roots
    vector<long double> roots;
    if (fabsl(a) < EPS) {
        if (fabsl(b) < EPS) return roots; // constant, no roots (or infinite if c==0)
        roots.push_back(-c / b);
        return roots;
    }
    long double disc = b*b - 4*a*c;
    if (disc < -EPS) return roots;
    if (fabsl(disc) <= EPS) {
        roots.push_back(-b / (2*a));
    } else {
        long double sd = sqrt((long double)disc);
        roots.push_back((-b - sd) / (2*a));
        roots.push_back((-b + sd) / (2*a));
    }
    return roots;
}

bool willCollide(long double dy, long double dv, long double da, long double D, long double T) {
    // f(t) = dy + dv*t + 0.5*da*t^2
    // want |f(t)| <= D on some t in [0,T]
    // solve f(t) - D = 0 and f(t) + D = 0 (each quadratic), gather roots
    long double A = 0.5L * da;
    vector<long double> roots;

    // Solve f(t) - D = 0  => A t^2 + dv t + (dy - D) = 0
    {
        auto r = solve_quadratic(A, dv, dy - D);
        for (auto x : r) roots.push_back(x);
    }
    // Solve f(t) + D = 0  => A t^2 + dv t + (dy + D) = 0
    {
        auto r = solve_quadratic(A, dv, dy + D);
        for (auto x : r) roots.push_back(x);
    }

    // Always include interval boundaries beyond roots so we can sample intervals.
    // Add -INF and +INF aren't necessary; we'll sort roots and check intervals adjacent to [0,T].
    // Add 0 and T as candidates so we ensure endpoints are considered.
    roots.push_back(0.0L);
    roots.push_back(T);

    // Clean and sort
    sort(roots.begin(), roots.end());
    vector<long double> uniq;
    for (auto x : roots) {
        if (uniq.empty() || fabsl(x - uniq.back()) > 1e-12L) uniq.push_back(x);
    }
    roots.swap(uniq);

    // Build intervals between consecutive critical points, but also consider tiny neighborhoods around each root.
    // We'll check each interval (roots[k], roots[k+1]) and also check exact roots (in case equality at endpoint).
    auto f = [&](long double t)->long double {
        return dy + dv*t + A*t*t;
    };

    // 1) Check exact points at roots (if within [0,T]) — handles tangency and endpoint equalities.
    for (auto t : roots) {
        if (t + 1e-16L < 0.0L || t - 1e-16L > T) continue;
        if (fabsl(f(t)) <= D + 1e-12L) return true;
    }

    // 2) Check open intervals between consecutive roots. For each interval, if it overlaps [0,T], sample midpoint.
    for (size_t k = 0; k + 1 < roots.size(); ++k) {
        long double L = roots[k];
        long double R = roots[k+1];
        // interval (L, R)
        // Find overlap with (0, T)
        long double oL = max(L, (long double)0.0L);
        long double oR = min(R, T);
        if (oL + 1e-16L >= oR) continue; // no overlap (or just point which handled above)
        long double mid = (oL + oR) * 0.5L;
        if (fabsl(f(mid)) <= D + 1e-12L) return true;
    }

    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    long double T;
    if (!(cin >> N >> M >> T)) return 0;

    vector<Robot> robots;
    robots.reserve(N + M);

    for (int i = 0; i < N + M; ++i) {
        Robot r;
        cin >> r.type >> r.pos >> r.vel >> r.acc;
        robots.push_back(r);
    }

    const long double D = 24.0L;
    long long collisions = 0;

    for (int i = 0; i < (int)robots.size(); ++i) {
        for (int j = i + 1; j < (int)robots.size(); ++j) {
            if (robots[i].type == robots[j].type) continue;
            long double dy = robots[i].pos - robots[j].pos;
            long double dv = robots[i].vel - robots[j].vel;
            long double da = robots[i].acc - robots[j].acc;
            if (willCollide(dy, dv, da, D, T)) ++collisions;
        }
    }

    cout << collisions << '\n';
    return 0;
}
