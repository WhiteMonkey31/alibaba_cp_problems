# Problem Statement — Warehouse Aisle Collision Risk

In an automated warehouse, there are two parallel aisles (**Aisle 0** and **Aisle 1**) where robots move in centimeters from a fixed origin.

Each robot moves according to the following rules:

* At time **t = 0**, its position is **p** (in cm), velocity is **v** (in cm/s), and acceleration is **a** (in cm/s²).
* Its motion is strictly along the aisle (no turning).
* Position at time **t** is given by:

$P(t) = p + v \cdot t + \frac{1}{2} a \cdot t^2$

Two robots **collide** if, at any point in time **t** within the closed interval **\[0, T]**, their positions differ by at most **24 cm** and they are in **different aisles**.

Collisions are counted per pair — if two robots come within 24 cm more than once, it still counts as **1 collision for that pair**.

Your task: **Given all robots, compute the number of distinct robot pairs that collide at least once** in the time interval **\[0, T]**.

---

## Input Format

* First line: Three values —

  * **N** (number of robots in Aisle 0)
  * **M** (number of robots in Aisle 1)
  * **T** (duration in seconds to consider)
* Next **N + M** lines: Each line contains:

  * `type` (**0** for Aisle 0, **1** for Aisle 1)
  * **p** (initial position in cm)
  * **v** (velocity in cm/s)
  * **a** (acceleration in cm/s²)

---

## Output Format

One integer — the total number of distinct colliding pairs.

---

## Constraints

* $1 \leq N, M \leq 1000$
* $0 < T \leq 10^4$
* $-10^5 \leq p, v, a \leq 10^5$
* Positions, velocities, and accelerations may be negative (robots can move in either direction).
* Time and motion are **continuous**, not discrete.

---

## Example 1

**Input:**

```
2 2 10
0 0 2 0
0 100 0 0
1 10 0 0
1 200 -5 0
```

**Output:**

```
3
```
