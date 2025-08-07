#include <iostream>
#include <vector>
using namespace std;

int main() {
    int t;
    cin >> t;  // Number of test cases
    for (int test = 0; test < t; ++test) {
        int n;
        cin >> n;  // Size of the permutation

        // Example: Ask a query
        cout << "1 " << n;
        for (int i = 1; i <= n; ++i) cout << " " << i;
        cout << endl;

        // Important! Flush the output
        cout.flush();

        // Read the response from the judge
        vector<int> response(min(60, n));
        for (int i = 0; i < response.size(); ++i) {
            cin >> response[i];
        }

        // Process the response...
        // then issue more queries, flush, etc.
    }

    return 0;
}
