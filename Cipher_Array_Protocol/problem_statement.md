# Problem Statement.md

## Cipher Array Protocol

In the year 2147, secret communication networks rely on numeric arrays called **Cipher Arrays** to secure messages. A Cipher Array \( a \) of length \( n \) is generated based on a command string \( s \) of length \( n \) consisting only of characters `'A'`, `'B'`, and `'C'`. Each character imposes specific constraints on the values in the array \( a \).

Your task is to determine the total number of valid Cipher Arrays modulo \( 998244353 \) that satisfy the following conditions for each position \( i \) (1-indexed):

### Character 'A' Constraint:
- The value \( a[i] \) must be equal to the count of distinct integers that appear an **odd number of times** in the prefix subarray \( a[1..i-1] \).
- Additionally, when inserting new distinct integers, each integer can be classified into one of three categories:
  - **Prime integers** (e.g., 2, 3, 5, 7, ...)
  - **Composite integers** (e.g., 4, 6, 8, 9, ...)
  - **Neither** (specifically the integer 1)
- The prefix must track how many distinct primes and composites have been inserted so far.

### Character 'B' Constraint:
- The value \( a[i] \) must be equal to the count of distinct integers that appear an **even number of times** in the suffix subarray \( a[i+1..n] \).
- The suffix must similarly track the count of prime and composite distinct integers.

### Character 'C' Constraint:
- The value \( a[i] \) must be equal to \((X + Y) \bmod 3\), where:
  - \( X \) is the count of distinct prime integers in the prefix subarray \( a[1..i-1] \).
  - \( Y \) is the count of distinct composite integers in the suffix subarray \( a[i+1..n] \).

---

### Additional Details:

- The array \( a \) consists of positive integers, but the exact values are **not fixed**. Instead, integers are introduced dynamically while respecting the constraints on primes, composites, and the integer 1.
- You must count the number of distinct arrays \( a \) that satisfy the above conditions for **all** \( i \) from 1 to \( n \).
- Since the answer can be very large, output it modulo \( 998244353 \).

---

## Input Format:

- The first line contains a single integer \( t \) — the number of test cases.
- For each test case:
  - The first line contains an integer \( n \) — the length of the command string.
  - The second line contains the string \( s \) of length \( n \), consisting only of characters `'A'`, `'B'`, and `'C'`.

---

## Output Format:

For each test case, output a single integer — the total number of valid Cipher Arrays modulo \( 998244353 \).

---

## Constraints:

- \( 1 \leq t \leq 10 \)
- \( 1 \leq n \leq 50 \)
- \( s \) consists only of characters `'A'`, `'B'`, and `'C'`

---

## Sample Input:
3
3
AAA
4
ABCA
5
BCCAB

## Sample Output:

36
24624
433702512


---

## Explanation:

- For the first test case `"AAA"`, the count of valid arrays is **36**.
- For the second test case `"ABCA"`, the count is **24624**.
- For the third test case `"BCCAB"`, the count is **433702512**.

These outputs reflect the number of ways to assign integers to the array positions, respecting the prime/composite/1 classification and parity constraints specified by each character in the command string.
