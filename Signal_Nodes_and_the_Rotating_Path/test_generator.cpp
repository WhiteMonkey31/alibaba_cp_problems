#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include<queue>
using namespace std;

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
// Problem logic inside solve_optimal()
// --------------------------------------------------
string solve_optimal(const string& input_str) {
    IORedirector redirector(input_str);

    int T; cin >> T;
    while (T--) {
        int n, m;
        cin >> n >> m;
        vector<vector<int>> adj(n);
        vector<int> shift(n);
        for (int i = 0; i < n; i++) cin >> shift[i];
        for (int i = 0; i < m; i++) {
            int u, v; cin >> u >> v; --u; --v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        const long long INF = 1LL << 60;
        vector<vector<pair<long long,long long>>> dist(n);
        for (int i = 0; i < n; i++) {
            int degU = (int)adj[i].size();
            dist[i].assign(max(1, degU), {INF, INF});
        }

        struct State { long long time, wait; int node, phase; 
            bool operator>(const State& other) const {
                if(time != other.time) return time > other.time;
                return wait > other.wait;
            }
        };
        priority_queue<State, vector<State>, greater<State>> pq;
        dist[0][0] = {0, 0};
        pq.push({0, 0, 0, 0});

        while (!pq.empty()) {
            auto [time, wait, u, phase] = pq.top(); pq.pop();
            if(make_pair(time, wait) != dist[u][phase]) continue;
            int degU = (int)adj[u].size();
            if(degU == 0) continue;

            int forcedIndex = (time + shift[u]) % degU;
            int v = adj[u][forcedIndex];
            int degV = (int)adj[v].size();
            int newPhaseV = (time+1) % degV;
            pair<long long,long long> candMove = {time+1, wait};
            if(candMove < dist[v][newPhaseV]) {
                dist[v][newPhaseV] = candMove;
                pq.push({time+1, wait, v, newPhaseV});
            }

            int newPhaseWait = (phase+1)%degU;
            pair<long long,long long> candWait = {time+1, wait+1};
            if(candWait < dist[u][newPhaseWait]) {
                dist[u][newPhaseWait] = candWait;
                pq.push({time+1, wait+1, u, newPhaseWait});
            }
        }

        pair<long long,long long> ans = {INF, INF};
        for(auto &p : dist[n-1]) ans = min(ans, p);
        if(ans.first == INF) cout << "-1\n";
        else cout << ans.first << " " << ans.second << "\n";
    }

    return redirector.get_output();
}

// --------------------------------------------------
// Utilities for test generation
// --------------------------------------------------
int find_next_file_number() {
    int next_num = 1;
    while(true) {
        string filename = "test_cases/" + to_string(next_num) + ".in";
        ifstream file(filename);
        if(!file.good()) return next_num;
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
    int n = rand()%10 + 2;  // 2..11 nodes
    int m = rand()%min(n*(n-1)/2,10) + 1;  // edges 1..10 or <= n*(n-1)/2
    string input_file = "test_cases/" + to_string(file_num) + ".in";
    ofstream fin(input_file);
    fin << "1\n" << n << " " << m << "\n";

    // shifts
    for(int i=0;i<n;i++) fin << rand()%5 << " ";
    fin << "\n";

    // edges
    vector<pair<int,int>> edges;
    for(int i=0;i<m;i++) {
        int u = rand()%n;
        int v = rand()%n;
        while(u==v) v = rand()%n;
        edges.push_back({u,v});
        fin << u+1 << " " << v+1 << "\n";
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
    for(int test_num=1; test_num<=num_tests; test_num++){
        int file_num = start_file_num + test_num - 1;
        generate_test_case(test_num, file_num);
    }
    cout << "Generated " << num_tests << " test cases.\n";
    return 0;
}
