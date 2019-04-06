#include "bfilter.hpp"

int main(){
    int nk = 10000;
    BloomFilter::BloomFilter bf(nk);
    std::cout << bf.tsize() << std::endl;
    std::cout << bf.hcount() << std::endl;
    bf.show_table();
    bf.Add(20);
    bf.show_table();
}