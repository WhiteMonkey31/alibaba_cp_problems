#include<iostream>
int main(){
    std::cout<<"5\n";
    std::cout.flush();
    int x{};
    std::cin>>x;
    std::cout<<"Judge replied: "<<x*2<<std::endl;
    // cout << q * 2 << endl;  // reply with double
    std::cout.flush(); 
    return 0;
}