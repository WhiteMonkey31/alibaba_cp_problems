#include<iostream>
#include<algorithm>
#include<vector>
int main(){
    std::vector<int> nums{1,4,7,9,3,5};
    sort(nums.begin(),nums.end());
    for(int &x:nums){
        std::cout<<x<<" ";
    }
    std::cout<<std::endl;
    sort(nums.begin(),nums.end(),std::greater<int>());
    for(int x:nums){
        std::cout<<x<<" ";
    }
    return 0;
}