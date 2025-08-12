# Solution — Tournament Bias Spike Detection

## Approach

The task is to identify bias spikes by detecting when the bias value of a match exceeds all previous recorded biases in the sequence.

### Steps:
1. **Initialization**  
   - Keep track of the maximum bias value seen so far (`max_bias`), initialized to a very small number.
   - Maintain a counter for bias spikes.

2. **Iteration**  
   - For each match:
     - If the current bias is greater than `max_bias`:
       - Increment the spike counter.
       - Update `max_bias` to the current bias.

3. **Output**  
   - After processing all matches, output the count of detected bias spikes.

### Time Complexity
- **O(n)** — A single pass through the list of matches.

### Space Complexity
- **O(1)** — Only a few variables are used regardless of input size.

## Pseudocode

read n
read array bias[1..n]
spike_count = 0
max_bias = -infinity
for i = 1 to n:
if bias[i] > max_bias:
spike_count += 1
max_bias = bias[i]
print spike_count


## Example Walkthrough

### Example:
Input:


5
3 7 2 9 11

Processing:
- Start: `max_bias = -inf`, spikes = 0
- 3 > -inf → spike=1, max_bias=3
- 7 > 3 → spike=2, max_bias=7
- 2 ≤ 7 → no spike
- 9 > 7 → spike=3, max_bias=9
- 11 > 9 → spike=4, max_bias=11

Output:

4

