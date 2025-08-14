Title:
D. Signal Nodes and the Rotating Path

Time limit per test: 2 seconds
Memory limit per test: 256 megabytes

You are given a connected, undirected, and loop-free network of n intersections and m bidirectional roads.

At the start (time = 0 seconds), a delivery drone is located at intersection 1.
The drone must reach intersection n by moving or waiting in discrete 1-second steps.

Each intersection u has its incident roads indexed from 1 to deg(u) in the order the roads were given in the input.

At every integer second t, if the drone is at intersection u, it must choose exactly one of the following actions:

Stay put for 1 second.

Travel for 1 second along the road whose index is:

((t + shift[u]) mod deg(u)) + 1


Here:

shift[u] is an integer given in the input for each intersection u.

The (t + shift[u]) value is not reset when waiting — time always moves forward globally.

Note: Waiting still increases the global time by 1.

The goal is to find:

The minimum total time to reach intersection n from intersection 1.

Among all ways to achieve that minimum total time, the minimum number of waiting seconds used.

Input format

Each test contains multiple test cases.

The first line contains an integer T — the number of test cases (1 ≤ T ≤ 1000).

For each test case:

The first line contains two integers n and m — number of intersections and number of roads.
(2 ≤ n ≤ 5000, n−1 ≤ m ≤ min( (n*(n−1))/2 , 5⋅10^5 ))

The second line contains n integers: shift[1], shift[2], …, shift[n]
(0 ≤ shift[i] < 10^9)

The next m lines each contain two integers u and v — describing a bidirectional road (1 ≤ u, v ≤ n, u ≠ v).

It is guaranteed the graph is connected and has no multiple roads between the same pair of intersections.
The sum of n over all test cases does not exceed 5000; the sum of m over all test cases does not exceed 5⋅10^5.

Output format

For each test case, output two integers separated by a space:

The minimum total time to reach intersection n.

The minimum possible total waiting time among all shortest-time solutions.

Example

Input

2
6 6
0 0 0 0 0 0
1 2
2 3
3 4
4 6
1 5
5 6
4 3
1 0 0 0
1 2
1 3
1 4


Output

4 2
3 0