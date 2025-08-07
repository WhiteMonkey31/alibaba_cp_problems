# F1. Top-K Tracker (Easy Version)

**Time Limit per Test:** 3 seconds  
**Memory Limit per Test:** 256 megabytes  

**This is an interactive problem.**

> This is the easy version of the problem. The only difference is that `n ≤ 845` in this version.

---

## Problem Description

There is a hidden permutation `p`, which is a permutation of `{1, 2, …, n}`. Let `pos_i` denote the position of the value `i` in `p`, i.e., `pos[p_i] = i`.

To find this permutation, you can ask **four types of queries**:

### Query Types

1. **Query Type 1**:
    ```
    1 m i1 i2 … im
    ```
    - (1 ≤ m ≤ n, i1, i2, …, im must be distinct)
    - The jury will return the top `k = min(60, m)` largest numbers in `[p_i1, p_i2, …, p_im]` in **increasing order**.

2. **Query Type 2**:
    ```
    2 m i1 i2 … im
    ```
    - Returns the top `k = min(60, m)` largest numbers in `[pos_i1, pos_i2, …, pos_im]` in **increasing order**.

3. **Query Type 3**:
    ```
    3 m i1 i2 … im
    ```
    - Returns the top `k = min(300, m)` largest numbers in `[p_i1, p_i2, …, p_im]` in **increasing order**.

4. **Query Type 4**:
    ```
    4 m i1 i2 … im
    ```
    - Returns the top `k = min(300, m)` largest numbers in `[pos_i1, pos_i2, …, pos_im]` in **increasing order**.

### Query Constraints

Let `c_i` be the usage count of queries of type `i` (1 ≤ i ≤ 4). Your query count must satisfy the following constraints:

- `c1 + c2 + c3 + c4 ≤ 30`
- `c3 + c4 ≤ 1`

---

## Input

Each test contains multiple test cases.  
- The first line contains a single integer `t` (1 ≤ t ≤ 40) — the number of test cases.

For each test case:
- The first line contains an integer `n` (2 ≤ n ≤ 845). At this moment, the permutation `p` is chosen.

> The interactor in this task is not adaptive. In other words, the permutation `p` is fixed in every test case and does not change during the interaction.

---

## Interaction

To ask a query of type `x`, print:

```
x m i1 i2 … im
```

After that, you receive `k = min(60, m)` or `min(300, m)` integers — the top `k` largest number(s) as described above.

To answer the test case with your guess of permutation `p`, print:

```
! p1 p2 … pn
```

This **does not count** as a query.

---

### Important:

- After printing a query or answer, flush the output:
    - C++: `cout.flush()`
    - Java: `System.out.flush()`
    - Python: `stdout.flush()`

---

## Hacks

To hack, follow the format:

```
t
n
p1 p2 … pn
```

---

## Example

**Input:**
```
2
3

2 3

3

2

2
```

**Output:**
```
3 2 3 1
2 1 2
! 3 1 2
4 1 1
! 2 1
```

**Explanation:**

In the first test case, the hidden permutation is `p = [3, 1, 2]`, so `pos = [2, 3, 1]`.

- Query: `3 2 3 1` returns `2 3` (top elements of [p3, p1])
- Query: `2 1 2` returns `3` (top element of [pos2])

> Note: This example is illustrative and may not uniquely determine the permutation.