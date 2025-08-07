#include <iostream>
using namespace std;

int main() {
    int low = 1, high = 100;
    while (low <= high) {
        int mid = (low + high) / 2;
        cout << mid << endl;      // Make a guess
        cout.flush();             // VERY IMPORTANT!

        string response;
        cin >> response;          // Read judge's reply

        if (response == "correct") break;
        else if (response == "lower") high = mid - 1;
        else if (response == "higher") low = mid + 1;
    }
    return 0;
}
