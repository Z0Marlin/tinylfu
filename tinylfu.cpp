#include <stdexcept>
#include "bfilter.hpp"
#include "cache.hpp"

class TinyLFU {
private:
    uint32_t sample_size, counter;
    size_t size;
    CachePolicy *cache_policy;
    BloomFilter::BloomFilter *histogram;

public:
    TinyLFU(const size_t size=1e4, CachePolicy *policy=NULL, const uint32_t sample_size=100) {
        if (policy == NULL) {
            throw std::invalid_argument("No Cache policy Supplied");
        }   

        this->counter = 0;
        this->sample_size = sample_size;
        this->size = size;
        this->cache_policy = policy;
        this->histogram = new BloomFilter::BloomFilter(size);
    }

    void refresh() {
        if(counter == sample_size) {
            for(cacheobj_t i = 0; i < histogram->tsize(); i++) {
                histogram->setValue(i, histogram->getValue(i)>>2);
            }
            counter = 0;
        }
    }

    bool has_obj(cacheobj_t i){
        histogram->Add(i);
        return cache_policy->has_obj(i);
    }

    void add(cacheobj_t i){
        if (cache_policy->has_obj(i))
            return;
        
        if (cache_policy->is_full() == false) {
            cache_policy->add(i);
            
        }
        else {
            cacheobj_t victim = cache_policy->next_victim();
            if (histogram->Estimate(i) > histogram->Estimate(victim)) {
                cache_policy->evict();
                cache_policy->add(i);
            }               
        }
        counter++;
        refresh();
    }

    ~TinyLFU(){
        delete histogram;
    }
};

int main(int argc, char** argv){
    long int hits = 0, miss = 0, trials,csize;
    int mode;
    cacheobj_t o;
    scanf("%ld%ld%d", &trials, &csize, &mode);
    if(mode == 0){
        TinyLFU t(1000, new LRU(csize));
        for(int i = 0; i < trials; i++){
        scanf("%u", &o);
        if(t.has_obj(o))
            hits++;
        else{
            t.add(o);
            miss++;
        }
        }
    }
    else if(mode == 1){
        TinyLFU t(1000, new LFU(csize));
        for(int i = 0; i < trials; i++){
        scanf("%u", &o);
        if(t.has_obj(o))
            hits++;
        else{
            t.add(o);
            miss++;
        }
        }
    }
    else if(mode == 2){
        LRU t(csize);
        for(int i = 0; i < trials; i++){
        scanf("%u", &o);
        if(t.has_obj(o))
            hits++;
        else{
            t.add(o);
            miss++;
        }
        }
    }
    else{
        LFU t(csize);
        for(int i = 0; i < trials; i++){
        scanf("%u", &o);
        if(t.has_obj(o))
            hits++;
        else{
            t.add(o);
            miss++;
        }
        }
    }
    // TinyLFU t(10, new LRU(10));
    // trials = 10;
    // for(int i = 0; i < 10; i++)
    //     t.add(i);
    // for(int i = 0; i < 10; i++){
    //     if(t.has_obj(i))
    //         hits++;
    //     else{
    //         miss++;
    //     }
    // }
    double hit_ratio = static_cast<double>(hits)/static_cast<double>(trials);
    double miss_ratio = static_cast<double>(miss)/static_cast<double>(trials);
    printf("Hit ratio: %lf\n", hit_ratio);
    printf("Miss ratio: %lf\n", miss_ratio);
}
