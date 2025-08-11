#include <iostream>
#include<vector>

using namespace std;

const int MOD = 998244353;

bool isPrime(int x) {
    if (x < 2) return false;
    for (int i = 2; i*i <= x; i++) {
        if (x % i == 0) return false;
    }
    return true;
}

bool isComposite(int x) {
    return x > 1 && !isPrime(x);
}


vector<vector<vector<vector<vector<int>>>>> computePrefixDPWithPrimeComposite(const string& s, int n) {
    
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

                        
                        if (s[i] == 'A') {
                            
                            if (D + 1 <= n && primeCount + 1 <= n)
                                dp[i+1][oddCount+1][D+1][primeCount+1][compositeCount] =
                                    (dp[i+1][oddCount+1][D+1][primeCount+1][compositeCount] + ways) % MOD;

                            if (D + 1 <= n && compositeCount + 1 <= n)
                                dp[i+1][oddCount+1][D+1][primeCount][compositeCount+1] =
                                    (dp[i+1][oddCount+1][D+1][primeCount][compositeCount+1] + ways) % MOD;

                            if (D + 1 <= n)
                                dp[i+1][oddCount+1][D+1][primeCount][compositeCount] =
                                    (dp[i+1][oddCount+1][D+1][primeCount][compositeCount] + ways) % MOD;

                            if (oddCount > 0)
                                dp[i+1][oddCount-1][D][primeCount][compositeCount] =
                                    (dp[i+1][oddCount-1][D][primeCount][compositeCount] + 1LL * ways * oddCount) % MOD;

                            if (D - oddCount > 0)
                                dp[i+1][oddCount+1][D][primeCount][compositeCount] =
                                    (dp[i+1][oddCount+1][D][primeCount][compositeCount] + 1LL * ways * (D - oddCount)) % MOD;

                        } else {
                            
                            if (D + 1 <= n && primeCount + 1 <= n)
                                dp[i+1][oddCount+1][D+1][primeCount+1][compositeCount] =
                                    (dp[i+1][oddCount+1][D+1][primeCount+1][compositeCount] + ways) % MOD;

                            
                            if (D + 1 <= n && compositeCount + 1 <= n)
                                dp[i+1][oddCount+1][D+1][primeCount][compositeCount+1] =
                                    (dp[i+1][oddCount+1][D+1][primeCount][compositeCount+1] + ways) % MOD;

                           
                            if (D + 1 <= n)
                                dp[i+1][oddCount+1][D+1][primeCount][compositeCount] =
                                    (dp[i+1][oddCount+1][D+1][primeCount][compositeCount] + ways) % MOD;

                            
                            if (oddCount > 0)
                                dp[i+1][oddCount-1][D][primeCount][compositeCount] =
                                    (dp[i+1][oddCount-1][D][primeCount][compositeCount] + 1LL * ways * oddCount) % MOD;

                            
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

    const int MAX = n + 2;
    vector<vector<vector<vector<vector<int>>>>> dp(
        MAX,
        vector<vector<vector<vector<int>>>>(MAX,
            vector<vector<vector<int>>>(MAX,
                vector<vector<int>>(MAX,
                    vector<int>(MAX, 0)))));
    
    dp[n + 1][0][0][0][0] = 1; 

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
                            
                            if (D + 1 <= n && primeCount + 1 <= n) {
                                dp[i][evenCount + 1][D + 1][primeCount + 1][compositeCount] =
                                    (dp[i][evenCount + 1][D + 1][primeCount + 1][compositeCount] + ways) % MOD;
                            }

                            if (D + 1 <= n && compositeCount + 1 <= n) {
                                dp[i][evenCount + 1][D + 1][primeCount][compositeCount + 1] =
                                    (dp[i][evenCount + 1][D + 1][primeCount][compositeCount + 1] + ways) % MOD;
                            }

                            if (D + 1 <= n) {
                                dp[i][evenCount + 1][D + 1][primeCount][compositeCount] =
                                    (dp[i][evenCount + 1][D + 1][primeCount][compositeCount] + ways) % MOD;
                            }

                            if (evenCount > 0) {
                                dp[i][evenCount - 1][D][primeCount][compositeCount] =
                                    (dp[i][evenCount - 1][D][primeCount][compositeCount] + 1LL * ways * evenCount) % MOD;
                            }

                            if (D - evenCount > 0) {
                                dp[i][evenCount + 1][D][primeCount][compositeCount] =
                                    (dp[i][evenCount + 1][D][primeCount][compositeCount] + 1LL * ways * (D - evenCount)) % MOD;
                            }

                        } else {
                            
                            if (D + 1 <= n && primeCount + 1 <= n) {
                                dp[i][evenCount + 1][D + 1][primeCount + 1][compositeCount] =
                                    (dp[i][evenCount + 1][D + 1][primeCount + 1][compositeCount] + ways) % MOD;
                            }

                            if (D + 1 <= n && compositeCount + 1 <= n) {
                                dp[i][evenCount + 1][D + 1][primeCount][compositeCount + 1] =
                                    (dp[i][evenCount + 1][D + 1][primeCount][compositeCount + 1] + ways) % MOD;
                            }

                            if (D + 1 <= n) {
                                dp[i][evenCount + 1][D + 1][primeCount][compositeCount] =
                                    (dp[i][evenCount + 1][D + 1][primeCount][compositeCount] + ways) % MOD;
                            }

                            if (evenCount > 0) {
                                dp[i][evenCount - 1][D][primeCount][compositeCount] =
                                    (dp[i][evenCount - 1][D][primeCount][compositeCount] + 1LL * ways * evenCount) % MOD;
                            }

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

   
    for (int oddCount = 0; oddCount <= i - 1; oddCount++) {
        for (int D_p = oddCount; D_p <= i - 1; D_p++) {
            for (int primeCount_p = 0; primeCount_p <= D_p; primeCount_p++) {
                for (int compositeCount_p = 0; compositeCount_p <= D_p - primeCount_p; compositeCount_p++) {
                    int prefixWays = dpPrefix[i - 1][oddCount][D_p][primeCount_p][compositeCount_p];
                    if (prefixWays == 0) continue;

                    for (int evenCount = 0; evenCount <= n - i; evenCount++) {
                        for (int D_s = evenCount; D_s <= n - i; D_s++) {
                            for (int primeCount_s = 0; primeCount_s <= D_s; primeCount_s++) {
                                for (int compositeCount_s = 0; compositeCount_s <= D_s - primeCount_s; compositeCount_s++) {
                                    int suffixWays = dpSuffix[i + 1][evenCount][D_s][primeCount_s][compositeCount_s];
                                    if (suffixWays == 0) continue;

                                    int val = (primeCount_p + compositeCount_s) % 3;

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
    long long totalWays = 1; 

    for (int i = 1; i <= n; i++) {
        char c = s[i-1];
        long long waysForPos = 0;

        if (c == 'A') {

            for (int oddCount = 0; oddCount <= i-1; oddCount++) {
                for (int D = oddCount; D <= i-1; D++) {
                    for (int primeCount = 0; primeCount <= D; primeCount++) {
                        for (int compositeCount = 0; compositeCount <= D - primeCount; compositeCount++) {
                            int ways = dpPrefix[i-1][oddCount][D][primeCount][compositeCount];
                            if (ways == 0) continue;

                            waysForPos = (waysForPos + ways) % MOD;
                        }
                    }
                }
            }

        } else if (c == 'B') {
            for (int evenCount = 0; evenCount <= n - i; evenCount++) {
                for (int D = evenCount; D <= n - i; D++) {
                    for (int primeCount = 0; primeCount <= D; primeCount++) {
                        for (int compositeCount = 0; compositeCount <= D - primeCount; compositeCount++) {
                            int ways = dpSuffix[i+1][evenCount][D][primeCount][compositeCount];
                            if (ways == 0) continue;

                            waysForPos = (waysForPos + ways) % MOD;
                        }
                    }
                }
            }

        } else if (c == 'C') {
          
            waysForPos = countValidWaysForCCondition(i, dpPrefix, dpSuffix, s, n);
        }

        if (waysForPos == 0) return 0;

       
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

        int maxD = n;

        auto dpPrefix = computePrefixDPWithPrimeComposite(s, n);
        auto dpSuffix = computeSuffixDPWithPC(s, n);

        int answer = countTotalValidArrays(s, n, dpPrefix, dpSuffix);
        cout << answer << "\n";
    }
    return 0;
}
