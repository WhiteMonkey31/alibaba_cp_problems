// Test generator for "Cipher Array Protocol"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;
using namespace chrono;

// --------------------------------------------------
// I/O redirection helper
// --------------------------------------------------
class IORedirector {
    stringstream input, output;
    streambuf *old_cin, *old_cout;
public:
    IORedirector(const string& in) : input(in) {
        old_cin = cin.rdbuf(input.rdbuf());
        old_cout = cout.rdbuf(output.rdbuf());
    }
    ~IORedirector() {
        cin.rdbuf(old_cin);
        cout.rdbuf(old_cout);
    }
    string get_output() { return output.str(); }
};

// --------------------------------------------------
// Your solution (as "optimal") plugged into generator
// --------------------------------------------------
const int MOD = 998244353;

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

                        // Introduce a new distinct number: prime, composite, or 1
                        if (D + 1 <= n && primeCount + 1 <= n)
                            dp[i+1][oddCount+1][D+1][primeCount+1][compositeCount] =
                                (dp[i+1][oddCount+1][D+1][primeCount+1][compositeCount] + ways) % MOD;

                        if (D + 1 <= n && compositeCount + 1 <= n)
                            dp[i+1][oddCount+1][D+1][primeCount][compositeCount+1] =
                                (dp[i+1][oddCount+1][D+1][primeCount][compositeCount+1] + ways) % MOD;

                        if (D + 1 <= n)
                            dp[i+1][oddCount+1][D+1][primeCount][compositeCount] =
                                (dp[i+1][oddCount+1][D+1][primeCount][compositeCount] + ways) % MOD;

                        // Toggle an existing value's parity (odd->even or even->odd)
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

    for (int i = n; i >= 1; i--) {
        for (int evenCount = 0; evenCount <= n; evenCount++) {
            for (int D = evenCount; D <= n; D++) {
                for (int primeCount = 0; primeCount <= D; primeCount++) {
                    for (int compositeCount = 0; compositeCount <= D - primeCount; compositeCount++) {
                        int ways = dp[i + 1][evenCount][D][primeCount][compositeCount];
                        if (ways == 0) continue;

                        if (D + 1 <= n && primeCount + 1 <= n)
                            dp[i][evenCount + 1][D + 1][primeCount + 1][compositeCount] =
                                (dp[i][evenCount + 1][D + 1][primeCount + 1][compositeCount] + ways) % MOD;

                        if (D + 1 <= n && compositeCount + 1 <= n)
                            dp[i][evenCount + 1][D + 1][primeCount][compositeCount + 1] =
                                (dp[i][evenCount + 1][D + 1][primeCount][compositeCount + 1] + ways) % MOD;

                        if (D + 1 <= n)
                            dp[i][evenCount + 1][D + 1][primeCount][compositeCount] =
                                (dp[i][evenCount + 1][D + 1][primeCount][compositeCount] + ways) % MOD;

                        if (evenCount > 0)
                            dp[i][evenCount - 1][D][primeCount][compositeCount] =
                                (dp[i][evenCount - 1][D][primeCount][compositeCount] + 1LL * ways * evenCount) % MOD;

                        if (D - evenCount > 0)
                            dp[i][evenCount + 1][D][primeCount][compositeCount] =
                                (dp[i][evenCount + 1][D][primeCount][compositeCount] + 1LL * ways * (D - evenCount)) % MOD;
                    }
                }
            }
        }
    }
    return dp;
}

int countValidWaysForCCondition(
    int i,
    const vector<vector<vector<vector<vector<int>>>>>& dpPrefix,
    const vector<vector<vector<vector<vector<int>>>>>& dpSuffix,
    const string& s,
    int n) {

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

                                    // In your current logic, all prefix/suffix states are counted.
                                    // (If you later enforce (primeCount_p + compositeCount_s) % 3 match, do it here.)
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

int countTotalValidArrays(
    const string& s,
    int n,
    const vector<vector<vector<vector<vector<int>>>>>& dpPrefix,
    const vector<vector<vector<vector<vector<int>>>>>& dpSuffix) {

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
        } else { // 'C'
            waysForPos = countValidWaysForCCondition(i, dpPrefix, dpSuffix, s, n);
        }

        if (waysForPos == 0) return 0;
        totalWays = (totalWays * waysForPos) % MOD;
    }
    return (int)totalWays;
}

string solve_optimal(const string& input_str) {
    IORedirector redirector(input_str);

    int t; 
    if (!(cin >> t)) return redirector.get_output();
    while (t--) {
        int n; string s; 
        cin >> n >> s;

        auto dpPrefix = computePrefixDPWithPrimeComposite(s, n);
        auto dpSuffix = computeSuffixDPWithPC(s, n);
        int ans = countTotalValidArrays(s, n, dpPrefix, dpSuffix);
        cout << ans << "\n";
    }
    return redirector.get_output();
}

// --------------------------------------------------
// Generator utilities
// --------------------------------------------------
int find_next_file_number() {
    int next_num = 1;
    while (true) {
        string filename = "test_cases/" + to_string(next_num) + ".in";
        ifstream file(filename);
        if (!file.good()) return next_num;
        next_num++;
    }
}

void run_solution(const string& input_file, const string& output_file, int test_num, int file_num) {
    ifstream fin(input_file);
    stringstream buffer; buffer << fin.rdbuf(); fin.close();
    string result = solve_optimal(buffer.str());
    ofstream fout(output_file); fout << result; fout.close();
    cout << "Generated test " << test_num << " (file " << file_num << ")\n";
}

string random_s(int n) {
    static const char set[] = {'A','B','C'};
    string s; s.reserve(n);
    for (int i = 0; i < n; ++i) s.push_back(set[rand() % 3]);
    return s;
}

void generate_test_case(int test_num, int file_num) {
    // Keep n small to avoid huge memory/time due to 5D DP.
    // Adjust ranges if you want heavier tests, but be cautious.
    int t = rand() % 2 + 1; // 1..2 testcases per file
    string input_file = "test_cases/" + to_string(file_num) + ".in";
    ofstream fin(input_file);

    fin << t << "\n";
    for (int tc = 0; tc < t; ++tc) {
        int n = rand() % 8 + 1; // 1..8 (safe for the heavy DP)
        string s = random_s(n);
        fin << n << "\n" << s << "\n";
    }
    fin.close();

    string output_file = "test_cases/" + to_string(file_num) + ".out";
    run_solution(input_file, output_file, test_num, file_num);
}

int main() {
    srand((unsigned)time(0));
    system("mkdir -p test_cases");
// #ifdef _WIN32
//     system("if not exist test_cases mkdir test_cases");
// #else
//     system("mkdir -p test_cases");
// #endif

    int start_file_num = find_next_file_number();
    int num_tests = 50;

    cout << "Generating " << num_tests << " test cases starting from file number " << start_file_num << "\n";
    cout << "--------------------------------------------------------------------------------\n";
    for (int test_num = 1; test_num <= num_tests; ++test_num) {
        int file_num = start_file_num + test_num - 1;
        generate_test_case(test_num, file_num);
    }
    cout << "--------------------------------------------------------------------------------\n";
    cout << "Test generation complete! Generated " << num_tests << " test cases.\n";
    return 0;
}
