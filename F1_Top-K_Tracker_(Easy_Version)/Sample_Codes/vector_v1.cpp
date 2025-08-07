#include<iostream>
#include<vector>
int main(){
    std::vector<int>nums;
    nums.push_back(3);
    nums.push_back(7);
    std::cout<<nums[0]<<" ";
    std::cout<<nums[1]<<" \n";
    std::cout<<nums.size()<<" ";
    nums.pop_back();
    std::cout<<nums.size()<<" ";
    nums.clear();
    std::cout<<nums.size()<<" ";
    return 0;


}