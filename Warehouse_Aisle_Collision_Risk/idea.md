# idea.md — Warehouse Aisle Collision Risk

## Problem summary

We have two groups of robots moving along two parallel aisles (type 0 and type 1). Each robot's 1D position along its aisle follows:

$P(t) = p + v\cdot t + \tfrac{1}{2} a \cdot t^2$

Given a time window $[0, T]$ and collision threshold $D = 24$ cm, count the number of distinct pairs (one robot from aisle 0, one from aisle 1) for which there exists some $t \in [0, T]$ such that the absolute difference of their positions is $\le D$.

This file describes the idea, reasoning, and algorithmic steps to solve the problem exactly and efficiently (for typical CP constraints where $N+M$ is up to a few thousands).

---

## Key observations

1. **Relative motion reduces to one function.**
   For a pair of robots with parameters $(p_1,v_1,a_1)$ and $(p_2,v_2,a_2)$, define relative quantities:
   $\Delta p = p_1 - p_2,\quad \Delta v = v_1 - v_2,\quad \Delta a = a_1 - a_2.$
   The relative displacement function is
   $f(t) = \Delta p + \Delta v\,t + \tfrac{1}{2} \Delta a\,t^2.$
   A collision occurs iff there exists $t\in[0,T]$ with $|f(t)| \le D$.

2. **Nature of f(t):**

   * If $\Delta a = 0$ then $f(t)$ is linear (or constant if $\Delta v=0$).
   * If $\Delta a \ne 0$ then $f(t)$ is a quadratic (parabola). The minimum absolute value may occur at endpoints or at the parabola's critical point.

3. **Exact checking via roots.**
   Instead of sampling times, we compute *exact* times when $f(t) = D$ and when $f(t) = -D$. Each is a quadratic equation in general. Those real roots partition the time line into intervals where the sign/inequality of $|f(t)| - D$ is constant. If any such interval (or root point) intersects $[0,T]$ and satisfies $|f(t)| \le D$, the pair collides.

4. **Advantages of this approach:**

   * Deterministic and exact (up to floating-point precision). No timestep errors.
   * Complexity per pair is constant-time arithmetic and small-degree root solving.

---

## Algorithm (per pair)

1. Compute $\Delta p,\Delta v,\Delta a$.
2. If both robots have identical motion ($\Delta a = 0$ and $\Delta v = 0$) then they collide iff $|\Delta p| \le D$ (for any t).
3. Otherwise, form the two equations:

   * $f(t) - D = 0$ → quadratic in t
   * $f(t) + D = 0$ → quadratic in t
     Solve each for real roots using robust quadratic solver (handle near-zero a coefficient to fallback to linear).
4. Collect all real roots from both equations, add the endpoints 0 and T, sort unique.
5. Check each root that lies in $[0,T]$ — if $|f(root)| \le D$ (allow tiny EPS) return true.
6. For every open interval between consecutive roots, if the interval overlaps $[0,T]$ sample its midpoint and test $|f(mid)| \le D$. If any satisfy, return true.
7. If none pass, return false.

Repeat for every pair (i in aisle 0, j in aisle 1) and count collisions.

---

## Complexity

* Pair checks: $O(N\cdot M)$ or $O((N+M)^2)$ if you check every unordered pair and filter by aisle type. For typical CP constraints where $N+M \le 4000$, the worst-case \~8M pair checks is feasible in C++ (each pair is small constant work).
* Per pair: constant-time (solve up to two quadratics, evaluate f() a handful of times).

Memory: $O(N+M)$ to store robot data.

---

## Numerical considerations

* Use `long double` for improved precision on root computations and comparisons.
* Use a small EPS (e.g., `1e-12`..`1e-14`) when comparing reals to zero or comparing distances to D.
* Quadratic solver should handle near-zero leading coefficient (treat as linear) and near-zero discriminant (double root).

---

## Edge cases

* Robots already within distance D at t=0.
* Robots that are identically moving: constant relative displacement.
* Linear relative motion ($\Delta a = 0$) with closest approach in interior of $[0,T]$.
* Quadratic relative motion where the parabola vertex lies outside $[0,T]$ but endpoints overlap.
* Tangent cases where f(t)=±D exactly at a single instant (should count as collision).

---

## Test case design (recommended)

1. **No motion**: many robots stationary far apart → answer 0.
2. **Immediate collisions**: pairs already within D at t=0 → count them.
3. **Endpoint collision**: pairs that only reach boundary at t=T.
4. **Mid-interval collision**: acceleration-driven meet at fractional t inside (0,T).
5. **Tangent collision**: f(t)=D exactly (double root) to test discriminant handling.
6. **Large N test**: stress with N+M ≈ 3000 and random parameters to validate runtime.

For each test, provide both small and large variants and include expected answers.

---

## Implementation notes

* Provide the solver as a single-pass pairwise algorithm. Use a helper `willCollide(dy, dv, da, D, T)` that returns boolean.
* Avoid printing debug output in final judge submission. Keep I/O minimal.
* Optionally allow D to be input to make problem more general, otherwise hardcode 24 in the statement.

---

## Final remark

This idea transforms a potentially tricky timestep-simulation problem into a clean analytical geometry/kinematics check. It rewards contestants who think about continuous-time motion and robust root-solving rather than naive sampling.
