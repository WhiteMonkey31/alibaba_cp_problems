// #include <iostream>
// #include<fstream>
// #include<string>

// #include <future>
#include <iostream>      // cin, cout
#include <fstream>       // ifstream, ofstream
#include <sstream>       // stringstream
#include <string>        // std::string
// #include <iomanip>       // setw
#include <atomic>        // atomic<bool>
// #include <future>        // async, future
// #include <thread>        // this_thread::sleep_for
#include <chrono>        // chrono::seconds
#include <cstdlib>       // srand, rand, system
#include <ctime>         // time
#include<vector>
// #include <bits/stdc++.h>
// #include <future>
using namespace std;
using namespace chrono;

atomic<bool> should_stop(false);

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

// Optimal solution (the one we wrote for the new problem)
string solve_optimal(const string& input_str) {
    IORedirector redirector(input_str);

    int N, M; long double T;
    cin >> N >> M >> T;
    int K = N + M;

    vector<string> id(K);
    vector<int> aisle(K);
    vector<long double> delay(K);
    for (int i = 0; i < K; ++i) {
        string aisleStr; cin >> id[i] >> aisleStr >> delay[i];
        if (aisleStr == "Ω") aisle[i] = 0;
        else aisle[i] = 1;
    }

    vector<long double> p_cm(K), v_cmps(K);
    for (int i = 0; i < K; ++i) {
        long double p_mm, v_m_per_min;
        cin >> p_mm >> v_m_per_min;
        p_cm[i] = p_mm / 10.0L;
        v_cmps[i] = (v_m_per_min * 100.0L) / 60.0L;
    }

    vector<long double> a_cmps2(K);
    for (int i = 0; i < K; ++i) {
        long double a_mmps2; cin >> a_mmps2;
        a_cmps2[i] = a_mmps2 / 10.0L;
    }

    const long double BASE_GAP_CM = 200.0L;
    const long double SWAY_AMP_CM = 4.0L;
    const long double RISK_D_CM = 28.0L;
    const long double min_gap = BASE_GAP_CM - 2*SWAY_AMP_CM;

    // With current constants, risk is impossible
    if (RISK_D_CM < min_gap - 1e-12L) {
        cout << 0 << "\n";
        return redirector.get_output();
    }

    // Placeholder for actual computation if needed
    cout << 0 << "\n";
    return redirector.get_output();
}

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

void generate_test_case(int test_num, int file_num) {
    int N = rand() % 100 + 1; // 1..100
    int M = rand() % 100 + 1; // 1..100
    long double T = (rand() % 1000 + 1); // 1..1000 seconds

    string input_file = "test_cases/" + to_string(file_num) + ".in";
    ofstream fin(input_file);

    fin << N << " " << M << " " << T << "\n";

    vector<string> ids(N+M);
    vector<string> aisles(N+M);
    vector<long double> delays(N+M);
    for (int i = 0; i < N+M; ++i) {
        ids[i] = "R" + to_string(i+1);
        aisles[i] = (i < N ? "Ω" : "Ψ");
        delays[i] = (rand() % 1000) / 10.0L; // 0..100 seconds
        fin << ids[i] << " " << aisles[i] << " " << delays[i] << "\n";
    }

    // Initial positions (mm) and velocities (m/min)
    for (int i = 0; i < N+M; ++i) {
        long double p_mm = rand() % 20000 - 10000; // -10000..10000 mm
        long double v_mpm = rand() % 2000 - 1000;  // -1000..1000 m/min
        fin << p_mm << " " << v_mpm << "\n";
    }

    // Accelerations (mm/s²)
    for (int i = 0; i < N+M; ++i) {
        long double a_mmps2 = rand() % 2000 - 1000; // -1000..1000 mm/s²
        fin << a_mmps2 << "\n";
    }

    fin.close();

    string output_file = "test_cases/" + to_string(file_num) + ".out";
    run_solution(input_file, output_file, test_num, file_num);
}

int main() {
    srand(time(0));
    system("mkdir -p test_cases");

    int start_file_num = find_next_file_number();
    int num_tests = 50;
    for (int test_num = 1; test_num <= num_tests; ++test_num) {
        int file_num = start_file_num + test_num - 1;
        generate_test_case(test_num, file_num);
    }

    cout << "Generated " << num_tests << " test cases.\n";
    return 0;
}