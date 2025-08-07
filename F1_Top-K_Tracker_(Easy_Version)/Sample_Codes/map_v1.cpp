#include<iostream>
#include<map>
int main(){
    std::map<std::string,int>mp;
    mp["apple"]=2;
    mp["banana"]=3;
    mp["cherry"]=19;
    for(auto& [key,value]:mp){
        std::cout<<key<<":"<<value<<'\n';
    }
    return 0;
}