#include<iostream>
#include<bitset>
int main(){
    std::bitset<8>bs("10110010");
    std::cout<<"Original: "<<bs<<std::endl;
    bs.flip();
    std::cout<<"Flipped: "<<bs<<std::endl;
    bs.set(4);
    std::cout<<"After setting bit 3: "<<bs<<std::endl;
    std::cout<<"NUmber of 1s: "<<bs.count()<<std::endl;
    if(bs.test(4)){
        std::cout<<"Bit 4 is set.\n";
    }
    return 0;
}