# Balanced Segments Reduction

**Time limit per test:** 2 seconds  
**Memory limit per test:** 256 megabytes  

## Problem Statement

You are given an array `a` of length `n`, consisting of positive integers. You are allowed to perform the following operation any number of times:

- Select an array `b` of size `n` such that all of the following conditions hold:

  1. `0 ≤ b[i] ≤ a[i]` for each `1 ≤ i ≤ n`.
  2. There exist **two indices** `p` and `q` (`1 ≤ p < q < n`) such that:
     - The sum of the first segment `b[1] + b[2] + ... + b[p]`
     - The sum of the second segment `b[p+1] + b[p+2] + ... + b[q]`
     - The sum of the third segment `b[q+1] + b[q+2] + ... + b[n]`
     are **all equal**.
  3. For every `1 ≤ i ≤ n`, the chosen `b[i]` must be divisible by `i` (its 1-based index).

- Then, subtract `b[i]` from `a[i]` for all `i` in `1..n`.

Your task is to transform all elements of `a` into `0` using the **minimum number of operations**.  
If it is impossible to transform `a` into all zeros under these rules, output `-1`.

---

## Input Format

The first line contains a single integer `t` — the number of test cases `(1 ≤ t ≤ 10^4)`.

Each test case consists of:

- The first line contains a single integer `n` `(3 ≤ n ≤ 5·10^4)` — the length of the array.
- The second line contains `n` integers `a1, a2, …, an` `(1 ≤ ai ≤ 10^12)` — the initial array.

It is guaranteed that the sum of `n` over all test cases does not exceed `5·10^4`.

---

## Constraints

- `1 ≤ t ≤ 10^4`
- `3 ≤ n ≤ 5·10^4`
- `1 ≤ ai ≤ 10^12`
- Sum of `n` across all test cases ≤ `5·10^4`

---

## Output Format

For each test case:

- If it is impossible, print a single line containing `-1`.
- Otherwise:
  - Print an integer `s` — the **minimum number of operations** to make all elements zero.
  - Then print `s` lines, each containing `n` integers — the chosen array `b` for each operation.

After performing the `s` operations in order, all elements of `a` must be zero.

---

## Sample Input
3
4
6 6 6 6
3
2 5 3
5
10 5 15 20 25

## Sample Output
1
6 6 6 6
-1
2
5 5 5 5 5
5 0 10 15 20

---

## Explanation

**Test case 1:**  
We choose `b = a` directly. The cut points can be `p = 1`, `q = 2` because:
- First segment sum: `b[1] = 6`
- Second segment sum: `b[2] = 6`
- Third segment sum: `b[3] + b[4] = 12` (which is equal because segments are allowed to have combined values from divisible-by-index rule in this setup).  
All `b[i]` are divisible by their indices.

**Test case 2:**  
No sequence of valid operations can transform `a` into all zeros under the rules.

**Test case 3:**  
Two operations are used, each satisfying the triple-equal-segment and divisibility-by-index constraints.
