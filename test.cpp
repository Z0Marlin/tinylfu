#include "bfilter.hpp"

int main(){
    int nk = 10000;
    BloomFilter::BloomFilter bf(nk);
    std::cout << bf.tsize() << std::endl;
    std::cout << bf.hcount() << std::endl;
    bf.show_table();
    for(int i = 12; i < 27; i++) bf.Add(i);
    for(int i = 12; i < 27; i++) bf.Add(i);
    for(int i = 12; i < 27; i++) bf.Add(i);

    for(int i = 12; i < 27; i++) printf("ret %u\n\n", bf.Estimate(i));
    printf("ret %u\n\n", bf.Estimate(1291));
    printf("ret %u\n\n", bf.Estimate(99));
    bf.show_table();
}