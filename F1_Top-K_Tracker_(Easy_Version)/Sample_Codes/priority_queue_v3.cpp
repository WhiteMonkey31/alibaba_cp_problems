#include<iostream>
#include<queue>
int main(){
    std::priority_queue<std::pair<int,int>>pq;
    pq.push({3,100});
    pq.push({1,500});
    pq.push({2,300});
    while(!pq.empty()){
        std::cout<<pq.top().first<<" "<<pq.top().second<<std::endl;
        pq.pop();
    }
    return 0;
}