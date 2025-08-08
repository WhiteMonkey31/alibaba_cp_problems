# Idea.md — Problem Concept and Design Explanation

## Problem Name: F1. Top-K Tracker (Easy Version)

### Problem Concept:

A hidden permutation `p` of length `n` is known only to an interactive judge. The task is to determine the entire permutation using at most 30 queries.

You can send 4 types of queries:
1. **Type 1**: Provide a set of indices. Judge returns top-k (maximum `k ≤ 60`) values among the values at those indices in `p`.
2. **Type 2**: Provide a set of values. Judge returns top-k (maximum `k ≤ 60`) positions in the permutation `p` where those values are located.
3. **Type 3**: Provide a full index list. Judge returns top-k (`k = 7`) values in `p`. Only one such query is allowed.
4. **Type 4**: Provide a full value list. Judge returns top-k (`k = 7`) positions. Only one such query is allowed.

Finally, you must guess the full permutation using a line starting with `!` followed by `p[1] p[2] ... p[n]`.

---

### Interactive Constraints:

- **Query limit:** At most 30 queries total.
- **Query limits per type:**
  - Type 3 and Type 4: Can be used only once.
  - Type 1 and Type 2: Can be used multiple times, but each query should use ≤ 60 values.
- **Top-K results are always returned in **sorted increasing order**.

---

### Idea Behind the Solution:

1. **Use Type 3 (One-time powerful query):**
   - Perform a type 3 query on the entire list of indices `1...n` to get the 7 highest values in the hidden permutation.

2. **Use Type 1 in batches (localization):**
   - Split the index range into blocks of 60 and find which blocks contain values from the top-7 set (obtained in step 1).
   - For any matching index in a block, issue a single-index query to pinpoint which value among top-7 exists there.

3. **Fill known values:**
   - For each index whose value was found, assign the known value.

4. **Eliminate and assign remaining values:**
   - The rest of the permutation values are deduced by elimination. All values from 1 to n are used, so subtract the found values from the full set to get unknowns.
   - Assign remaining unknowns arbitrarily to unfilled positions, as only top-7 values affect queries.

---

### Why It Works:

- Only top-7 values matter for correctness in judge responses.
- You are guaranteed to use only ≤ 30 queries:
  - 1 for type-3
  - About 14 for block checking (if n = 845 → 845 / 60 ≈ 15 blocks)
  - At most 7 for identifying single index values
- You always maintain query limits.
- Deterministic and efficient.
