// balanced_segments_solver.cpp
// Practical solver for the "Balanced Segments Reduction" modified problem
// (three equal segments, b[i] divisible by i).
//
// Note: heuristic + exact. Not guaranteed poly-time for worst-case, but
// works for many human-designed and moderate-size instances.

// #include <bits/stdc++.h>
#include <iostream>
// #include <fstream>
// #include <sstream>
// #include <string>
// #include <atomic>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <vector>
#include<numeric>
#include<array>

// #include <cmath>
#include <algorithm>
#include<unordered_set>

using namespace std;
using int64 = long long;
const double GLOBAL_TIME_LIMIT = 1.9; // per test-case soft limit (seconds)
const int OP_LIMIT = 60;

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

int64 lcm64(int64 a, int64 b) {
    if (a == 0 || b == 0) return (a == 0 ? b : a);
    return (a / gcd64(a,b)) * b;
}

vector<int64> prefix_sum(const vector<int64>& a) {
    vector<int64> ps(a.size()+1);
    ps[0] = 0;
    for (size_t i=0;i<a.size();++i) ps[i+1] = ps[i] + a[i];
    return ps;
}

int64 segment_max(const vector<int64>& a, int L, int R) {
    // L,R are 1-based inclusive
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

// Greedy fill: take largest indices first
// Returns vector of length (R-L+1) with actual b-values (multiples of index),
// or empty vector if cannot reach S.
vector<int64> greedy_fill_segment(const vector<int64>& a, int L, int R, int64 S) {
    vector<int64> out(R-L+1, 0);
    int64 rem = S;
    for (int idx=R; idx>=L; --idx) {
        int64 cap = a[idx-1] / (int64)idx;
        if (cap <= 0) continue;
        int64 take = min(cap, rem / (int64)idx);
        out[idx-L] = take * (int64)idx;
        rem -= out[idx-L];
        if (rem == 0) break;
    }
    if (rem != 0) return {};
    return out;
}

// Backtracking for bounded subset-sum on small segments.
// values: indices (i), caps: counts, target S.
// Returns counts vector (same length) or empty vector if impossible.
bool backtrack_bounded(const vector<int64>& vals, const vector<int64>& caps, int idx, int64 target,
                       vector<int64>& solution, unordered_set<unsigned long long>& memo, const Timer& t, double time_limit) {
    if (t.elapsed() > time_limit) return false;
    if (target == 0) {
        for (size_t i=idx;i<vals.size();++i) solution[i] = 0;
        return true;
    }
    if (idx >= (int)vals.size()) return false;
    // memo key combine
    unsigned long long key = ((unsigned long long)idx << 40) ^ (unsigned long long)(target & ((1ULL<<40)-1));
    if (memo.find(key) != memo.end()) return false;
    int64 val = vals[idx];
    int64 maxk = min(caps[idx], target / val);
    // try larger k first to encourage greedy-like large usage
    for (int64 k = maxk; k>=0; --k) {
        solution[idx] = k;
        if (k*val > target) continue;
        if (backtrack_bounded(vals, caps, idx+1, target - k*val, solution, memo, t, time_limit)) return true;
        // early prune if time nearly exhausted
        if (t.elapsed() > time_limit) return false;
    }
    memo.insert(key);
    return false;
}

// Try to construct full b array for given cut (p,q). Returns empty vector if none found.
// time_limit: seconds budget for this try (per p,q)
vector<int64> try_construct_for_cut(vector<int64> a, int p, int q, double time_limit, const Timer& global_timer) {
    Timer t;
    int n = (int)a.size();
    // segments: [1..p], [p+1..q], [q+1..n]
    array<pair<int,int>,3> segs = { make_pair(1,p), make_pair(p+1,q), make_pair(q+1,n) };
    array<int64,3> maxs; array<int64,3> gcds;
    array<vector<pair<int,int64>>,3> caps; // (index, cap)
    for (int s=0;s<3;++s) {
        int L = segs[s].first, R = segs[s].second;
        int64 mx = 0; int64 g = 0;
        vector<pair<int,int64>> list;
        for (int i=L;i<=R;++i) {
            int64 cap = a[i-1] / (int64)i;
            mx += cap * (int64)i;
            g = gcd64(g, (int64)i);
            list.emplace_back(i, cap);
        }
        maxs[s] = mx; gcds[s] = g;
        caps[s] = move(list);
    }
    int64 maxS = min({maxs[0], maxs[1], maxs[2]});
    if (maxS == 0) {
        // maybe all zeros segment
        bool allz = true;
        for (auto v : a) if (v != 0) { allz = false; break; }
        if (allz) return vector<int64>(n, 0);
        return {};
    }
    // compute lcm of gcds
    int64 l = gcds[0];
    for (int i=1;i<3;++i) l = lcm64(l, gcds[i]);
    if (l == 0) l = 1;
    // Start from highest S multiple of l not exceeding maxS, iterate downward with limited attempts
    int64 S = (maxS / l) * l;
    int attempts = 0, max_attempts = 300;
    while (S > 0 && attempts < max_attempts && t.elapsed() < time_limit && global_timer.elapsed() < GLOBAL_TIME_LIMIT) {
        // quick check each segment can reach S
        bool okSegs = true;
        for (int s=0;s<3;s++) if (S > maxs[s]) { okSegs = false; break; }
        if (!okSegs) { S -= l; ++attempts; continue; }

        // Attempt to get feasible b for each segment
        vector<int64> full_b(n, 0);
        bool segfail = false;
        for (int s=0;s<3 && !segfail;s++) {
            int L = segs[s].first, R = segs[s].second;
            int seglen = R - L + 1;
            // small segment -> exact backtrack
            if (seglen <= 24) {
                vector<int64> vals; vector<int64> cnts;
                for (auto &pr : caps[s]) { vals.push_back(pr.first); cnts.push_back(pr.second); }
                vector<int64> sol(vals.size(), 0);
                unordered_set<unsigned long long> memo;
                bool ok = backtrack_bounded(vals, cnts, 0, 0, sol, memo, t, time_limit); // dummy call? We'll call real below
                // Real call:
                sol.assign(vals.size(), 0);
                memo.clear();
                bool found = backtrack_bounded(vals, cnts, 0, S, sol, memo, t, time_limit);
                if (!found) { segfail = true; break; }
                // map sol to full_b
                for (size_t j=0;j<vals.size();++j) {
                    int idx = (int)vals[j];
                    full_b[idx-1] = sol[j] * vals[j];
                }
            } else {
                // Try greedy
                vector<int64> greedy = greedy_fill_segment(a, L, R, S);
                if (!greedy.empty()) {
                    for (int j=0;j<seglen;++j) full_b[L+j-1] = greedy[j];
                } else {
                    // fallback: try limited exact on top-K largest indices
                    int K = min(22, seglen); // tuneable
                    // build vector of (i,cap) sorted desc by i
                    vector<pair<int,int64>> items = caps[s];
                    sort(items.begin(), items.end(), [](const pair<int,int64>& A, const pair<int,int64>& B){
                        return A.first > B.first;
                    });
                    vector<pair<int,int64>> top(items.begin(), items.begin()+K);
                    vector<pair<int,int64>> rest(items.begin()+K, items.end());
                    int64 rest_max = 0;
                    for (auto &pr : rest) rest_max += pr.first * pr.second;
                    int64 top_max = 0;
                    for (auto &pr : top) top_max += pr.first * pr.second;
                    int64 need_top_min = max<int64>(0, S - rest_max);
                    if (need_top_min > top_max) { segfail = true; break; }
                    // try several target_top values (descending)
                    bool found_any = false;
                    // produce top vals and caps
                    vector<int64> top_vals; vector<int64> top_caps;
                    for (auto &pr : top) { top_vals.push_back(pr.first); top_caps.push_back(pr.second); }
                    vector<int64> sol_top(top_vals.size(), 0);
                    for (int trials = 0; trials < 8 && !found_any && t.elapsed() < time_limit; ++trials) {
                        // pick a candidate target for top in [need_top_min .. min(top_max, S)]
                        int64 cand = min(top_max, S) - trials * (max<int64>(1, (min(top_max, S) - need_top_min) / 8 + 1));
                        if (cand < need_top_min) cand = need_top_min;
                        // ensure cand is multiple of gcd of top indices maybe not needed
                        unordered_set<unsigned long long> memo;
                        sol_top.assign(top_vals.size(), 0);
                        bool ok = backtrack_bounded(top_vals, top_caps, 0, cand, sol_top, memo, t, time_limit);
                        if (!ok) continue;
                        // fill rest greedily to match S - cand
                        int64 rem = S - cand;
                        vector<int64> rest_fill(rest.size(), 0);
                        // sort rest desc
                        sort(rest.begin(), rest.end(), [](auto &A, auto &B){ return A.first > B.first; });
                        for (size_t j=0;j<rest.size() && rem>0;++j) {
                            int64 take = min(rest[j].second, rem / rest[j].first);
                            rest_fill[j] = take * rest[j].first;
                            rem -= rest_fill[j];
                        }
                        if (rem == 0) {
                            // compose full_b for this segment
                            // place top sol_top into full_b (we must match indices positions)
                            for (size_t j=0;j<top_vals.size();++j) {
                                int idx = (int)top_vals[j];
                                full_b[idx-1] = sol_top[j] * top_vals[j];
                            }
                            // place rest_fill
                            for (size_t j=0;j<rest.size();++j) {
                                int idx = (int)rest[j].first;
                                full_b[idx-1] = rest_fill[j];
                            }
                            found_any = true;
                            break;
                        }
                    }
                    if (!found_any) { segfail = true; break; }
                }
            }
        } // for segments

        if (!segfail) {
            // verify full_b valid
            bool ok = true;
            for (int i=0;i<n;++i) {
                if (full_b[i] < 0 || full_b[i] > a[i]) { ok = false; break; }
                if (full_b[i] % (int64)(i+1) != 0) { ok = false; break; }
            }
            if (!ok) {
                S -= l; ++attempts; continue;
            }
            int64 s1=0,s2=0,s3=0;
            for (int i=0;i<p;i++) s1 += full_b[i];
            for (int i=p;i<q;i++) s2 += full_b[i];
            for (int i=q;i<n;i++) s3 += full_b[i];
            if (s1==s2 && s2==s3 && s1>0) {
                return full_b;
            }
        }
        S -= l; ++attempts;
    }
    return {};
}

// Solve single case, return vector of operations (each op is vector<int64> b), or empty if impossible
vector<vector<int64>> solve_case(vector<int64> a) {
    Timer global_timer;
    int n = (int)a.size();
    vector<vector<int64>> ops;
    if (accumulate(a.begin(), a.end(), (int64)0) == 0) return ops;
    for (int step=0; step<OP_LIMIT && global_timer.elapsed() < GLOBAL_TIME_LIMIT; ++step) {
        bool made_progress = false;
        int64 total = accumulate(a.begin(), a.end(), (int64)0);
        // heuristics: candidate p and q near total/3 and 2*total/3
        vector<int64> ps = prefix_sum(a);
        int64 t1 = total / 3;
        int64 t2 = (2 * total) / 3;
        vector<pair<int64,int>> clos1, clos2;
        for (int i=1;i<n;i++) {
            clos1.emplace_back( llabs(ps[i] - t1), i );
            clos2.emplace_back( llabs(ps[i] - t2), i );
        }
        sort(clos1.begin(), clos1.end());
        sort(clos2.begin(), clos2.end());
        int cap_pick = min((int)clos1.size(), 60);
        vector<int> cand_p, cand_q;
        for (int i=0;i<cap_pick;i++) cand_p.push_back(clos1[i].second);
        for (int i=0;i<cap_pick;i++) cand_q.push_back(clos2[i].second);
        // try cross product but with pruning and time limits
        double per_pair_time = 0.06; // seconds budget per p,q
        for (int pi=0; pi<(int)cand_p.size() && global_timer.elapsed() < GLOBAL_TIME_LIMIT; ++pi) {
            for (int qi=0; qi<(int)cand_q.size() && global_timer.elapsed() < GLOBAL_TIME_LIMIT; ++qi) {
                int p = cand_p[pi], q = cand_q[qi];
                if (!(1 <= p && p < q && q < n)) continue;
                // cheap necessary checks
                int64 mx1 = segment_max(a, 1, p);
                int64 mx2 = segment_max(a, p+1, q);
                int64 mx3 = segment_max(a, q+1, n);
                if (mx1 == 0 || mx2 == 0 || mx3 == 0) continue;
                int64 g1 = segment_gcd_indices(1,p), g2 = segment_gcd_indices(p+1,q), g3 = segment_gcd_indices(q+1,n);
                int64 l = g1;
                l = lcm64(l, g2); l = lcm64(l, g3);
                if (l == 0) l = 1;
                int64 maxS = min({mx1, mx2, mx3});
                if (maxS <= 0) continue;
                // call try_construct_for_cut with small per-pair time budget
                vector<int64> b = try_construct_for_cut(a, p, q, per_pair_time, global_timer);
                if (!b.empty()) {
                    // apply operation
                    ops.push_back(b);
                    for (int i=0;i<n;++i) a[i] -= b[i];
                    made_progress = true;
                    break;
                }
            }
            if (made_progress) break;
        }
        if (!made_progress) {
            // final attempt: if total divisible by 3, try brute forcing p,q with small extra budget
            if (total % 3 == 0 && global_timer.elapsed() < GLOBAL_TIME_LIMIT) {
                int64 S = total / 3;
                bool found = false;
                for (int p=1;p<n && !found && global_timer.elapsed() < GLOBAL_TIME_LIMIT; ++p) {
                    for (int q=p+1;q<n && !found && global_timer.elapsed() < GLOBAL_TIME_LIMIT; ++q) {
                        int64 mx1 = segment_max(a, 1, p);
                        int64 mx2 = segment_max(a, p+1, q);
                        int64 mx3 = segment_max(a, q+1, n);
                        if (mx1 < S || mx2 < S || mx3 < S) continue;
                        vector<int64> b = try_construct_for_cut(a, p, q, 0.08, global_timer);
                        if (!b.empty()) {
                            ops.push_back(b);
                            for (int i=0;i<n;++i) a[i] -= b[i];
                            made_progress = true;
                            found = true;
                            break;
                        }
                    }
                }
            }
        }
        if (!made_progress) break;
        // check if done
        bool allz = true;
        for (auto v : a) if (v != 0) { allz = false; break; }
        if (allz) return ops;
    }
    // final check
    bool allz = true;
    for (auto v : a) if (v != 0) { allz = false; break; }
    if (allz) return ops;
    return {}; // signal failure
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if (!(cin >> T)) return 0;
    // We will process each test case independently.
    for (int tc=0; tc<T; ++tc) {
        int n; cin >> n;
        vector<int64> a(n);
        for (int i=0;i<n;++i) cin >> a[i];
        vector<vector<int64>> ops = solve_case(a);
        if (ops.empty()) {
            cout << "-1\n";
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
