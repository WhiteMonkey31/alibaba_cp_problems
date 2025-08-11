# solution.md — Warehouse Aisle Collision Risk

## Short idea

For each pair of robots (one from Aisle 0 and one from Aisle 1) compute the relative motion

$f(t) = \Delta p + \Delta v\,t + \tfrac{1}{2} \Delta a\,t^2.$

We need to determine whether there exists any real $t\in[0,T]$ such that $|f(t)| \le D$ (where $D=24$).

The robust solution is to solve exactly the equations `f(t) - D = 0` and `f(t) + D = 0` (each quadratic or linear), collect all real roots, add the endpoints `0` and `T`, and then test both root points and interior points of intervals between consecutive roots that overlap `[0,T]`. If any test point satisfies `|f(t)| <= D`, the pair collides.

This approach handles constant, linear and quadratic relative motions and treats tangency and endpoints correctly.

---

## Complexity

* Pair checks: $O(N\cdot M)$ (or $O((N+M)^2)$ if you scan all unordered pairs and skip same-aisle pairs).
* Per pair: constant-time root-solving (at most two quadratics) and interval checks.

Memory: $O(N+M)$.

---

## C++ Implementation (exact, robust)

```cpp
#include <bits/stdc++.h>
using namespace std;

struct Robot { int type; long double pos, vel, acc; };
const long double EPS = 1e-14L;

vector<long double> solve_quadratic(long double a, long double b, long double c) {
    vector<long double> roots;
    if (fabsl(a) < EPS) {
        if (fabsl(b) < EPS) return roots;
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
    long double A = 0.5L * da;
    vector<long double> roots;

    // f(t) - D = 0
    {
        auto r = solve_quadratic(A, dv, dy - D);
        roots.insert(roots.end(), r.begin(), r.end());
    }
    // f(t) + D = 0
    {
        auto r = solve_quadratic(A, dv, dy + D);
        roots.insert(roots.end(), r.begin(), r.end());
    }

    roots.push_back(0.0L);
    roots.push_back(T);

    sort(roots.begin(), roots.end());
    vector<long double> uniq;
    for (auto x : roots) if (uniq.empty() || fabsl(x - uniq.back()) > 1e-12L) uniq.push_back(x);
    roots.swap(uniq);

    auto f = [&](long double t){ return dy + dv*t + A*t*t; };

    // Check exact roots in [0,T]
    for (auto t : roots) {
        if (t + 1e-16L < 0.0L || t - 1e-16L > T) continue;
        if (fabsl(f(t)) <= D + 1e-12L) return true;
    }

    // Check intervals between roots
    for (size_t k = 0; k + 1 < roots.size(); ++k) {
        long double L = roots[k];
        long double R = roots[k+1];
        long double oL = max(L, (long double)0.0L);
        long double oR = min(R, T);
        if (oL + 1e-16L >= oR) continue;
        long double mid = (oL + oR) * 0.5L;
        if (fabsl(f(mid)) <= D + 1e-12L) return true;
    }
    return false;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M; long double T;
    if (!(cin >> N >> M >> T)) return 0;
    vector<Robot> robots(N+M);
    for (int i = 0; i < N+M; ++i) cin >> robots[i].type >> robots[i].pos >> robots[i].vel >> robots[i].acc;

    const long double D = 24.0L;
    long long ans = 0;
    for (int i = 0; i < N+M; ++i) {
        for (int j = i+1; j < N+M; ++j) {
            if (robots[i].type == robots[j].type) continue;
            long double dy = robots[i].pos - robots[j].pos;
            long double dv = robots[i].vel - robots[j].vel;
            long double da = robots[i].acc - robots[j].acc;
            if (willCollide(dy, dv, da, D, T)) ++ans;
        }
    }
    cout << ans << '\n';
}
```

---

## Implementation notes

* Use `long double` for increased numeric robustness.
* EPS values guard against tiny numerical noise; tune if necessary for your judge.
* If the problem asks to read the threshold `D` from input, replace the constant.

---

## Testing suggestions

* Unit-test `solve_quadratic` for linear, double-root, and no-root cases.
* Include cases where the collision occurs exactly at `t=0`, exactly at `t=T`, and mid-interval.
* Stress test with random inputs for performance (N+M up to 3k–4k).

---

## Final remarks

This solution is deterministic, exact (within floating precision) and contest-ready for the constraints specified in the statement.
