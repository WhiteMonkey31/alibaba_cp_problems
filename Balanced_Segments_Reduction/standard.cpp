

// #include <iostream>
// #include<vector>
#include <iostream>
// #include <fstream>
// #include <sstream>
// #include <string>
// #include <atomic>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <vector>
// #include<numeric>
// #include<array>
// #include<unordered_map>
#include<functional>
#include<utility>
#include<string>


// #include <cmath>
#include <algorithm>
#include<unordered_set>
#include<queue>


using namespace std;
using int64 = long long;

int MAX_DEPTH = 8;           // Increase with caution (exponential blow-up)
double GLOBAL_TIME_LIMIT = 9.0; // seconds per test case

struct Timer {
    chrono::steady_clock::time_point start;
    Timer() { start = chrono::steady_clock::now(); }
    double elapsed() const {
        using namespace chrono;
        return duration_cast<duration<double>>(steady_clock::now() - start).count();
    }
};


int gcd(int a, int b) {
    while (b != 0) {
        a %= b;
        std::swap(a, b);
    }
    return a;
}


inline int64 gcd64(int64 a, int64 b) {
    if (a == 0) return b;
    if (b == 0) return a;
    return gcd(a, b);
}
inline int64 lcm64(int64 a, int64 b) {
    if (a == 0 || b == 0) return (a == 0 ? b : a);
    return (a / gcd64(a,b)) * b;
}

// Utility: convert vector to canonical string key
string state_key(const vector<int64>& v) {
    string s;
    s.reserve(v.size()*12);
    for (size_t i=0;i<v.size();++i) {
        if (i) s.push_back(',');
        s += to_string(v[i]);
    }
    return s;
}

// compute segment maximum achievable sum given caps floor(a[i]/i)
int64 segment_max(const vector<int64>& a, int L, int R) {
    // L, R are 1-based inclusive
    int64 s = 0;
    for (int i=L;i<=R;++i) {
        int64 cap = a[i-1] / (int64)i;
        s += cap * (int64)i;
    }
    return s;
}
int64 segment_gcd_indices(int L, int R) {
    int64 g = 0;
    for (int i=L;i<=R;++i) g = gcd64(g, (int64)i);
    return g;
}


void gen_segment_solutions_backtrack(
    const vector<int64>& a,
    int L, int R,
    int64 S,
    vector<vector<int64>>& out_sols,
    size_t max_sols,
    const Timer& timer,
    double time_limit
) {
    int len = R - L + 1;
    vector<int64> caps(len), idxs(len);
    for (int j=0;j<len;++j) {
        int idx = L + j;
        idxs[j] = idx;
        caps[j] = a[idx-1] / (int64)idx;
    }
    vector<int64> current(len, 0);

    // Backtrack from rightmost to leftmost (try larger indices first) — often reduces branching
    function<void(int,int64)> dfs = [&](int pos, int64 rem) {
        if (timer.elapsed() > time_limit) return;
        if (out_sols.size() >= max_sols) return;
        if (rem < 0) return;
        if (pos < 0) {
            if (rem == 0) {
                out_sols.push_back(current);
            }
            return;
        }
        int64 idx = idxs[pos];
        int64 cap = caps[pos];
        // choose k from max down to 0
        int64 maxk = min(cap, rem / idx);
        for (int64 k = maxk; k >= 0; --k) {
            current[pos] = k * idx;
            dfs(pos-1, rem - k*idx);
            if (timer.elapsed() > time_limit) return;
            if (out_sols.size() >= max_sols) return;
        }
        current[pos] = 0;
    };

    dfs(len-1, S);
}

// Given current array a, enumerate possible next states by producing all valid b arrays.
// To avoid explosion, we restrict limits: S attempts limited and per-segment solutions limited.
void enumerate_moves(
    const vector<int64>& a,
    vector<vector<int64>>& moves, // append full-length b arrays
    const Timer& timer
) {
    int n = (int)a.size();
    // parameters to control explosion:
    const size_t MAX_SOL_PER_SEG = 500;    // max solutions we collect per segment per S
    const int MAX_S_ATTEMPTS_PER_CUT = 200; // max different S tried per (p,q)
    const double PER_CUT_TIME = 0.15;      // seconds per (p,q)
    // iterate p,q
    for (int p=1;p<=n-2;++p) {
        for (int q=p+1;q<=n-1;++q) {
            if (timer.elapsed() > GLOBAL_TIME_LIMIT) return;
            // compute segment maxes and gcds
            int64 mx1 = segment_max(a, 1, p);
            int64 mx2 = segment_max(a, p+1, q);
            int64 mx3 = segment_max(a, q+1, n);
            if (mx1 == 0 || mx2 == 0 || mx3 == 0) continue;
            int64 g1 = segment_gcd_indices(1,p);
            int64 g2 = segment_gcd_indices(p+1,q);
            int64 g3 = segment_gcd_indices(q+1,n);
            int64 l = g1;
            l = lcm64(l, g2);
            l = lcm64(l, g3);
            if (l == 0) l = 1;
            int64 maxS = min({mx1, mx2, mx3});
            if (maxS <= 0) continue;
            // start from highest S multiple of l
            int64 S = (maxS / l) * l;
            int attempts = 0;
            Timer cutTimer;
            while (S > 0 && attempts < MAX_S_ATTEMPTS_PER_CUT && cutTimer.elapsed() < PER_CUT_TIME && timer.elapsed() < GLOBAL_TIME_LIMIT) {
                // quick check
                if (S > mx1 || S > mx2 || S > mx3) { S -= l; ++attempts; continue; }
                // generate per-segment solutions (with caps)
                vector<vector<int64>> sol1, sol2, sol3;
                double per_seg_time = min( (PER_CUT_TIME / 3.0), 0.04 );
                gen_segment_solutions_backtrack(a, 1, p, S, sol1, MAX_SOL_PER_SEG, cutTimer, per_seg_time);
                if (sol1.empty()) { S -= l; ++attempts; continue; }
                gen_segment_solutions_backtrack(a, p+1, q, S, sol2, MAX_SOL_PER_SEG, cutTimer, per_seg_time);
                if (sol2.empty()) { S -= l; ++attempts; continue; }
                gen_segment_solutions_backtrack(a, q+1, n, S, sol3, MAX_SOL_PER_SEG, cutTimer, per_seg_time);
                if (sol3.empty()) { S -= l; ++attempts; continue; }
                // now we have lists; combine them to full b arrays
                // cap total combinations to avoid explosion
                size_t cap_total_combinations = 2000;
                size_t count = 0;
                for (auto &v1: sol1) {
                    for (auto &v2: sol2) {
                        for (auto &v3: sol3) {
                            vector<int64> b(n, 0);
                            // copy v1 into b[0..p-1]
                            for (int i=0;i<(int)v1.size();++i) b[i] = v1[i];
                            for (int i=0;i<(int)v2.size();++i) b[p + i] = v2[i];
                            for (int i=0;i<(int)v3.size();++i) b[q + i] = v3[i];
                            // validate b <= a (should hold by construction)
                            bool ok = true;
                            for (int i=0;i<n;++i) {
                                if (b[i] < 0 || b[i] > a[i]) { ok = false; break; }
                                if (b[i] % (i+1) != 0) { ok = false; break; }
                            }
                            if (!ok) continue;
                            // ensure segment sums equal S (should hold)
                            int64 s1=0,s2=0,s3=0;
                            for (int i=0;i<p;++i) s1 += b[i];
                            for (int i=p;i<q;++i) s2 += b[i];
                            for (int i=q;i<n;++i) s3 += b[i];
                            if (s1==S && s2==S && s3==S) {
                                moves.push_back(b);
                                ++count;
                                if (count >= cap_total_combinations) break;
                            }
                        }
                        if (count >= cap_total_combinations) break;
                    }
                    if (count >= cap_total_combinations) break;
                }
                // done with this S
                S -= l;
                ++attempts;
            } // S loop
            if (timer.elapsed() > GLOBAL_TIME_LIMIT) return;
        } // q
    } // p
}

// BFS search to find minimal operations
// returns vector of operations (each vector<int64> b), or empty if impossible within limits
vector<vector<int64>> find_min_operations_exact(vector<int64> start) {
    Timer timer;
    int n = (int)start.size();
    // trivial
    bool allzero = true;
    for (auto x : start) if (x != 0) { allzero = false; break; }
    if (allzero) return {};
    // BFS
    queue<pair<vector<int64>, vector<vector<int64>>>> q; // state, ops so far
    unordered_set<string> seen;
    string key0 = state_key(start);
    q.push({start, {}});
    seen.insert(key0);

    int depth = 0;
    while (!q.empty()) {
        auto cur = q.front(); q.pop();
        vector<int64> a = cur.first;
        vector<vector<int64>> ops_so_far = cur.second;
        if ((int)ops_so_far.size() > MAX_DEPTH) continue;
        // check time
        if (timer.elapsed() > GLOBAL_TIME_LIMIT) break;
        // enumerate moves from a
        vector<vector<int64>> moves;
        enumerate_moves(a, moves, timer);
        // sort/make unique moves to reduce duplicates
        // we will proceed through moves
        for (auto &b : moves) {
            // apply
            vector<int64> nxt(n);
            bool ok = true;
            for (int i=0;i<n;++i) {
                if (b[i] < 0 || b[i] > a[i]) { ok = false; break; }
                nxt[i] = a[i] - b[i];
            }
            if (!ok) continue;
            // check zero
            bool iszero = true;
            for (auto v : nxt) if (v != 0) { iszero = false; break; }
            vector<vector<int64>> ops_next = ops_so_far;
            ops_next.push_back(b);
            if (iszero) return ops_next; // BFS ensures minimal ops
            string k = state_key(nxt);
            if (!seen.count(k)) {
                seen.insert(k);
                q.push({nxt, ops_next});
            }
            // small time check
            if (timer.elapsed() > GLOBAL_TIME_LIMIT) break;
        }
        if (timer.elapsed() > GLOBAL_TIME_LIMIT) break;
    }
    // if we exit BFS without reaching zero
    return {}; // empty -> no solution found within limits/time
}

// ---------- I/O and main ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;
    for (int tc=0; tc<T; ++tc) {
        int n; cin >> n;
        vector<int64> a(n);
        for (int i=0;i<n;++i) cin >> a[i];

        // Optional: for interactive tuning you can set MAX_DEPTH or GLOBAL_TIME_LIMIT based on n
        if (n <= 6) { MAX_DEPTH = 10; GLOBAL_TIME_LIMIT = 6.0; }
        else if (n <= 8) { MAX_DEPTH = 8; GLOBAL_TIME_LIMIT = 7.5; }
        else { MAX_DEPTH = 6; GLOBAL_TIME_LIMIT = 9.0; }

        auto ops = find_min_operations_exact(a);
        if (ops.empty()) {
            cout << -1 << "\n";
        } else {
            cout << ops.size() << "\n";
            for (auto &b : ops) {
                for (int i=0;i<n;++i) {
                    if (i) cout << ' ';
                    cout << b[i];
                }
                cout << "\n";
            }
        }
    }
    return 0;
}
