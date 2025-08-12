// tournament_bias_spike.cpp
// Compile with: g++ -O2 -std=gnu++17 tournament_bias_spike.cpp -o spike

#include <iostream>
#include<vector>
#include<algorithm>
#include<set>
using namespace std;
using int64 = long long;

struct Fenwick {
    int n;
    vector<int64> bit;
    Fenwick() {}
    Fenwick(int n_): n(n_), bit(n_+1, 0) {}
    void add(int idx, int64 delta) {
        for (; idx <= n; idx += idx & -idx) bit[idx] += delta;
    }
    int64 sumPrefix(int idx) {
        int64 s = 0;
        for (; idx > 0; idx -= idx & -idx) s += bit[idx];
        return s;
    }
    int64 total() { return sumPrefix(n); }

    // find smallest index i such that prefix_sum(i) >= target
    // assumes all values are non-negative and target > 0 and total >= target
    int findByPrefix(int64 target) {
        int idx = 0;
        int LOG = 1;
        while ((1 << LOG) <= n) ++LOG;
        for (int k = LOG; k >= 0; --k) {
            int nxt = idx + (1 << k);
            if (nxt <= n && bit[nxt] < target) {
                target -= bit[nxt];
                idx = nxt;
            }
        }
        return idx + 1;
    }

    // find k-th (1-indexed) where fenwick stores counts (for distinct search)
    int findByCount(int64 k) { // same as findByPrefix but semantic
        return findByPrefix(k);
    }
};


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    if (!(cin >> t)) return 0;
    while (t--) {
        int n;
        int K, L;
        int P;
        cin >> n >> K >> L >> P;
        vector<int> a(n);
        for (int i = 0; i < n; ++i) cin >> a[i];

        // Coordinate compress values
        vector<int> vals = a;
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());
        int M = (int)vals.size();

        auto getId = [&](int x){
            return int(lower_bound(vals.begin(), vals.end(), x) - vals.begin()) + 1; // 1-based
        };

        // For each compressed id, we maintain multiset of indices currently in window
        vector< multiset<int> > idxSet(M+1);

        // Fenwicks:
        Fenwick fenwW(M); // sum of weights per distinct value (weights = ai % P), aggregated by compressed id
        Fenwick fenwD(M); // distinct present? 1 if present, 0 if not

        int64 best = LLONG_MIN;
        bool foundAny = false;

        // function to add a element at index i
        auto addIndex = [&](int i){
            int v = a[i];
            int id = getId(v);
            int64 w = v % P;
            bool wasPresent = !idxSet[id].empty();
            idxSet[id].insert(i);
            if (!wasPresent) {
                fenwD.add(id, 1); // now distinct appears
            }
            if (w != 0) fenwW.add(id, w); // add weight (can be zero)
            else {
                // still add zero doesn't change fenwW; okay skip
            }
        };

        // function to remove element at index i
        auto removeIndex = [&](int i){
            int v = a[i];
            int id = getId(v);
            int64 w = v % P;
            auto it = idxSet[id].find(i);
            if (it != idxSet[id].end()) idxSet[id].erase(it);
            bool nowPresent = !idxSet[id].empty();
            if (!nowPresent) fenwD.add(id, -1);
            if (w != 0) fenwW.add(id, -w);
        };

        // iterate over all lengths len in [K, L]
        // For each length, slide a window of that fixed size in O(n log M).
        for (int len = K; len <= L && len <= n; ++len) {
            // reset structures
            for (int id = 1; id <= M; ++id) {
                idxSet[id].clear();
            }
            fenwW = Fenwick(M);
            fenwD = Fenwick(M);

            // init first window [0 .. len-1]
            for (int i = 0; i < len; ++i) addIndex(i);

            // evaluate current window and all positions
            for (int start = 0; start + len <= n; ++start) {
                // check distinct count
                int64 distinctCount = fenwD.total();
                if (distinctCount >= 3) {
                    int64 totalW = fenwW.total();
                    if (totalW > 0) {
                        // find weighted median compressed id
                        // We want first prefix >= (totalW+1)/2  (ceil half)
                        int64 target = (totalW + 1) / 2;
                        int pos = fenwW.findByPrefix(target); // compressed id
                        int medianValue = vals[pos-1];

                        // third smallest distinct
                        int thirdId = fenwD.findByPrefix(3); // k=3
                        int thirdValue = vals[thirdId-1];

                        int64 score = (int64)medianValue - (int64)thirdValue;
                        if (!foundAny || score > best) best = score, foundAny = true;
                    } else {
                        // total weight 0 => target 0 => behavior ambiguous; skip window since weighted median not well-defined
                        // (or we could treat first element as median — but spec says weights can be zero; safest is to skip)
                    }
                }
                // slide to next window: remove start, add start+len
                if (start + len < n) {
                    removeIndex(start);
                    addIndex(start + len);
                }
            }
        }

        if (!foundAny) {
            // No valid window found
            cout << "IMPOSSIBLE\n"; // or per your desired output when none valid
        } else {
            cout << best << "\n";
        }
    }
    return 0;
}
