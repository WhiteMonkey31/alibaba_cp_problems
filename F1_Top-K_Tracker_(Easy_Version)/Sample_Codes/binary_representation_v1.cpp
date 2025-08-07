#include <iostream>
#include <bitset>
using namespace std;

int main() {
    int number = 13;

    // Method 1: Manual conversion (printing)
    while (number > 0) {
        cout << (number % 2);
        number /= 2;
    }
    cout << endl;

    // Method 2: Using bitset (better!)
    bitset<8> b(13);  // 8-bit binary representation
    cout << "Binary of 13 is: " << b << endl;

    return 0;
}
