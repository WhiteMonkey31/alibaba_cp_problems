#include<iostream>
#include<set>
int main(){
    std::set<int>s;
    s.insert(3);
    s.insert(1);
    s.insert(2);
    s.insert(3);
    for(int x:s){
        std::cout<<x<<" ";
    }
    std::cout<<std::endl;

    std::multiset<int>ms;
    ms.insert(3);
    ms.insert(1);
    ms.insert(2);
    ms.insert(3);
    for(int x:ms){
        std::cout<<x<<" ";
    }
    std::cout<<std::endl;


    return 0;
}