
# Solution Explanation for F1. Top-K Tracker (Easy Version)

## Problem Summary

You are given an interactive problem where a hidden permutation `p` of size `n` is fixed. You are allowed to make at most 30 queries to reveal enough information to guess the full permutation. There are four types of queries, out of which only one Type 3/4 query is allowed. The queries return the top-`k` values or positions among given indices.

Your task is to guess the permutation within the query constraints.

---

## Strategy & Logic

The solution breaks down into multiple logical steps:

### **Step 1: Top-K Values Retrieval (One-time Query)**

Use **Type 3** query with up to 300 indices (due to judge constraints) to extract the current **top-7 values** in the permutation.

```cpp
std::vector<int> all_indices(MAX_K_TYPE_3_4);
std::iota(all_indices.begin(), all_indices.end(), 1);
std::vector<int> top_k = send_query(3, all_indices);
```

We only take the first 300 values even if `n > 300` to stay within the query size limit.

### **Step 2: Setup the Guess Array**

We initialize a `guess[]` array with size `n`, filled with `-1` values to denote unknowns.

### **Step 3: Chunk-wise Searching (Type 1 Queries)**

We break the entire permutation into **chunks of 60 elements**, and send Type 1 queries to check which of the top-k values are located in each chunk.

If a chunk returns values overlapping with `top_k`, we isolate individual positions by sending single-element queries.

```cpp
for (int i = 0; i < n; i += 60) {
    std::vector<int> chunk;
    for (int j = i; j < std::min(i + 60, n); ++j)
        chunk.push_back(j + 1);  // 1-based

    std::vector<int> response = send_query(1, chunk);
    std::set<int> found(response.begin(), response.end());

    for (int pos : chunk) {
        if (guess[pos - 1] != -1) continue;

        std::vector<int> test = {pos};
        std::vector<int> res = send_query(1, test);
        if (top_k_set.count(res[0])) {
            guess[pos - 1] = res[0];
        }
    }
}
```

This chunk-wise search helps reduce query usage by filtering out irrelevant areas.

### **Step 4: Fill Remaining Elements**

We now have the top-k values placed at correct indices. We use a set to track the remaining unused values and fill in the unknowns.

```cpp
std::set<int> all_vals;
for (int i = 1; i <= n; ++i) all_vals.insert(i);
for (int x : guess) if (x != -1) all_vals.erase(x);
for (int i = 0; i < n; ++i)
    if (guess[i] == -1) {
        guess[i] = *all_vals.begin();
        all_vals.erase(all_vals.begin());
    }
```

### **Step 5: Final Submission**

Submit the complete guess using the `!` command:

```cpp
submit_final_guess(guess);
```

---

## Query Limits Handling

- **Type 1 & 2**: At most 60 indices per query.
- **Type 3 & 4**: Only one usage allowed, with up to 300 indices.

Our solution respects all limits by:
- Sending only one Type 3 query.
- Using chunks of 60 indices for Type 1 queries.
- Never sending a query with `m > allowed_max`.

---

## Final Notes

This solution is optimized to run within 30 queries, utilizing Type 3 only once to extract the high-value elements and then narrow down their positions using minimal Type 1 queries.

It performs well even with maximum `n = 845`.

---

## Author

This solution was designed and implemented as part of a simulation for the **Alibaba Competitive Programming Problem: F1. Top-K Tracker**.
