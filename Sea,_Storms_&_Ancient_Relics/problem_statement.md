# Sea, Storms & Ancient Relics

**Time Limit:** 3 seconds  
**Memory Limit:** 256 MB  

Far beneath the waves, Umi has discovered an ancient vault of relics.  
Each relic has a power value, represented by an integer `a[i]`.  
There are `n` relics, each with value between `1` and `m`.  

Umi wishes to form **two distinct duos of relics** `(p, q)` and `(r, s)` such that:

1. The two duos use **four distinct relics** (all indices different).  
2. Within each duo, the relics are **coprime**:  
   - `gcd(a[p], a[q]) = 1`  
   - `gcd(a[r], a[s]) = 1`  
3. Additionally, the **combined strength** of the four relics, defined as  
strength = (a[p] * a[q]) + (a[r] * a[s])
must **not be divisible by a given integer k**.

---

## Input
Each test contains multiple test cases.  

- The first line contains a single integer `t` (1 ≤ t ≤ 10^4) — the number of test cases.  
- Each test case begins with three integers `n, m, k`  
(4 ≤ n ≤ 2⋅10^5, 1 ≤ m ≤ 10^6, 2 ≤ k ≤ 1000).  
- The next line contains `n` integers `a1, a2, …, an` (1 ≤ ai ≤ m).  

It is guaranteed that the sum of `n` over all test cases does not exceed 2⋅10^5.  

---

## Output
For each test case:  
- If no such duos exist, output `0`.  
- Otherwise, output **four distinct indices** `p q r s`.  

If multiple answers exist, output any.

---

## Example

### Input
3
4 15 5
4 7 9 15
4 10 3
1 2 4 8
6 100 7
30 17 25 49 77 91

### Output
1 3 2 4
0
1 2 3 4

### Explanation
- In the first case: gcd(4,9)=1, gcd(7,15)=1, and strength = (4*9 + 7*15) = 36 + 105 = 141, which is not divisible by 5.  
- In the second case, it is impossible.  
- In the third case, gcd(17,25)=1 and gcd(49,91)=1, and total strength = (17*25 + 49*91) = 425 + 4459 = 4884, not divisible by 7.
