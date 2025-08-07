#include<iostream>
#include<vector>
#include<algorithm>
int main(){
    std::vector<int>nums{1,3,5,5,7,9};
    if(std::binary_search(nums.begin(),nums.end(), 7)){
        std::cout<<"4 is found.\n";
    }else{
        std::cout<<"4 is not Found!.\n";
    }
    auto it{std::lower_bound(nums.begin(),nums.end(),5)};
    auto its{std::upper_bound(nums.begin(),nums.end(),5)};
    int index=static_cast<int>(it-nums.begin());
    std::cout<<index<<std::endl;;

    std::cout << "First index >= " << 5 << ": " << (it - nums.begin()) << "\n";
    std::cout << "First index > "  << 5 << ": " << (its - nums.begin()) << "\n";
    std::cout << "Count of " << 5 << ": " << (its - it) << "\n";

    return 0;
}