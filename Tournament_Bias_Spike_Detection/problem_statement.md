# Tournament Bias Spike Detection

**Time Limit:** 5 seconds  
**Memory Limit:** 512 megabytes  

## Problem Statement
You are analyzing scores from `n` games. Each score `a_i` is an integer.  
You must choose a continuous subarray `[l, r]` such that:

1. Its length is between `K` and `L` (inclusive).
2. It contains at least **three distinct scores**.

---

### Step 1 — Weighted Median
- Compute weights:  
  \[
  w_i = a_i \bmod P
  \]
- Sort the subarray by score (if scores are equal, smaller original index comes first).
- **Weighted median** is the element where the cumulative sum of weights first reaches or exceeds **half** of the total sum of weights.
- **Tie-break rule:** If two elements both qualify, choose the one with the **largest original index**.

---

### Step 2 — Third Smallest Distinct Element
- From the sorted list of scores in the subarray, pick the **third smallest distinct value**.
- If there are fewer than three distinct values, the subarray is **invalid**.

---

### Score of a Subarray
\[
\text{Score} = \text{(Weighted Median)} - \text{(Third Smallest Distinct Value)}
\]

Your task is to find the **maximum score** over all valid subarrays.

---

## Input Format
t
n K L P
a1 a2 ... an
(repeat for each test case)

---

## Output Format
For each test case, print the maximum possible score.  
If no valid subarray exists, output:

IMPOSSIBLE


---

## Constraints
- \( 1 \leq t \leq 2 \cdot 10^4 \)
- \( 1 \leq n \leq 3 \cdot 10^5 \)
- \( 1 \leq a_i \leq 10^9 \)
- \( 1 \leq K \leq L \leq n \)
- \( 1 \leq P \leq 10^7 \)
- Sum of all \( n \) over all test cases \(\leq 3 \cdot 10^5\)

---

## Example
**Input**
1
6 3 5 7
5 12 7 9 4 6


**Explanation**

One optimal subarray is:

[12, 7, 9, 4]

Sorted by score:

[4, 7, 9, 12]

Weights (mod 7):

[4, 0, 2, 5]

Total weight = 11  
Half = 5.5 → Weighted median = **9** (tie-breaking ensures 9 wins)  
Third smallest distinct = **9**  
Score = \( 9 - 9 = 0 \)  

Other subarrays may give higher scores.
