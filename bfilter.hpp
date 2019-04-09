#ifndef BFILTER_HPP_
#define BFILTER_HPP_
#include<bits/stdc++.h>

namespace BloomFilter {
    typedef uint32_t tablekey_t;
    typedef uint64_t hashkey_t;
    typedef std::vector<hashkey_t> hashkeys_t;
    typedef uint8_t bfreq_t;

    class BloomFilter{
    private:
        const uint32_t hash_count;
        const uint32_t table_size;
        static const uint32_t BloomFilterSizeRatio = 25;
        static const size_t BloomFilterMinSize = 25000;
        static const size_t BloomFilterMaxSize = 3*1024*1024;
        std::vector<bfreq_t> count_table;
        hashkeys_t hkeys;
        static size_t CalFilterSize(size_t);
        static size_t CalHashCount(size_t);
    public:
        BloomFilter(size_t key_count);
        void Add(tablekey_t);
        bfreq_t Estimate(tablekey_t);
        
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
        void setValue(uint32_t ind, bfreq_t val){
            if(ind >= 0 && ind < table_size)
                count_table[ind] = val;
        }
        bfreq_t getValue(uint32_t ind){
            if(ind >= 0 && ind < table_size)
                return count_table[ind];
            throw std::out_of_range("Error! Index out of range");
        }
    };

    uint32_t HashMix(hashkey_t hkey, tablekey_t c){
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

void BloomFilter::BloomFilter::Add(tablekey_t k){
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

BloomFilter::bfreq_t BloomFilter::BloomFilter::Estimate(tablekey_t k){
    bfreq_t min_count, c;
    uint32_t i;
    min_count = count_table[HashMix(hkeys[0], k)%table_size];
    for(i = 1; i < hash_count; i++){
        c = count_table[HashMix(hkeys[i], k)%table_size];
        if(c < min_count)
            min_count = c;
    }
    return min_count;
}

size_t BloomFilter::BloomFilter::CalFilterSize(size_t key_count){
    size_t default_min = BloomFilter::BloomFilterMinSize;
    size_t number_of_keys = std::max(key_count, default_min);
    return static_cast<uint32_t>(std::min(number_of_keys * BloomFilter::BloomFilterSizeRatio
                                , BloomFilter::BloomFilterMaxSize * 8));
}

size_t BloomFilter::BloomFilter::CalHashCount(size_t key_count){
    return static_cast<uint32_t>(0.693*CalFilterSize(key_count)/key_count);
}

BloomFilter::BloomFilter::BloomFilter(size_t key_count): table_size(CalFilterSize(key_count)) 
                                                    , hash_count(CalHashCount(key_count)){
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
