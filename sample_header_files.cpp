
#include<vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <vector>
#include<numeric>
#include<array>
#include<unordered_map>
#include<functional>
#include<utility>
#include <cmath>
#include <algorithm>
#include<unordered_set>
#include<random>



int gcd(int a, int b) {
    while (b != 0) {
        a %= b;
        std::swap(a, b);
    }
    return a;
}
