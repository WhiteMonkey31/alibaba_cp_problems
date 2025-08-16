#include <iostream>
#include<vector>

using namespace std;
using ll = long long;


int gcd(int a, int b) {
    while (b != 0) {
        a %= b;
        std::swap(a, b);
    }
    return a;
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    if (!(cin >> t)) return 0;
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

        // Build list of all coprime pairs (i < j) with their product mod k.
        struct PairInfo { int i, j; int mod; };
        vector<PairInfo> cpairs;
        cpairs.reserve((size_t)n * (n - 1) / 2);

        // Precompute a[i] % k for faster product mod
        vector<int> amod(n);
        for (int i = 0; i < n; ++i) amod[i] = (int)((a[i] % k + k) % k);

        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                if (gcd(a[i], a[j]) == 1) {
                    int pm = (int)((1LL * amod[i] * amod[j]) % k);
                    cpairs.push_back({i, j, pm});
                }
            }
        }

        bool found = false;

        // Try all unordered pairs of coprime pairs; check disjointness and mod condition.
        // This is O(P^2) where P is number of coprime pairs.
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
    return 0;
}
