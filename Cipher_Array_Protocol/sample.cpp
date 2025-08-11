#include <iostream>
#include<vector>

using namespace std;

const int MOD = 998244353;

// Check prime
bool isPrime(int x) {
    if (x < 2) return false;
    for (int i = 2; i*i <= x; i++) {
        if (x % i == 0) return false;
    }
    return true;
}

// Check composite
bool isComposite(int x) {
    return x > 1 && !isPrime(x);
}


vector<vector<vector<vector<vector<int>>>>> computePrefixDPWithPrimeComposite(const string& s, int n) {
    // dp[i][oddCount][D][primeCount][compositeCount]
    vector<vector<vector<vector<vector<int>>>>> dp(
        n+1,
        vector<vector<vector<vector<int>>>>(n+1,
            vector<vector<vector<int>>>(n+1,
                vector<vector<int>>(n+1,
                    vector<int>(n+1, 0)))));
    
    dp[0][0][0][0][0] = 1;

    for (int i = 0; i < n; i++) {
        for (int oddCount = 0; oddCount <= i; oddCount++) {
            for (int D = oddCount; D <= i; D++) {
                for (int primeCount = 0; primeCount <= D; primeCount++) {
                    for (int compositeCount = 0; compositeCount <= D - primeCount; compositeCount++) {
                        int ways = dp[i][oddCount][D][primeCount][compositeCount];
                        if (ways == 0) continue;

                        // Handle 'A' condition exactly as before for oddCount and D
                        if (s[i] == 'A') {
                            // Add new integer - try three cases:
                            // 1. new integer is prime
                            if (D + 1 <= n && primeCount + 1 <= n)
                                dp[i+1][oddCount+1][D+1][primeCount+1][compositeCount] =
                                    (dp[i+1][oddCount+1][D+1][primeCount+1][compositeCount] + ways) % MOD;

                            // 2. new integer is composite
                            if (D + 1 <= n && compositeCount + 1 <= n)
                                dp[i+1][oddCount+1][D+1][primeCount][compositeCount+1] =
                                    (dp[i+1][oddCount+1][D+1][primeCount][compositeCount+1] + ways) % MOD;

                            // 3. new integer is neither (e.g., 1)
                            if (D + 1 <= n)
                                dp[i+1][oddCount+1][D+1][primeCount][compositeCount] =
                                    (dp[i+1][oddCount+1][D+1][primeCount][compositeCount] + ways) % MOD;

                            // Add existing odd-frequency integer
                            if (oddCount > 0)
                                dp[i+1][oddCount-1][D][primeCount][compositeCount] =
                                    (dp[i+1][oddCount-1][D][primeCount][compositeCount] + 1LL * ways * oddCount) % MOD;

                            // Add existing even-frequency integer
                            if (D - oddCount > 0)
                                dp[i+1][oddCount+1][D][primeCount][compositeCount] =
                                    (dp[i+1][oddCount+1][D][primeCount][compositeCount] + 1LL * ways * (D - oddCount)) % MOD;

                        } else {
                            // For 'B' and 'C' at prefix stage, propagate all transitions (simplified)
                            // Add new integer - three cases for prime, composite, neither

                            // prime new integer
                            if (D + 1 <= n && primeCount + 1 <= n)
                                dp[i+1][oddCount+1][D+1][primeCount+1][compositeCount] =
                                    (dp[i+1][oddCount+1][D+1][primeCount+1][compositeCount] + ways) % MOD;

                            // composite new integer
                            if (D + 1 <= n && compositeCount + 1 <= n)
                                dp[i+1][oddCount+1][D+1][primeCount][compositeCount+1] =
                                    (dp[i+1][oddCount+1][D+1][primeCount][compositeCount+1] + ways) % MOD;

                            // neither prime nor composite
                            if (D + 1 <= n)
                                dp[i+1][oddCount+1][D+1][primeCount][compositeCount] =
                                    (dp[i+1][oddCount+1][D+1][primeCount][compositeCount] + ways) % MOD;

                            // existing odd-frequency integer
                            if (oddCount > 0)
                                dp[i+1][oddCount-1][D][primeCount][compositeCount] =
                                    (dp[i+1][oddCount-1][D][primeCount][compositeCount] + 1LL * ways * oddCount) % MOD;

                            // existing even-frequency integer
                            if (D - oddCount > 0)
                                dp[i+1][oddCount+1][D][primeCount][compositeCount] =
                                    (dp[i+1][oddCount+1][D][primeCount][compositeCount] + 1LL * ways * (D - oddCount)) % MOD;
                        }
                    }
                }
            }
        }
    }
    return dp;
}


vector<vector<vector<vector<vector<int>>>>> computeSuffixDPWithPC(const string &s, int n) {
    // dp[i][evenCount][D][primeCount][compositeCount] = number of ways
    // i = position in suffix (1-based)
    // evenCount = how many distinct integers have even frequency so far in suffix starting at i
    // D = total distinct integers so far in suffix starting at i
    // primeCount = how many of those distinct integers are prime
    // compositeCount = how many of those distinct integers are composite
    const int MAX = n + 2;
    vector<vector<vector<vector<vector<int>>>>> dp(
        MAX,
        vector<vector<vector<vector<int>>>>(MAX,
            vector<vector<vector<int>>>(MAX,
                vector<vector<int>>(MAX,
                    vector<int>(MAX, 0)))));
    
    dp[n + 1][0][0][0][0] = 1; // empty suffix after position n

    // Helper lambdas for prime checking — can be precomputed outside for efficiency
    auto isPrime = [](int x) {
        if (x < 2) return false;
        for (int i = 2; i * i <= x; i++) {
            if (x % i == 0) return false;
        }
        return true;
    };

    for (int i = n; i >= 1; i--) {
        for (int evenCount = 0; evenCount <= n; evenCount++) {
            for (int D = evenCount; D <= n; D++) {
                for (int primeCount = 0; primeCount <= D; primeCount++) {
                    for (int compositeCount = 0; compositeCount <= D - primeCount; compositeCount++) {
                        int ways = dp[i + 1][evenCount][D][primeCount][compositeCount];
                        if (ways == 0) continue;

                        if (s[i - 1] == 'B') {
                            // a[i] must equal evenCount at this position.

                            // Add new integer - must update primeCount or compositeCount accordingly

                            // Try prime new integer:
                            if (D + 1 <= n && primeCount + 1 <= n) {
                                dp[i][evenCount + 1][D + 1][primeCount + 1][compositeCount] =
                                    (dp[i][evenCount + 1][D + 1][primeCount + 1][compositeCount] + ways) % MOD;
                            }

                            // Try composite new integer:
                            if (D + 1 <= n && compositeCount + 1 <= n) {
                                dp[i][evenCount + 1][D + 1][primeCount][compositeCount + 1] =
                                    (dp[i][evenCount + 1][D + 1][primeCount][compositeCount + 1] + ways) % MOD;
                            }

                            // Try neither prime nor composite (e.g. 1):
                            if (D + 1 <= n) {
                                dp[i][evenCount + 1][D + 1][primeCount][compositeCount] =
                                    (dp[i][evenCount + 1][D + 1][primeCount][compositeCount] + ways) % MOD;
                            }

                            // Add existing even-frequency integer (will become odd frequency):
                            if (evenCount > 0) {
                                dp[i][evenCount - 1][D][primeCount][compositeCount] =
                                    (dp[i][evenCount - 1][D][primeCount][compositeCount] + 1LL * ways * evenCount) % MOD;
                            }

                            // Add existing odd-frequency integer (will become even frequency):
                            if (D - evenCount > 0) {
                                dp[i][evenCount + 1][D][primeCount][compositeCount] =
                                    (dp[i][evenCount + 1][D][primeCount][compositeCount] + 1LL * ways * (D - evenCount)) % MOD;
                            }

                        } else {
                            // For other chars (A, C), just propagate transitions similarly:

                            // New integer: try prime
                            if (D + 1 <= n && primeCount + 1 <= n) {
                                dp[i][evenCount + 1][D + 1][primeCount + 1][compositeCount] =
                                    (dp[i][evenCount + 1][D + 1][primeCount + 1][compositeCount] + ways) % MOD;
                            }

                            // New integer: try composite
                            if (D + 1 <= n && compositeCount + 1 <= n) {
                                dp[i][evenCount + 1][D + 1][primeCount][compositeCount + 1] =
                                    (dp[i][evenCount + 1][D + 1][primeCount][compositeCount + 1] + ways) % MOD;
                            }

                            // New integer: neither prime nor composite
                            if (D + 1 <= n) {
                                dp[i][evenCount + 1][D + 1][primeCount][compositeCount] =
                                    (dp[i][evenCount + 1][D + 1][primeCount][compositeCount] + ways) % MOD;
                            }

                            // Existing even-frequency integer -> odd-frequency
                            if (evenCount > 0) {
                                dp[i][evenCount - 1][D][primeCount][compositeCount] =
                                    (dp[i][evenCount - 1][D][primeCount][compositeCount] + 1LL * ways * evenCount) % MOD;
                            }

                            // Existing odd-frequency integer -> even-frequency
                            if (D - evenCount > 0) {
                                dp[i][evenCount + 1][D][primeCount][compositeCount] =
                                    (dp[i][evenCount + 1][D][primeCount][compositeCount] + 1LL * ways * (D - evenCount)) % MOD;
                            }
                        }
                    }
                }
            }
        }
    }
    return dp;
}


int countValidWaysForCCondition(int i,
    const vector<vector<vector<vector<vector<int>>>>>& dpPrefix,
    const vector<vector<vector<vector<vector<int>>>>>& dpSuffix,
    const string& s,
    int n) {

    const int MOD = 998244353;
    long long waysForPos = 0;

    // We don't know a[i] explicitly; instead,
    // a[i] can be any value from 0 to 2 (since mod 3).
    // The DP counts arrays a that satisfy the rules,
    // so we sum over all combinations (prefix,suffix) where
    // (primeCount_p + compositeCount_s) % 3 == a[i].
    // But a[i] itself is a[i], so effectively, all such pairs count.

    // Since the problem asks total number of valid arrays,
    // we just sum over all (primeCount_p, compositeCount_s) where
    // (primeCount_p + compositeCount_s) % 3 == a[i],
    // but a[i] is the value that must equal (X + Y) % 3.
    // We want the total sum over all possible a[i] that satisfy the rule.

    // For each prefix state at i-1
    for (int oddCount = 0; oddCount <= i - 1; oddCount++) {
        for (int D_p = oddCount; D_p <= i - 1; D_p++) {
            for (int primeCount_p = 0; primeCount_p <= D_p; primeCount_p++) {
                for (int compositeCount_p = 0; compositeCount_p <= D_p - primeCount_p; compositeCount_p++) {
                    int prefixWays = dpPrefix[i - 1][oddCount][D_p][primeCount_p][compositeCount_p];
                    if (prefixWays == 0) continue;

                    // For each suffix state at i+1
                    for (int evenCount = 0; evenCount <= n - i; evenCount++) {
                        for (int D_s = evenCount; D_s <= n - i; D_s++) {
                            for (int primeCount_s = 0; primeCount_s <= D_s; primeCount_s++) {
                                for (int compositeCount_s = 0; compositeCount_s <= D_s - primeCount_s; compositeCount_s++) {
                                    int suffixWays = dpSuffix[i + 1][evenCount][D_s][primeCount_s][compositeCount_s];
                                    if (suffixWays == 0) continue;

                                    // Check condition (X + Y) % 3 == a[i]
                                    // a[i] = (primeCount_p + compositeCount_s) % 3
                                    // Since a[i] must equal that value for validity, count ways

                                    // Here, a[i] = (primeCount_p + compositeCount_s) % 3

                                    // The number of ways to build arrays with a[i] equal to that value
                                    // is product of prefixWays * suffixWays

                                    // So add to total
                                    int val = (primeCount_p + compositeCount_s) % 3;

                                    // We want to count all valid arrays satisfying the condition,
                                    // so simply sum over these ways
                                    waysForPos = (waysForPos + (long long)prefixWays * suffixWays) % MOD;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return (int)waysForPos;
}



int countTotalValidArrays(const string& s, int n,
                          const vector<vector<vector<vector<vector<int>>>>> dpPrefix,
                          const vector<vector<vector<vector<vector<int>>>>> dpSuffix) {
    const int MOD = 998244353;
    long long totalWays = 1; // product over positions

    for (int i = 1; i <= n; i++) {
        char c = s[i-1];
        long long waysForPos = 0;

        if (c == 'A') {
            // For 'A' at position i:
            // sum all prefix dp states at i-1 where a[i] == oddCount in prefix DP

            for (int oddCount = 0; oddCount <= i-1; oddCount++) {
                for (int D = oddCount; D <= i-1; D++) {
                    for (int primeCount = 0; primeCount <= D; primeCount++) {
                        for (int compositeCount = 0; compositeCount <= D - primeCount; compositeCount++) {
                            int ways = dpPrefix[i-1][oddCount][D][primeCount][compositeCount];
                            if (ways == 0) continue;

                            // The value a[i] must be equal to oddCount, so count these ways
                            waysForPos = (waysForPos + ways) % MOD;
                        }
                    }
                }
            }

        } else if (c == 'B') {
            // For 'B' at position i:
            // sum all suffix dp states at i+1 where a[i] == evenCount in suffix DP

            for (int evenCount = 0; evenCount <= n - i; evenCount++) {
                for (int D = evenCount; D <= n - i; D++) {
                    for (int primeCount = 0; primeCount <= D; primeCount++) {
                        for (int compositeCount = 0; compositeCount <= D - primeCount; compositeCount++) {
                            int ways = dpSuffix[i+1][evenCount][D][primeCount][compositeCount];
                            if (ways == 0) continue;

                            // The value a[i] must be equal to evenCount, so count these ways
                            waysForPos = (waysForPos + ways) % MOD;
                        }
                    }
                }
            }

        } else if (c == 'C') {
            // For 'C', use the special function combining prefix and suffix DP states
            // waysForPos = countValidWaysForCCondition(i, dpPrefix, dpSuffix, s, n);
            waysForPos = countValidWaysForCCondition(i, dpPrefix, dpSuffix, s, n);
        }

        // If no ways satisfy condition at this position, answer is zero
        if (waysForPos == 0) return 0;

        // Multiply the ways for this position
        totalWays = (totalWays * waysForPos) % MOD;
    }

    return totalWays;
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t; cin >> t;
    while (t--) {
        int n; string s; cin >> n >> s;

        // Max distinct integers <= n
        int maxD = n;

        auto dpPrefix = computePrefixDPWithPrimeComposite(s, n);
        auto dpSuffix = computeSuffixDPWithPC(s, n);

        // int answer = combineResults(dpPrefix, dpSuffix, s);
        // cout << answer << "\n";
        
        int answer = countTotalValidArrays(s, n, dpPrefix, dpSuffix);
        cout << answer << "\n";


       
    }
    return 0;
}
