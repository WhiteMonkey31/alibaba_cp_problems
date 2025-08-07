#include<iostream>
#include<vector>
#include<algorithm>
int main(){
    std::vector<int>vp{2,3,6,9,5,1,7};
    std::sort(vp.begin(),vp.end(),[](int a,int b){
        return a>b;
    });
    for (int x : vp) std::cout << x << " ";
    std::cout << "\n";
    return 0;
}