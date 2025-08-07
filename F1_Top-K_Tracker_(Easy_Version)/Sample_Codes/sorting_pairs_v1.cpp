#include<iostream>
#include<algorithm>
#include<vector>
int main(){
    std::vector<std::pair<int,int>>vp{{1,3},{4,5},{2,4},{2,1}};
    sort(vp.begin(),vp.end());
    for(auto [x,y]:vp){
        std::cout<<"("<<x<<","<<y<<") ";
    }
    return 0;
}