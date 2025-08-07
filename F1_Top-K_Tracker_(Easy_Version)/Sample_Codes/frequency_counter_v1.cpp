#include<iostream>
#include<unordered_map>
#include<vector>
int main(){
    std::vector<int>arr{1,2,2,3,3,3};
    std::unordered_map<int,int>freq;
    for(int x:arr){
        freq[x]++;
    }
    for(auto& [count,value]:freq){
        std::cout<<value<<" appears "<<count<<" times.\n";
    }
    return 0;
}