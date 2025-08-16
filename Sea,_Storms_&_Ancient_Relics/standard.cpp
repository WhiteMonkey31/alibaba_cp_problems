#include <iostream>
#include<vector>
#include<unordered_map>

using namespace std;
using ll = long long;

const int MAXM = 1000000; // safe upper bound for m

static int lpf[MAXM + 1];


int gcd(int a, int b) {
    while (b != 0) {
        a %= b;
        std::swap(a, b);
    }
    return a;
}


void build_lpf() {
    // least prime factor (SPF) sieve
    for (int i = 2; i <= MAXM; ++i) {
        if (lpf[i] == 0) {
            lpf[i] = i;
            if ((ll)i * i <= MAXM) {
                for (int j = i * i; j <= MAXM; j += i)
                    if (lpf[j] == 0) lpf[j] = i;
            }
        }
    }
    lpf[1] = 1;
}

vector<int> prime_factors_unique(int x) {
    vector<int> res;
    while (x > 1) {
        int p = lpf[x];
        if (p == 0) p = x;
        res.push_back(p);
        while (x % p == 0) x /= p;
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    build_lpf();

    int t;
    if (!(cin >> t)) return 0;
    while (t--) {
        int n, m, k;
        cin >> n >> m >> k;
        vector<int> a(n);
        for (int i = 0; i < n; ++i) cin >> a[i];

        // frequency and positions
        vector<int> freq(m + 1, 0);
        vector<vector<int>> pos(m + 1);
        for (int i = 0; i < n; ++i) {
            if (a[i] <= m) {
                freq[a[i]]++;
                pos[a[i]].push_back(i + 1); // 1-based
            }
        }

        // countMultiples[d] = number of elements divisible by d
        vector<int> countMultiples(m + 1, 0);
        for (int d = 1; d <= m; ++d) {
            for (int mult = d; mult <= m; mult += d) {
                if (freq[mult]) countMultiples[d] += freq[mult];
            }
        }

        // distinct present values
        vector<int> values;
        values.reserve(m);
        for (int v = 1; v <= m; ++v) if (freq[v] > 0) values.push_back(v);

        unordered_map<int, vector<int>> pf_map;
        pf_map.reserve(values.size() * 2);
        for (int v : values) pf_map[v] = prime_factors_unique(v);

        // Collect candidate pairs (p_idx, q_idx). Use up to two p-candidates per value to increase coverage.
        vector<pair<int,int>> pairs;
        pairs.reserve(3000);
        const int MAX_PAIRS_KEEP = 3000;

        for (int v : values) {
            // compute how many elements share any prime with v via inclusion-exclusion
            vector<int> primes = pf_map[v];
            int kpr = (int)primes.size();
            int totalBad = 0;
            if (kpr > 0) {
                int subsets = 1 << kpr;
                for (int mask = 1; mask < subsets; ++mask) {
                    ll prod = 1;
                    int bits = 0;
                    bool overflow = false;
                    for (int b = 0; b < kpr; ++b) if (mask & (1 << b)) {
                        ++bits;
                        prod *= primes[b];
                        if (prod > m) { overflow = true; break; }
                    }
                    if (overflow) continue;
                    int cnt = countMultiples[(int)prod];
                    if (bits & 1) totalBad += cnt;
                    else totalBad -= cnt;
                }
            }
            int coprimeCount = n - totalBad;
            if (coprimeCount <= 0) continue;

            // We'll attempt up to two different p_idx choices (if available) to increase variety
            int maxPChoices = min((int)pos[v].size(), 2);
            for (int pi = 0; pi < maxPChoices && (int)pairs.size() < MAX_PAIRS_KEEP; ++pi) {
                int p_idx = pos[v][pi];
                int q_idx = -1;

                // try same value if v==1 and there's another occurrence
                if (v == 1 && pos[v].size() >= 2) {
                    q_idx = (pos[v][0] != p_idx ? pos[v][0] : pos[v][1]);
                } else {
                    // scan other distinct values w and try both their first/second indices
                    for (int w : values) {
                        if (w == v) continue;
                        if (gcd(v, w) != 1) continue;
                        // try a couple of positions of w to increase disjointness chance
                        for (int pick = 0; pick < (int)pos[w].size() && pick < 2; ++pick) {
                            int candidate_q = pos[w][pick];
                            if (candidate_q != p_idx) { q_idx = candidate_q; break; }
                        }
                        if (q_idx != -1) break;
                    }
                }

                // fallback: full scan over indices if still not found
                if (q_idx == -1) {
                    for (int i = 0; i < n; ++i) {
                        int idx = i + 1;
                        if (idx == p_idx) continue;
                        if (gcd(v, a[i]) == 1) { q_idx = idx; break; }
                    }
                }

                if (q_idx == -1 || q_idx == p_idx) continue;
                pairs.emplace_back(p_idx, q_idx);
            }
            if ((int)pairs.size() >= MAX_PAIRS_KEEP) break;
        }

        // Now search for two disjoint pairs satisfying (a[p]*a[q] + a[r]*a[s]) % k != 0
        bool found = false;
        int P = (int)pairs.size();
        for (int i = 0; i < P && !found; ++i) {
            for (int j = i + 1; j < P && !found; ++j) {
                int p = pairs[i].first, q = pairs[i].second;
                int r = pairs[j].first, s = pairs[j].second;
                if (p == r || p == s || q == r || q == s) continue;
                ll prod1 = (ll)a[p-1] * (ll)a[q-1];
                ll prod2 = (ll)a[r-1] * (ll)a[s-1];
                int modsum = (int)(((prod1 % k) + (prod2 % k)) % k);
                if (modsum != 0) {
                    cout << p << ' ' << q << ' ' << r << ' ' << s << '\n';
                    found = true;
                }
            }
        }

        // special-case many ones
        if (!found && pos.size() > 1 && pos[1].size() >= 4) {
            int p = pos[1][0], q = pos[1][1], r = pos[1][2], s = pos[1][3];
            ll prod1 = 1LL * a[p-1] * a[q-1];
            ll prod2 = 1LL * a[r-1] * a[s-1];
            if ((((prod1 % k) + (prod2 % k)) % k) != 0) {
                cout << p << ' ' << q << ' ' << r << ' ' << s << '\n';
                found = true;
            }
        }

        if (!found) cout << 0 << '\n';
    }
    return 0;
}
