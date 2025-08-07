#include<iostream>
#include<algorithm>
#include<vector>
int main(){
    std::vector<int> nums{1,2,3,6,8,0,4,7};
    std::cout<<"Permutation for {1,2,3}: \n";
    sort(nums.begin(),nums.end());
    do{
        for(int x:nums){
            std::cout<<x<<" ";
        }
        std::cout<<std::endl;
    }while(next_permutation(nums.begin(),nums.end()));
    std::cout<<"Done.";
    return 0;
}