#include <map>
#include <utility>
#include <set>
#include "cache.hpp"

class LFU : public Cache{
private:
    uint32_t capacity;
    uint32_t _minFrequent;
    
    std::map<uint32_t,std::pair<uint32_t, uint32_t> > cache; // store key -> {value, frequent}
    std::map<uint32_t,std::list<uint32_t> > fList; // frequent -> list<uint32_t> keys
    std::map<uint32_t,std::list<uint32_t>::iterator> kIterTable; // key -> list::iterator
    
public:
    LFU(uint32_t capacity) {
        this->capacity = capacity;
    }
    
    uint32_t get(uint32_t key) {
        if(!cache.count(key)) return -1;
        
        fList[cache[key].second].erase(kIterTable[key]); // remove
        cache[key].second++;
        fList[cache[key].second].push_back(key);
        kIterTable[key] = --fList[cache[key].second].end();
        
        if(fList[_minFrequent].size() < 1){
            _minFrequent = cache[key].second;
        }

        return cache[key].first;
    }
    bool is_full() {
      return cache.size() == capacity;
    }
    cacheobj_t evict() {
      if(cache.size() == capacity){
            uint32_t evictedKey = fList[_minFrequent].front();
            fList[_minFrequent].pop_front();
            kIterTable.erase(evictedKey);
            cache.erase(evictedKey);
        }
        
    }    
    void set(uint32_t key, uint32_t value) {
        if(capacity <= 0) return ;
        
        // update old new refresh
        if(get(key) != -1){
            cache[key].first = value;
            return ;
        }
        
        if (is_full()) 
          evict();
        
        _minFrequent = 1;
        fList[_minFrequent].push_back(key);
        kIterTable[key] = --fList[_minFrequent].end();
        cache[key] = {value,_minFrequent};
    
    }
    void add(uint32_t item) {
        set(item, item);
    }

    bool has_obj(uint32_t item) {
      return get(item) == -1;
    }

    cacheobj_t next_victim() {
        return fList[_minFrequent].front();
    }
};
