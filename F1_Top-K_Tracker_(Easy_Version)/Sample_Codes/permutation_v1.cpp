#include<iostream>
#include<algorithm>
#include<vector>
int main(){
    std::vector<int> nums={1,2,3};
    std::cout<<"ALl Permutations of {1,2,3}: \n";
    sort(nums.begin(),nums.end());
    do{
        for(int x:nums){
           std::cout<<x<<" ";
        }
        std::cout<<std::endl;
    }while(next_permutation(nums.begin(),nums.end()));
    return 0;
}