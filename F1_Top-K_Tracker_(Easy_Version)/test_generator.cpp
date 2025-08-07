// optional: test case generator (can be .py too)

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

vector<int> hidden_perm;
vector<int> pos_of;
int n;

// Utility to print top-k largest elements from a vector<int>, in increasing order
vector<int> get_top_k(const vector<int>& vals, int k) {
    vector<int> copy = vals;
    sort(copy.begin(), copy.end());
    if ((int)copy.size() > k)
        copy.erase(copy.begin(), copy.end() - k);
    return copy;
}

// Query Type 1: return top k in [pi1, pi2, ..., pim]
void handle_query_type_1(const vector<int>& indices) {
    vector<int> vals;
    for (int idx : indices) vals.push_back(hidden_perm[idx - 1]);
    vector<int> res = get_top_k(vals, min(60, (int)vals.size()));
    for (int v : res) cout << v << " ";
    cout << endl;
    cout.flush();
}

// Query Type 2: return top k in [pos[i1], pos[i2], ..., pos[im]]
void handle_query_type_2(const vector<int>& values) {
    vector<int> vals;
    for (int val : values) vals.push_back(pos_of[val]);
    vector<int> res = get_top_k(vals, min(60, (int)vals.size()));
    for (int v : res) cout << v << " ";
    cout << endl;
    cout.flush();
}

int main() {
    int t;
    cin >> t;
    cout << t << endl; // echo test case count
    for (int test = 0; test < t; ++test) {
        cin >> n;
        cout << n << endl;

        // Generate hidden permutation with fixed seed
        hidden_perm.resize(n);
        for (int i = 0; i < n; ++i) hidden_perm[i] = i + 1;
        unsigned seed = 1337 + test; // consistent for each test
        shuffle(hidden_perm.begin(), hidden_perm.end(), default_random_engine(seed));

        pos_of.assign(n + 1, 0);
        for (int i = 0; i < n; ++i) pos_of[hidden_perm[i]] = i + 1;

        string cmd;
        while (cin >> cmd) {
            if (cmd == "!") {
                vector<int> guess(n);
                for (int i = 0; i < n; ++i) cin >> guess[i];

                // Check answer
                bool correct = guess == hidden_perm;
                cout << (correct ? "Correct permutation!\n" : "Wrong permutation!\n");
                break;
            } else {
                int m;
                cin >> m;
                vector<int> indices(m);
                for (int i = 0; i < m; ++i) cin >> indices[i];

                if (cmd == "1") handle_query_type_1(indices);
                else if (cmd == "2") handle_query_type_2(indices);
                else {
                    cout << "Query type not implemented in judge.\n";
                    cout.flush();
                }
            }
        }
    }
    return 0;
}
