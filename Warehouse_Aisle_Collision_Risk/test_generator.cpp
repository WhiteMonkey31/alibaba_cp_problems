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

// =====================================================
// Your solution inside solve_optimal()
// =====================================================
struct AccSegment {
    long double t; // start time
    long double a; // acceleration
};

struct Robot {
    int type;
    long double pos, vel;
    vector<AccSegment> segments;
};

const long double EPS = 1e-12L;
const long double D = 24.0L;

vector<long double> solve_quadratic(long double a, long double b, long double c) {
    vector<long double> roots;
    if(fabsl(a) < EPS) {
        if(fabsl(b) < EPS) return roots;
        roots.push_back(-c/b);
        return roots;
    }
    long double disc = b*b - 4*a*c;
    if(disc < -EPS) return roots;
    if(fabsl(disc) < EPS) roots.push_back(-b/(2*a));
    else {
        long double sd = sqrt(disc);
        roots.push_back((-b - sd)/(2*a));
        roots.push_back((-b + sd)/(2*a));
    }
    return roots;
}

bool collide_segment(long double p1, long double v1, long double a1,
                     long double p2, long double v2, long double a2,
                     long double t0, long double t1) {
    long double dy = p1 - p2;
    long double dv = v1 - v2;
    long double da = a1 - a2;
    long double A = 0.5L*da;

    vector<long double> roots;
    auto r1 = solve_quadratic(A, dv, dy - D);
    auto r2 = solve_quadratic(A, dv, dy + D);
    roots.insert(roots.end(), r1.begin(), r1.end());
    roots.insert(roots.end(), r2.begin(), r2.end());
    roots.push_back(t0); roots.push_back(t1);

    sort(roots.begin(), roots.end());
    roots.erase(unique(roots.begin(), roots.end(),
                       [](long double x, long double y){return fabsl(x-y)<1e-12;}), roots.end());

    auto f = [&](long double t){
        return dy + dv*(t-t0) + A*(t-t0)*(t-t0);
    };

    for(auto t: roots){
        if(t+EPS < t0 || t-EPS > t1) continue;
        if(fabsl(f(t)) <= D + EPS) return true;
    }

    for(size_t k=0;k+1<roots.size();k++){
        long double L = max(roots[k], t0);
        long double R = min(roots[k+1], t1);
        if(L+EPS>=R) continue;
        long double mid = (L+R)/2.0L;
        if(fabsl(f(mid)) <= D + EPS) return true;
    }

    return false;
}

string solve_optimal(const string& input_str) {
    IORedirector redirector(input_str);

    int N, M;
    long double T;
    cin >> N >> M >> T;
    vector<Robot> robots(N+M);

    for(int i=0;i<N+M;i++){
        int type; long double p,v; cin>>type>>p>>v;
        robots[i].type = type; robots[i].pos = p; robots[i].vel = v;
        int k; cin>>k;
        robots[i].segments.resize(k);
        for(int j=0;j<k;j++) cin >> robots[i].segments[j].t >> robots[i].segments[j].a;
    }

    long long collisions = 0;
    for(int i=0;i<N+M;i++){
        for(int j=i+1;j<N+M;j++){
            if(robots[i].type == robots[j].type) continue;

            long double t_prev = 0.0L;
            long double p1 = robots[i].pos, v1 = robots[i].vel;
            long double p2 = robots[j].pos, v2 = robots[j].vel;
            size_t idx1=0, idx2=0;
            bool collided=false;

            while(t_prev<T && !collided){
                long double t_next = T;
                if(idx1<robots[i].segments.size()) t_next = min(t_next, robots[i].segments[idx1].t);
                if(idx2<robots[j].segments.size()) t_next = min(t_next, robots[j].segments[idx2].t);

                long double a1 = (idx1<robots[i].segments.size())?robots[i].segments[idx1].a:0.0L;
                long double a2 = (idx2<robots[j].segments.size())?robots[j].segments[idx2].a:0.0L;

                if(collide_segment(p1, v1, a1, p2, v2, a2, t_prev, t_next)){
                    collided=true; break;
                }

                long double dt = t_next - t_prev;
                p1 += v1*dt + 0.5*a1*dt*dt;
                v1 += a1*dt;
                p2 += v2*dt + 0.5*a2*dt*dt;
                v2 += a2*dt;

                t_prev = t_next;
                if(idx1<robots[i].segments.size() && fabsl(t_prev - robots[i].segments[idx1].t)<EPS) idx1++;
                if(idx2<robots[j].segments.size() && fabsl(t_prev - robots[j].segments[idx2].t)<EPS) idx2++;
            }

            if(collided) collisions++;
        }
    }

    cout << collisions << "\n";
    return redirector.get_output();
}

// =====================================================
// Template utility functions
// =====================================================
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
    int N = rand() % 5 + 1; // 1..5 robots type 0
    int M = rand() % 5 + 1; // 1..5 robots type 1
    long double T = (rand() % 20 + 5); // 5..25 seconds

    string input_file = "test_cases/" + to_string(file_num) + ".in";
    ofstream fin(input_file);

    fin << N << " " << M << " " << T << "\n";

    for (int i = 0; i < N+M; i++) {
        int type = (i < N ? 0 : 1);
        long double pos = (rand() % 2000 - 1000); // -1000..1000 cm
        long double vel = (rand() % 200 - 100) / 10.0; // -10..10 cm/s
        int k = rand() % 3 + 1; // 1..3 segments
        fin << type << " " << pos << " " << vel << " " << k << "\n";
        long double t_prev = 0;
        for (int j = 0; j < k; j++) {
            t_prev += (rand() % 5 + 1); // segment start time
            long double a = (rand() % 200 - 100) / 10.0; // -10..10 cm/s^2
            fin << t_prev << " " << a << "\n";
        }
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
