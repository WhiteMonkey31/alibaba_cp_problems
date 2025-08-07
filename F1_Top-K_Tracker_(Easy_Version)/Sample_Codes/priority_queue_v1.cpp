#include<iostream>
#include<queue>
int main(){
    std::priority_queue<int>pq;
    std::priority_queue<int,std::vector<int>,std::greater<int>>mh;
    pq.push(5);
    pq.push(1);
    pq.push(10);

    while (!pq.empty()) {
        std::cout << pq.top() << " ";
        pq.pop();
    }
    std::cout<<std::endl;
    mh.push(5);
    mh.push(1);
    mh.push(10);

    while (!mh.empty()) {
        std::cout << mh.top() << " ";
        mh.pop();
    }
    return 0;
}