#include<iostream>
#include<vector>
int main(){
    int n{};
    std::cin>>n;
    std::vector<int>v(static_cast<std::size_t>(n));
    for(int& x:v){
        std::cin>>x;
    }
    for(int& x: v){
        std::cout<<x<<" ";
    }
    std::cout<<std::endl;
    return 0;
}