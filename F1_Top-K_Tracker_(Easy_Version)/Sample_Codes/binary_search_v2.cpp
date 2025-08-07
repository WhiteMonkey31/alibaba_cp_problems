#include <iostream>
#include <vector>
using namespace std;

int binary_search(const vector<int>& arr, int target) {
    int left = 0, right = arr.size() - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (arr[mid] == target) return mid;
        else if (arr[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    
    return -1;  // Not found
}

int main() {
    vector<int> arr = {2, 4, 6, 8, 10, 12, 14};
    int target = 10;
    
    int index = binary_search(arr, target);
    if (index != -1)
        cout << "Found at index: " << index << "\n";
    else
        cout << "Not found\n";
    return 0;
}
