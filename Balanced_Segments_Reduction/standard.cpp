// balanced_segments_optimized.cpp
// Optimized hybrid solver for "Balanced Segments Reduction"
// (three equal segments; b[i] divisible by i).
//
// Heuristic+exact approach tuned to be robust and LLM-resistant.
//
// Compile: g++ -O2 -std=gnu++17 balanced_segments_optimized.cpp -o balanced_solver

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
#include<numeric>
#include<array>
#include<unordered_map>
#include<functional>
#include<utility>

// #include <cmath>
#include <algorithm>
// #include<unordered_set>
using namespace std;
using int64 = long long;

static const double GLOBAL_TIME_LIMIT = 1.8; // seconds per test case (soft)
static const double PER_CUT_TIME = 0.045;    // seconds spent trying a single (p,q)
static const int MAX_OPS = 50;               // fail after too many operations
static const int MAX_S_ATTEMPTS = 180;       // per (p,q)
static const int MAX_CAND_P = 48, MAX_CAND_Q = 48; // candidate ps and qs to try

struct Timer {
    chrono::steady_clock::time_point s;
    Timer() { s = chrono::steady_clock::now(); }
    double elapsed() const {
        using namespace chrono;
        return chrono::duration_cast<duration<double>>(steady_clock::now() - s).count();
    }
};

int gcd(int a, int b) {
    while (b != 0) {
        a %= b;
        std::swap(a, b);
    }
    return a;
}

inline int64 gcd64(int64 a, int64 b) { return b==0? a: gcd(a,b); }
inline int64 lcm64(int64 a, int64 b) { if(a==0 || b==0) return a^b; return (a / gcd64(a,b)) * b; }

// Precompute caps and prefix of cap-values for quick segment max queries
struct Precompute {
    int n;
    vector<int64> a;
    vector<int64> cap;       // cap[i] = floor(a[i]/(i+1))
    vector<int64> capVal;    // capVal[i] = cap[i] * (i+1)
    vector<int64> prefCapVal; // prefix sums of capVal
    vector<int64> prefA;     // prefix sums of a
    Precompute(const vector<int64>& arr) {
        a = arr; n = (int)a.size();
        cap.assign(n, 0);
        capVal.assign(n, 0);
        prefCapVal.assign(n+1, 0);
        prefA.assign(n+1, 0);
        for (int i=0;i<n;i++) {
            cap[i] = a[i] / (int64)(i+1);
            capVal[i] = cap[i] * (int64)(i+1);
            prefCapVal[i+1] = prefCapVal[i] + capVal[i];
            prefA[i+1] = prefA[i] + a[i];
        }
    }
    inline int64 segmentMax(int L, int R) const { // 1-based inclusive
        if (L>R) return 0;
        return prefCapVal[R] - prefCapVal[L-1];
    }
    inline int64 prefixA(int idx) const { return prefA[idx]; } // idx elements
};

// helper to compute gcd of index range
int64 gcd_range_indices(int L, int R) {
    int64 g = 0;
    for (int i=L;i<=R;i++) g = gcd64(g, (int64)i);
    return g;
}

// ------------------------ small-segment exact (meet-in-the-middle) ------------------------
// Solve bounded subset-sum where coin values are distinct indices and multiplicities are caps.
// For len <= 26 this is feasible by splitting into two halves.
// We return vector<int64> of chosen b-values (each multiple of index), or empty vector on fail.
// Note: target S must be <= segmentMax.
vector<int64> solve_segment_small_exact(const vector<int64>& a, int L, int R, int64 S, double timeBudget, const Timer& globalTimer) {
    // L,R are 1-based inclusive. Build values and caps
    int len = R - L + 1;
    vector<int> vals(len);
    vector<int64> caps(len);
    for (int i=0;i<len;i++){ vals[i] = L + i; caps[i] = a[L+i-1] / (int64)(L+i); }
    Timer t;
    // transform to multi-coin list by handling multiplicities but keep as (value, cap)
    // Meet-in-the-middle: enumerate possible sums in each half with multipliers
    int mid = len / 2;
    // Left half enumerations: vector of (sum, vector<multipliers for left half>)
    unordered_map<int64, vector<int64>> leftMap;
    // we'll cap enumerated combinations to prevent explosion
    const size_t LEFT_LIMIT = 1 << min(18, max(0, mid)); // soft limit
    // backtracking left half
    function<void(int,int64, vector<int64>&)> dfsL = [&](int idx, int64 cur, vector<int64>& multi) {
        if (globalTimer.elapsed() > GLOBAL_TIME_LIMIT) return;
        if (idx == mid) {
            // store
            leftMap[cur] = multi;
            return;
        }
        int v = vals[idx]; int64 capc = caps[idx];
        // iterate k from 0..min(cap, S/v) but prefer larger k first
        int64 maxk = min(capc, S / v);
        for (int64 k = maxk; k>=0; --k) {
            if (globalTimer.elapsed() > GLOBAL_TIME_LIMIT) return;
            multi[idx] = k;
            dfsL(idx+1, cur + k*(int64)v, multi);
            if (leftMap.size() > LEFT_LIMIT) return;
        }
        multi[idx] = 0;
    };
    vector<int64> mleft(len,0);
    dfsL(0, 0, mleft);
    if (globalTimer.elapsed() > GLOBAL_TIME_LIMIT) return {};
    // Right half
    int rlen = len - mid;
    const size_t RIGHT_LIMIT = 1 << min(18, max(0, rlen));
    function<vector<int64>()> findSolution = [&]() -> vector<int64> {
        vector<int64> mult(len,0);
        // backtrack right half and lookup complement in leftMap
        function<bool(int,int64, vector<int64>&)> dfsR = [&](int idx, int64 cur, vector<int64>& mr) -> bool {
            if (globalTimer.elapsed() > GLOBAL_TIME_LIMIT) return false;
            if (idx == len) {
                int64 need = S - cur;
                auto it = leftMap.find(need);
                if (it != leftMap.end()) {
                    // combine
                    for (int i=0;i<mid;i++) mult[i] = it->second[i];
                    for (int i=mid;i<len;i++) mult[i] = mr[i-mid];
                    return true;
                }
                return false;
            }
            int pos = idx - mid;
            if (idx < mid) return false;
            int v = vals[idx]; int64 capc = caps[idx];
            int64 maxk = min(capc, S / v);
            for (int64 k = maxk; k>=0; --k) {
                mr[pos] = k;
                if (dfsR(idx+1, cur + k*(int64)v, mr)) return true;
                if (globalTimer.elapsed() > GLOBAL_TIME_LIMIT) return false;
            }
            mr[pos] = 0;
            return false;
        };
        vector<int64> mr(rlen,0);
        if (dfsR(mid, 0, mr)) return mult;
        return {};
    };
    vector<int64> mults = findSolution();
    if (mults.empty()) return {};
    // convert multipliers to b-values
    vector<int64> bvals(len,0);
    for (int i=0;i<len;i++) bvals[i] = mults[i] * (int64)vals[i];
    return bvals;
}

// ------------------------ greedy + residue DP fallback ------------------------
// Greedy large-first attempt
vector<int64> greedy_fill(const vector<int64>& a, int L, int R, int64 S) {
    int len = R - L + 1;
    vector<int64> out(len, 0);
    int64 rem = S;
    for (int i=R;i>=L;i--) {
        int64 cap = a[i-1] / (int64)i;
        if (cap <= 0) continue;
        int64 take = min(cap, rem / (int64)i);
        out[i-L] = take * (int64)i;
        rem -= out[i-L];
        if (rem == 0) break;
    }
    if (rem != 0) return {};
    return out;
}

// Residue DP optimization: reduce problem by gcd/residue to manageable DP.
// We'll attempt DP on residues modulo m (smallest index in segment or gcd of indices).
// Returns chosen b-values (multiples of index) or empty.
vector<int64> residue_dp_fill(const vector<int64>& a, int L, int R, int64 S, double timeBudget, const Timer& globalTimer) {
    // choose modulus m = gcd of indices in [L..R] or just pick smallest index L
    int64 m = 0;
    for (int i=L;i<=R;i++) m = gcd64(m, (int64)i);
    if (m == 0) m = (int64)L;
    if (m > 256) m = (int64)L; // keep small modulus
    // DP over residues modulo m: keep best achievable sums for each residue using greedy-like large picks
    int len = R - L + 1;
    vector<int> vals(len);
    vector<int64> caps(len);
    for (int i=0;i<len;i++){ vals[i] = L + i; caps[i] = a[L+i-1] / (int64)(L+i); }
    // residue DP: for each residue r (0..m-1) keep map sum->vector<multipliers> trimmed to small size
    vector<unordered_map<int64, vector<int64>>> dp(2);
    dp[0].clear();
    dp[0][0] = vector<int64>(len, 0); // sum 0 achievable
    int cur = 0, nxt = 1;
    // iterate items large-first to bias toward big sums
    vector<int> order(len);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int p, int q){ return vals[p] > vals[q]; });
    const int KEEP_PER_RES = 32;
    for (int idx_order=0; idx_order<len; ++idx_order) {
        if (globalTimer.elapsed() > GLOBAL_TIME_LIMIT) return {};
        int i = order[idx_order];
        dp[nxt].clear();
        int v = vals[i];
        int64 capi = caps[i];
        for (auto &kv : dp[cur]) {
            int64 ssum = kv.first;
            const vector<int64>& mults = kv.second;
            // try k from 0..min(cap, (S-ssum)/v) but only a few top choices to limit branching
            int64 kmax = min(capi, max<int64>(0, (S - ssum) / v));
            int tries = 0;
            for (int64 k = kmax; k >= 0 && tries < 5; --k, ++tries) {
                int64 ns = ssum + k * v;
                if (ns > S) continue;
                // copy mults and set this idx
                vector<int64> nm = mults;
                nm[i] = k;
                // store ns
                // trim by residue and keep only top few sums
                auto it = dp[nxt].find(ns);
                if (it == dp[nxt].end()) dp[nxt][ns] = move(nm);
            }
        }
        // compress dp[nxt] keep only sums <= S and trim size
        if (dp[nxt].size() > 4096) {
            // keep top KEEP_PER_RES sums per residue
            vector<pair<int64, vector<int64>>> items;
            items.reserve(dp[nxt].size());
            for (auto &kv : dp[nxt]) if (kv.first <= S) items.emplace_back(kv.first, kv.second);
            sort(items.begin(), items.end(), [](auto &A, auto &B){ return A.first > B.first; });
            dp[nxt].clear();
            int take = min((int)items.size(), 4096);
            for (int t=0;t<take;t++) dp[nxt][items[t].first] = items[t].second;
        }
        swap(cur, nxt);
    }
    // now look for exact S
    auto it = dp[cur].find(S);
    if (it != dp[cur].end()) {
        vector<int64> mults = it->second;
        vector<int64> res(len,0);
        for (int i=0;i<len;i++) res[i] = mults[i] * (int64)vals[i];
        return res;
    }
    return {};
}

// ------------------------ attempt a single (p,q) ------------------------
// return b vector if found, else empty
vector<int64> try_cut_construct(const Precompute& P, vector<int64>& a, int p, int q, const Timer& globalTimer) {
    // enforce 1 <= p < q < n
    int n = P.n;
    if (!(1 <= p && p < q && q < n)) return {};
    double startTime = globalTimer.elapsed();
    Timer local;
    // compute segment maxima
    int L1=1,R1=p, L2=p+1,R2=q, L3=q+1,R3=n;
    int64 mx1 = P.segmentMax(L1,R1), mx2 = P.segmentMax(L2,R2), mx3 = P.segmentMax(L3,R3);
    if (mx1 == 0 || mx2 == 0 || mx3 == 0) return {};
    int64 g1 = gcd_range_indices(L1,R1), g2 = gcd_range_indices(L2,R2), g3 = gcd_range_indices(L3,R3);
    int64 l = g1;
    l = lcm64(l, g2); l = lcm64(l, g3);
    if (l <= 0) l = 1;
    int64 maxS = min({mx1, mx2, mx3});
    if (maxS <= 0) return {};
    // try S descending (multiples of l)
    int tries = 0;
    int64 S = (maxS / l) * l;
    while (S > 0 && tries < MAX_S_ATTEMPTS && globalTimer.elapsed() < GLOBAL_TIME_LIMIT) {
        if (local.elapsed() > PER_CUT_TIME) break;
        if (S > mx1 || S > mx2 || S > mx3) { S -= l; ++tries; continue; }
        // for each segment attempt fill (small segment exact else greedy+residue fallback)
        vector<int64> seg1, seg2, seg3;
        int len1 = R1-L1+1, len2 = R2-L2+1, len3 = R3-L3+1;
        bool fail=false;
        // seg1
        if (len1 <= 26) seg1 = solve_segment_small_exact(a, L1, R1, S, PER_CUT_TIME/3.0, globalTimer);
        else { seg1 = greedy_fill(a, L1, R1, S); if (seg1.empty()) seg1 = residue_dp_fill(a, L1, R1, S, PER_CUT_TIME/3.0, globalTimer); }
        if (seg1.empty()) { S -= l; ++tries; continue; }
        // seg2
        if (len2 <= 26) seg2 = solve_segment_small_exact(a, L2, R2, S, PER_CUT_TIME/3.0, globalTimer);
        else { seg2 = greedy_fill(a, L2, R2, S); if (seg2.empty()) seg2 = residue_dp_fill(a, L2, R2, S, PER_CUT_TIME/3.0, globalTimer); }
        if (seg2.empty()) { S -= l; ++tries; continue; }
        // seg3
        if (len3 <= 26) seg3 = solve_segment_small_exact(a, L3, R3, S, PER_CUT_TIME/3.0, globalTimer);
        else { seg3 = greedy_fill(a, L3, R3, S); if (seg3.empty()) seg3 = residue_dp_fill(a, L3, R3, S, PER_CUT_TIME/3.0, globalTimer); }
        if (seg3.empty()) { S -= l; ++tries; continue; }
        // verify all segments sums equal S (they do by construction), combine b
        vector<int64> b(n,0);
        for (int i=0;i<len1;i++) b[L1 + i -1] = seg1[i];
        for (int i=0;i<len2;i++) b[L2 + i -1] = seg2[i];
        for (int i=0;i<len3;i++) b[L3 + i -1] = seg3[i];
        // final validation
        bool ok=true;
        for (int i=0;i<n;i++) {
            if (b[i] < 0 || b[i] > a[i]) { ok=false; break; }
            if (b[i] % (int64)(i+1) != 0) { ok=false; break; }
        }
        if (!ok) { S -= l; ++tries; continue; }
        // success
        return b;
    }
    return {};
}

// Choose candidate p and q around thirds using prefix sums of a
pair<vector<int>, vector<int>> choose_candidate_pq(const Precompute& P) {
    int n = P.n;
    vector<pair<int64,int>> v;
    int64 total = P.prefA[n];
    int64 t1 = total / 3;
    int64 t2 = (2 * total) / 3;
    for (int i=1;i<n;i++) {
        v.emplace_back(llabs(P.prefA[i] - t1), i);
    }
    sort(v.begin(), v.end());
    vector<int> candP; for (int i=0;i<min((int)v.size(), MAX_CAND_P); ++i) candP.push_back(v[i].second);
    vector<pair<int64,int>> u;
    for (int i=1;i<n;i++) u.emplace_back(llabs(P.prefA[i] - t2), i);
    sort(u.begin(), u.end());
    vector<int> candQ; for (int i=0;i<min((int)u.size(), MAX_CAND_Q); ++i) candQ.push_back(u[i].second);
    return {candP, candQ};
}

// Top-level solve for one test case
// Returns sequence of operations (each op is vector<int64> b), or empty vector if failed
vector<vector<int64>> solve_one_case(vector<int64> a) {
    Timer globalTimer;
    int n = (int)a.size();
    vector<vector<int64>> operations;
    Precompute P(a);
    if (P.prefA[n] == 0) return operations;
    // limit number of operations
    for (int op=0; op<MAX_OPS && globalTimer.elapsed() < GLOBAL_TIME_LIMIT; ++op) {
        // recompute precompute for current a
        Precompute curP(a);
        auto [candP, candQ] = choose_candidate_pq(curP);
        bool progress = false;
        // attempt cross product of candidates (bounded)
        for (int pi=0; pi<(int)candP.size() && globalTimer.elapsed() < GLOBAL_TIME_LIMIT && !progress; ++pi) {
            for (int qi=0; qi<(int)candQ.size() && globalTimer.elapsed() < GLOBAL_TIME_LIMIT && !progress; ++qi) {
                int p = candP[pi], q = candQ[qi];
                if (!(1 <= p && p < q && q < n)) continue;
                // cheap prunes: segmentMax>0
                if (curP.segmentMax(1,p) == 0 || curP.segmentMax(p+1,q) == 0 || curP.segmentMax(q+1,n) == 0) continue;
                // try build b
                vector<int64> b = try_cut_construct(curP, a, p, q, globalTimer);
                if (!b.empty()) {
                    // apply
                    operations.push_back(b);
                    for (int i=0;i<n;i++) a[i] -= b[i];
                    progress = true;
                }
            }
        }
        if (!progress) {
            // fallback: try brute p,q scan for total%3 scenario
            int64 total = curP.prefA[n];
            if (total % 3 == 0) {
                int attempts = 0;
                int64 S = total/3;
                for (int p=1; p<n && attempts < 250 && globalTimer.elapsed() < GLOBAL_TIME_LIMIT && !progress; ++p) {
                    for (int q=p+1; q<n && attempts < 250 && globalTimer.elapsed() < GLOBAL_TIME_LIMIT && !progress; ++q) {
                        if (curP.segmentMax(1,p) < S || curP.segmentMax(p+1,q) < S || curP.segmentMax(q+1,n) < S) { ++attempts; continue; }
                        vector<int64> b = try_cut_construct(curP, a, p, q, globalTimer);
                        ++attempts;
                        if (!b.empty()) {
                            operations.push_back(b);
                            for (int i=0;i<n;i++) a[i] -= b[i];
                            progress = true;
                        }
                    }
                }
            }
        }
        if (!progress) break; // no possible next operation found
        // check if done
        bool allz = true;
        for (int i=0;i<n;i++) if (a[i] != 0) { allz = false; break; }
        if (allz) return operations;
    }
    // success?
    bool allz = true;
    for (int i=0;i<n;i++) if (a[i] != 0) { allz = false; break; }
    if (allz) return operations;
    // fail under resource limits
    return {};
}

// Simple verifier: check that ops transform initial array to zeros under rules
bool verify_solution(const vector<int64>& initial, const vector<vector<int64>>& ops) {
    int n = (int)initial.size();
    vector<int64> a = initial;
    if (ops.empty()) {
        for (int i=0;i<n;i++) if (a[i] != 0) return false;
        return true;
    }
    for (auto &b : ops) {
        if ((int)b.size() != n) return false;
        // check 0 <= b[i] <= a[i], b[i] % (i+1) == 0
        for (int i=0;i<n;i++) {
            if (b[i] < 0 || b[i] > a[i]) return false;
            if (b[i] % (int64)(i+1) != 0) return false;
        }
        // check existence of p,q where segment sums equal
        bool found=false;
        for (int p=1;p<n && !found;p++) for (int q=p+1; q<n && !found; q++) {
            int64 s1=0,s2=0,s3=0;
            for (int i=0;i<p;i++) s1 += b[i];
            for (int i=p;i<q;i++) s2 += b[i];
            for (int i=q;i<n;i++) s3 += b[i];
            if (s1==s2 && s2==s3) found=true;
        }
        if (!found) return false;
        for (int i=0;i<n;i++) a[i] -= b[i];
    }
    for (int i=0;i<n;i++) if (a[i] != 0) return false;
    return true;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Timer globalAll;
    int T; if (!(cin >> T)) return 0;
    while (T--) {
        int n; cin >> n;
        vector<int64> a(n);
        for (int i=0;i<n;i++) cin >> a[i];
        auto ops = solve_one_case(a);
        if (ops.empty()) {
            cout << -1 << "\n";
        } else {
            // verify before printing (safety)
            if (!verify_solution(a, ops)) {
                // If verification fails, print -1 to avoid wrong outputs
                cout << -1 << "\n";
            } else {
                cout << ops.size() << "\n";
                for (auto &b : ops) {
                    for (int i=0;i<n;i++) {
                        if (i) cout << ' ';
                        cout << b[i];
                    }
                    cout << "\n";
                }
            }
        }
    }
    return 0;
}
