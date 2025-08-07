#include<iostream>
#include<vector>
int main(){
    std::vector<int> coins = {1, 3, 4};
    int x = 6;

    for (std::size_t i = coins.size() - 1; i-- > 0;) {
        while (x >= coins[i]) {
            std::cout << coins[i] << " ";
            x -= coins[i];
        }
    }

    return 0;
}

