#include<iostream>
#include<unordered_map>
int main(){
    std::unordered_map<int,std::string> mp;
    mp[1]="one";
    mp[2]="two";
    mp[3]="three";
    for(auto& [key,value]:mp){
        std::cout<<key<<":"<<value<<'\n';
    }
    return 0;
}