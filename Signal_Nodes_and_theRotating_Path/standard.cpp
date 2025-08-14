#include <iostream>
#include<vector>
#include<algorithm>
#include<queue>
#include<cassert>

// #include <bits/stdc++.h>
using namespace std;

struct State {
    long long time, wait;
    int node, phase;
    bool operator>(const State& other) const {
        if (time != other.time) return time > other.time;
        return wait > other.wait;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T; // number of test cases
    cin >> T;

    while (T--) {
        int n, m;
        cin >> n >> m;

        vector<vector<int>> adj(n);
        vector<int> shift(n);

        // read shift array
        for (int i = 0; i < n; i++) cin >> shift[i];

        // read edges
        for (int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            --u; --v; // convert to 0-based
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        

        const int INF = 1e9;
        vector<vector<pair<long long,long long>>> dist(n);
        for (int i = 0; i < n; i++) {
            int degU = (int)adj[i].size();
            // dist[i].assign( degU, {LLONG_MAX, LLONG_MAX});
            dist[i].assign(max(1, degU), {LLONG_MAX, LLONG_MAX});

        }

        priority_queue<State, vector<State>, greater<State>> pq;
        dist[0][0] = {0, 0};
        pq.push({0, 0, 0, 0}); // start at node 0, time=0, wait=0, phase=0

        while (!pq.empty()) {
            auto [time, wait, u, phase] = pq.top(); pq.pop();

            if (make_pair(time, wait) != dist[u][phase]) continue;

            int degU = (int)adj[u].size();
            if (degU == 0) continue;

            // MOVE step with automatic shift normalization
            int shiftVal = shift[u];
            // if (shiftVal >= 1 && shiftVal <= degU) shiftVal -= 1; // convert 1-based to 0-based   //(but problem allows this 0-based shift)
            int forcedIndex = (time + shiftVal) % degU;
            int v = adj[u][forcedIndex];
            int degV = (int)adj[v].size();
            int newPhaseV = (time + 1) % degV;
            pair<long long,long long> candMove = {time + 1, wait};
            if (candMove < dist[v][newPhaseV]) {
                dist[v][newPhaseV] = candMove;
                pq.push({time + 1, wait, v, newPhaseV});
            }

            // WAIT step
            int newPhaseWait = (phase + 1) % degU;
            pair<long long,long long> candWait = {time + 1, wait + 1};
            if (candWait < dist[u][newPhaseWait]) {
                dist[u][newPhaseWait] = candWait;
                pq.push({time + 1, wait + 1, u, newPhaseWait});
            }
        }

        // find best answer for node n-1
        pair<long long,long long> ans = {LLONG_MAX, LLONG_MAX};
        for (auto &p : dist[n-1]) ans = min(ans, p);

        if (ans.first == LLONG_MAX) cout << "-1\n";
        else cout << ans.first << " " << ans.second << "\n";
    }

    return 0;
}
