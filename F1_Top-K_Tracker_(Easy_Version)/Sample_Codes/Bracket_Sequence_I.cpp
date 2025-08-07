// Baacket Sequence 1


#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

const int MOD = 1e9 + 7;
const int MAX = 2e6 + 5;

vector<ll> fact(MAX), inv_fact(MAX);

// Fast exponentiation
ll mod_pow(ll a, ll b) {
    ll res = 1;
    while (b > 0) {
        if (b & 1) res = res * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}

// Precompute factorials and inverse factorials
void precompute() {
    fact[0] = 1;
    for (int i = 1; i < MAX; ++i)
        fact[i] = fact[i - 1] * i % MOD;
    inv_fact[MAX - 1] = mod_pow(fact[MAX - 1], MOD - 2);
    for (int i = MAX - 2; i >= 0; --i)
        inv_fact[i] = inv_fact[i + 1] * (i + 1) % MOD;
}

// Compute C(n, k)
ll binomial(int n, int k) {
    if (k < 0 || k > n) return 0;
    return fact[n] * inv_fact[k] % MOD * inv_fact[n - k] % MOD;
}

int main() {
    int n;
    cin >> n;
    
    if (n % 2 == 1) {
        cout << 0 << endl;
        return 0;
    }

    precompute();

    int k = n / 2;
    ll result = binomial(2 * k, k) * mod_pow(k + 1, MOD - 2) % MOD;
    cout << result << endl;

    return 0;
}
