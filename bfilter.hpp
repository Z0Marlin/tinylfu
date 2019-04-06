#ifndef BFILTER_HPP_
#define BFILTER_HPP_
#include<bits/stdc++.h>
namespace BloomFilter
{
    typedef uint32_t tabledata_t;
    typedef uint64_t hashkey_t;
    typedef std::vector<hashkey_t> hashkeys_t;
    typedef uint8_t bfreq_t;
    class BloomFilter{
    private:
        const uint32_t hash_count;
        const uint32_t table_size;
        // std::vector<bfreq_t> count_table;
        // hashkeys_t hkeys;
        static const uint32_t BloomFilterSizeRatio = 25;
        static const uint32_t BloomFilterMinSize = 25000;
        static const uint32_t BloomFilterMaxSize = 3*1024*1024;
        static uint32_t CalFilterSize(unsigned int);
        static uint32_t CalHashCount(unsigned int);
    public:
        std::vector<bfreq_t> count_table;
        hashkeys_t hkeys;
        BloomFilter(int key_count);
        void Add(tabledata_t);
        bfreq_t Estimate(tabledata_t);
        constexpr uint32_t hcount(){
            return hash_count;
        }
        constexpr uint32_t tsize(){
            return table_size;
        }
        void show_table(){
            for(uint32_t i = 0; i < table_size; i++){
                if(count_table[i]) printf("%u ",i);
            }
            printf("\n");
        }
    };

    uint32_t HashMix(hashkey_t hkey, tabledata_t c){
        /* 
        * Hash Function
        * Credits: adobe/chromium project
        * Github repo: https://github.com/adobe/chromium
        */

        uint32_t a = static_cast<uint32_t>(hkey) & 0xFFFFFFFF;
        uint32_t b = static_cast<uint32_t>(hkey >> 32) & 0xFFFFFFFF;
        
        a -= (b + c);  a ^= (c >> 13);
        b -= (c + a);  b ^= (a << 8);
        c -= (a + b);  c ^= (b >> 13);
        a -= (b + c);  a ^= (c >> 12);
        b -= (c + a);  b ^= (a << 16);
        c -= (a + b);  c ^= (b >> 5);
        a -= (b + c);  a ^= (c >> 3);
        b -= (c + a);  b ^= (a << 10);
        c -= (a + b);  c ^= (b >> 15);

        return c;
    }
} // BloomFilter

void BloomFilter::BloomFilter::Add(tabledata_t k){
    uint32_t index[hash_count], i;
    bfreq_t min_count; 
    for(i = 0; i < hash_count; i++)
        index[i] = HashMix(hkeys[i], k)%table_size;
    min_count = count_table[index[0]];
    for(i = 1; i < hash_count; i++)
        if(count_table[index[i]] < min_count)
            min_count = count_table[index[i]];
    for(i = 0; i < hash_count; i++)
        ((count_table[index[i]]==min_count) && (count_table[index[i]]++));
}   

BloomFilter::bfreq_t BloomFilter::BloomFilter::Estimate(tabledata_t k){
    bfreq_t min_count, c;
    uint32_t i;
    min_count = count_table[HashMix(hkeys[0], k)];
    for(i = 1; i < hash_count; i++){
        c = count_table[HashMix(hkeys[i], k)%table_size];
        if(c < min_count)
            min_count = c;
    }
    return min_count;
}

uint32_t BloomFilter::BloomFilter::CalFilterSize(unsigned int key_count){
    unsigned int default_min = BloomFilter::BloomFilterMinSize;
    unsigned int number_of_keys = std::max(key_count, default_min);
    return static_cast<uint32_t>(std::min(number_of_keys * BloomFilter::BloomFilterSizeRatio
                                , BloomFilter::BloomFilterMaxSize * 8));
}

uint32_t BloomFilter::BloomFilter::CalHashCount(unsigned int key_count){
    return static_cast<uint32_t>(0.693*CalFilterSize(key_count)/key_count);
}

BloomFilter::BloomFilter::BloomFilter(int key_count): table_size(CalFilterSize(key_count)) 
                                                    , hash_count(CalHashCount(key_count)){
    printf("tsize: %u, hsize: %u\n", table_size, hash_count);
    uint32_t i;
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dist;
    for(i = 0; i < table_size; i++)
        count_table.push_back(0);
    for(i = 0; i < hash_count; i++){
        hkeys.push_back(dist(gen));
    }
}
#endif
