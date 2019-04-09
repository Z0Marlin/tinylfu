#include <stdexcept>
#include"bfilter.hpp"
#include "cache.hpp"
#include "lfu.hpp"

typedef uint32_t cacheobj_t;

class TinyLFU {
private:
    uint32_t steps, size;
    CachePolicy *cache_policy;
    BloomFilter::BloomFilter * histogram;

public:
    TinyLFU(uint32_t size=1e3, Cache * policy=NULL, uint32_t steps=10) {
        if (policy == NULL) {
            throw std::invalid_argument("No Cache policy Supplied");
        }   

        this->steps = steps;
        this->size = size;
        this->cache_policy = policy;
        this->histogram = new BloomFilter::BloomFilter(size);
    }

    void add(cacheobj_t i) {
        if (cache_policy->has_obj(i))
            return;
        
        if (cache_policy->is_full() == false) {
            cache_policy->add(i);
            histogram->Add(i);  
        }
        else {
            cacheobj_t victim = cache_policy->next_victim();
            if (histogram->Estimate(i) > histogram->Estimate(victim)) {
                histogram->Add(i);
                cache_policy->evict();
                cache_policy->add(i);
            }               
        }
    }
}; 

int main() {
    TinyLFU *tf = new TinyLFU(1e3, new LFU(1e3));
    for(cacheobj_t i = 1e3; i <= 1e3+300; i++)
        tf->add(i);
    return 0;
}