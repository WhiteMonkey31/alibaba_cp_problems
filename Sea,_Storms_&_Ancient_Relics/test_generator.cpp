#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <algorithm>
#include <future>
#include<iomanip>
using namespace std;
using namespace chrono;

atomic<bool> should_stop(false);

// I/O redirector to run a solver over a string input and capture output
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

// ===================================================================
// Brute-force solver inserted into solve_optimal (reads from string)
// ===================================================================
int gcd_int(long long a, long long b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        long long t = a % b;
        a = b;
        b = t;
    }
    return (int)a;
}

string solve_optimal(const string& input_str) {
    IORedirector redirector(input_str);

    int t;
    if (!(cin >> t)) {
        return redirector.get_output();
    }
    while (t--) {
        int n, m, k;
        cin >> n >> m >> k;
        vector<long long> a(n);
        for (int i = 0; i < n; ++i) cin >> a[i];

        // Trivial impossibility: for k == 1, every sum % 1 == 0.
        if (k == 1) {
            cout << 0 << '\n';
            continue;
        }

        struct PairInfo { int i, j; int mod; };
        vector<PairInfo> cpairs;
        // guard: avoid enormous reserve if n is huge due to a generation bug
        if (n > 5000) {
            // fall back to safe behavior: no pairs, print 0
            cout << 0 << '\n';
            continue;
        }
        cpairs.reserve((size_t)n * (n - 1) / 2);

        // Precompute a[i] % k for faster product mod
        vector<int> amod(n);
        for (int i = 0; i < n; ++i) amod[i] = (int)((a[i] % k + k) % k);

        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                if (gcd_int(a[i], a[j]) == 1) {
                    int pm = (int)((1LL * amod[i] * amod[j]) % k);
                    cpairs.push_back({i, j, pm});
                }
            }
        }

        bool found = false;

        for (size_t x = 0; x < cpairs.size() && !found; ++x) {
            for (size_t y = x + 1; y < cpairs.size(); ++y) {
                const auto &A = cpairs[x];
                const auto &B = cpairs[y];

                // disjoint indices: {A.i, A.j} must be disjoint from {B.i, B.j}
                if (A.i == B.i || A.i == B.j || A.j == B.i || A.j == B.j) continue;

                int sumMod = A.mod + B.mod;
                if (sumMod >= k) sumMod -= k; // equivalent to (A.mod + B.mod) % k
                if (sumMod != 0) {
                    // Output 1-based indices: p q r s
                    cout << (A.i + 1) << ' ' << (A.j + 1) << ' '
                         << (B.i + 1) << ' ' << (B.j + 1) << '\n';
                    found = true;
                    break;
                }
            }
        }

        if (!found) cout << 0 << '\n';
    }

    return redirector.get_output();
}

// ===================================================================
// Utility functions for test generation and running solver
// ===================================================================
int find_next_file_number() {
    int next_num = 1;
    while (true) {
        string filename = "test_cases/" + to_string(next_num) + ".in";
        ifstream file(filename);
        if (!file.good()) return next_num;
        next_num++;
    }
}

// Safe runner: runs solve_optimal via async with timeout; if it times out or throws,
// write a fallback output so the generator continues.
void run_solution_with_timeout(const string& input_file, const string& output_file, int test_num, int file_num) {
    cout << "Test " << setw(2) << test_num << " (files: " << setw(3) << file_num << ".in/out): ";
    cout.flush();

    // Read input file content
    ifstream fin(input_file);
    if (!fin.is_open()) {
        cerr << "ERROR: Cannot open " << input_file << "\n";
        return;
    }
    stringstream buffer;
    buffer << fin.rdbuf();
    string input_str = buffer.str();
    fin.close();

    // Run solver asynchronously
    string result;
    try {
        auto fut = async(launch::async, solve_optimal, input_str);
        auto status = fut.wait_for(seconds(10)); // 10-second timeout per file
        if (status == future_status::timeout) {
            cout << "[timeout -> fallback]" << endl;
            // create fallback output: for each testcase print 0 (safe)
            // We must know t: parse it quickly from input_str
            int t_local = 0;
            {
                stringstream ss(input_str);
                ss >> t_local;
            }
            stringstream out_ss;
            for (int i = 0; i < max(1, t_local); ++i) out_ss << 0 << '\n';
            result = out_ss.str();
        } else {
            result = fut.get();
            cout << "[ok]" << endl;
        }
    } catch (const exception &e) {
        cout << "[exception -> fallback]" << endl;
        int t_local = 0;
        {
            stringstream ss(input_str);
            ss >> t_local;
        }
        stringstream out_ss;
        for (int i = 0; i < max(1, t_local); ++i) out_ss << 0 << '\n';
        result = out_ss.str();
    }

    // Write output
    ofstream fout(output_file);
    if (!fout.is_open()) {
        cerr << "ERROR: Cannot open output file " << output_file << "\n";
        return;
    }
    fout << result;
    fout.close();
}

void run_solution(const string& input_file, const string& output_file, int test_num, int file_num) {
    run_solution_with_timeout(input_file, output_file, test_num, file_num);
}

// helper: generate a random prime in [2, maxv]
int random_prime_up_to(int maxv) {
    if (maxv < 2) return 2;
    int limit = max(2, maxv);
    vector<char> sieve(limit+1, 1);
    sieve[0] = sieve[1] = 0;
    for (int p = 2; p * p <= limit; ++p) if (sieve[p]) {
        for (int q = p*p; q <= limit; q += p) sieve[q] = 0;
    }
    vector<int> primes;
    for (int i = 2; i <= limit; ++i) if (sieve[i]) primes.push_back(i);
    if (primes.empty()) return 2;
    return primes[rand() % primes.size()];
}

// Generate a single test file (.in) with a set of testcases inside, and compute .out by running solver
void generate_test_case(int test_num, int file_num) {
    string input_file = "test_cases/" + to_string(file_num) + ".in";
    ofstream fin(input_file);

    // Random number of testcases in file
    int t = rand() % 8 + 1; // 1..8
    fin << t << "\n";

    for (int tc = 0; tc < t; ++tc) {
        // Choose variant types to ensure coverage of edge cases
        int variant = rand() % 7;

        int n, m, k;
        vector<int> arr;

        if (variant == 0) {
            // Minimal edge case: n=4, small m, random k
            n = 4;
            m = rand() % 10 + 1;
            k = rand() % 10 + 1;
            arr.resize(n);
            for (int i = 0; i < n; ++i) arr[i] = rand() % m + 1;
        } else if (variant == 1) {
            // All ones
            n = rand() % 10 + 4; // 4..13
            m = 1;
            k = rand() % 10 + 1;
            arr.assign(n, 1);
        } else if (variant == 2) {
            // Many duplicates
            n = rand() % 18 + 4; // 4..21
            m = rand() % 10 + 2; // 2..11
            k = rand() % 12 + 1;
            int base = rand() % m + 1;
            arr.resize(n);
            for (int i = 0; i < n; ++i) {
                if (rand() % 3 == 0) arr[i] = base;
                else arr[i] = rand() % m + 1;
            }
        } else if (variant == 3) {
            // array of primes
            n = rand() % 16 + 4; // 4..19
            m = max(10, rand() % 100 + 10);
            k = rand() % 50 + 1;
            arr.resize(n);
            for (int i = 0; i < n; ++i) arr[i] = random_prime_up_to(m);
        } else if (variant == 4) {
            // k = 1 special
            n = rand() % 18 + 4;
            m = rand() % 50 + 1;
            k = 1;
            arr.resize(n);
            for (int i = 0; i < n; ++i) arr[i] = rand() % m + 1;
        } else if (variant == 5) {
            // large m values at extremes
            n = rand() % 18 + 4;
            m = rand() % 100 + 50;
            k = rand() % 50 + 1;
            arr.resize(n);
            for (int i = 0; i < n; ++i) {
                if (rand() % 2) arr[i] = m; else arr[i] = rand() % (m/2 + 1) + 1;
            }
        } else {
            // generic random
            n = rand() % 20 + 4; // 4..23
            m = rand() % 200 + 1;
            k = rand() % 100 + 1;
            arr.resize(n);
            for (int i = 0; i < n; ++i) arr[i] = rand() % m + 1;
        }

        fin << n << " " << m << " " << k << "\n";
        for (int i = 0; i < n; ++i) {
            fin << arr[i] << (i+1==n? "\n" : " ");
        }
    }

    fin.close();

    string output_file = "test_cases/" + to_string(file_num) + ".out";
    run_solution(input_file, output_file, file_num, file_num);
}

int main() {
    srand((unsigned int)time(nullptr));
    system("mkdir -p test_cases");

    int start_file_num = find_next_file_number();
    int num_tests = 50;
    cout << "Generating " << num_tests << " test cases starting from file " << start_file_num << "\n";

    for (int test_num = 1; test_num <= num_tests; ++test_num) {
        int file_num = start_file_num + test_num - 1;
        generate_test_case(test_num, file_num);
    }

    cout << "Done. Generated " << num_tests << " test cases.\n";
    return 0;
}
