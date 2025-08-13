# Problem Statement — Warehouse Aisle Collision with Nonlinear Motion

In an automated warehouse, there are two parallel aisles (**Aisle 0** and **Aisle 1**) where robots move in centimeters from a fixed origin.

Each robot moves according to the following rules:

* At time **t = 0**, its position is **p** (in cm) and velocity is **v** (in cm/s).
* Its motion may include **piecewise constant accelerations** over multiple time segments.
* Additionally, each robot experiences a **small sinusoidal jitter** in its motion:

\[
P(t) = p + v \cdot t + \frac{1}{2} a(t) \cdot t^2 + A \cdot \sin(\omega t)
\]

where \(a(t)\) is the piecewise acceleration at time \(t\), \(A = 2.0\) cm, and \(\omega = 2.0\) rad/s.

Two robots **collide** if, at any point in time \(t\) within the closed interval **[0, T]**, their positions differ by at most **24 cm** and they are in **different aisles**.

Collisions are counted per robot pair — if two robots come within 24 cm more than once, it counts as **1 collision for that pair**.

Your task: **Given all robots, compute the number of distinct robot pairs that collide at least once** in the time interval **[0, T]**.

---

## Input Format

* First line: Three values —  
  * `N` (number of robots in Aisle 0)  
  * `M` (number of robots in Aisle 1)  
  * `T` (duration in seconds to consider)  

* Next `N + M` lines: Each line contains:  
  * `type` (**0** for Aisle 0, **1** for Aisle 1)  
  * `p` (initial position in cm)  
  * `v` (initial velocity in cm/s)  
  * `k` (number of acceleration segments)  
  * For each segment: two numbers  
    * `t_i` (start time of segment, in seconds)  
    * `a_i` (acceleration during this segment, in cm/s²)

---

## Output Format

One integer — the total number of distinct colliding pairs.

---

## Constraints

* \(1 \leq N, M \leq 1000\)  
* \(0 < T \leq 10^4\)  
* \(-10^5 \leq p, v, a_i \leq 10^5\)  
* Motion is **continuous in time**.  
* Memory Limit: **512 MB**  
* Time Limit: **6 seconds**  

---

## Sample Input

2 2 10
0 0 2 1 0 0
0 100 0 1 0 0
1 10 0 1 0 0
1 200 -5 1 0 0

## Sample Output

3

---

### Notes

* Each robot’s motion is **piecewise quadratic with sinusoidal jitter**.  
* Collisions should be counted **exactly once per pair**, regardless of how many times they come close.  
* This problem is intentionally designed to be difficult for naive solutions or LLM-generated code that does not consider **continuous motion with nonlinear components**.
