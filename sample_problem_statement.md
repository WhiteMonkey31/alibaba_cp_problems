# Quantum Drift: The Spaceman’s Journey

- **Time limit  :** 2 seconds  
- **Memory limit:** 512 MB



## Problem Statement

In a distant galaxy, a lone **spaceman** must journey from **Planet 1** to **Planet N**, navigating through a chaotic interstellar system.

Each planet has a unique **quantum frequency** $\( f_i \)$. Some planets are aligned via **navigable space currents**, while others are too volatile to traverse directly. These currents are **one-directional** and form a tightly bound, **fully connected network**.

The traveler can move in two distinct ways:

Space Current Drift

- The spaceman can move from planet \( u \) to planet \( v \) using a **space current**, but **only if there is a direct current** from \( u \) to \( v \).
- Each such movement **costs 1 unit** of energy.
- These currents form a **tree** of \( n - 1 \) one-way links, connecting all planets.

 Quantum Blink Teleportation

- The spaceman has access to a **Quantum Blink Module** for instant teleportation.
- He can teleport from **planet \( A \)** to **planet \( B \)** if and only if:
  $\[
  \gcd(f_A, f_B) > 1
  \]$
- He can teleport **at most \( k \) times**.
- The **\( i \)-th teleport** costs exactly \( i - 1 \) units of energy.
  - First blink: 0 energy
  - Second blink: 1 energy
  - Third blink: 2 energy
  - and so on...

 Mandatory Quantum Protocol

  Due to quantum instability near the destination, **at least one teleportation is mandatory** to survive the journey.

- If a path uses **only space currents**, it is considered invalid.
- If no teleportation is possible under the constraints, the journey is **impossible**.

Objective

Determine the **minimum total energy** required to travel from **Planet 1** to **Planet \( n \)** using any combination of space drifts and teleportation — but with **at least one teleportation used**.

If it's impossible to reach the destination under these rules, output `-1`.


## Input Format
```
n k
f1 f2 f3 ... fn
u1 v1
u2 v2
...
u_{n-1} v_{n-1}

```

**Constraints:**
- $2 \leq n \leq 10^5$ — number of planets  
- $1 \leq k \leq 10$ — maximum allowed teleportations  
- $1 \leq f_i \leq 10^6$ — frequency of each planet  
- Each of the next $(n - 1)$ lines contains:  
  - $u, v$ — one-way space current from planet $u$ to planet $v$  

The currents form a connected directed tree from planet $1$ to planet $n$.

## Output Format

A single integer — the minimum total energy required to reach Planet n from Planet 1,
using at least one teleportation.
If impossible, print -1.

## Sample Input

```
10 1
27 27 21 26 34 31 4 31 18 7 
1 2
1 3
1 4
2 5
1 6
3 7
4 8
6 9
4 10
```

## Sample Output
```
1
```

## Explanation
- Direct path via space currents from `1 → 4 → 10` would cost 2 (space drifts only), but this is invalid since no teleport is used.
- We can optimize with teleportation:
  - `1 → 10` is valid because GCD(27, 7) = 1 (not allowed) — so direct teleport is impossible.
  - `1` → 4 is valid because GCD(27, 26) = 1  (not allowed).
   
- Instead, we can teleport from `1 → 2` (GCD(27, 27) = 27 > 1) → First teleport cost = 0.
- Then from `2 → 5` using the space current costs 1.

- Total energy = **1 (space current) + 0 (teleport)** = **1**.

Valid, as at least one teleportation was used.
Minimum cost found = **1**.




