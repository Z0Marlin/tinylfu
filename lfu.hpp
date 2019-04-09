#include <map>
#include <utility>
#include <set>
#include "cache.hpp"

class LFU : public Cache{
public:
    LFUCache(uint32_t capacity) {
        _capcaity = capacity;
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
        cout<<cache[key].first<<endl;
        return cache[key].first;
    }
    bool is_full() {
      return cache.size() == capacity;
    }
    void evict() {
      if(cache.size() == _capcaity){
            uint32_t evictedKey = fList[_minFrequent].front();
            fList[_minFrequent].pop_front();
            kIterTable.erase(evictedKey);
            cache.erase(evictedKey);
        }
        
    }    
    void set(uint32_t key, uint32_t value) {
        if(_capcaity <= 0) return ;
        
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
    void has_obj(uuint32_t32_t item) {
      return get(item) == -1;
    }

private:
    uint32_t _capcaity;
    uint32_t _minFrequent;
    
    map<uint32_t,pair<uint32_t, uint32_t>> cache; // store key -> {value, frequent}
    map<uint32_t,list<uint32_t>> fList; // frequent -> list<uint32_t> keys
    map<uint32_t,list<uint32_t>::iterator> kIterTable; // key -> list::iterator
};

// int main(){
    
//     LFUCache cache(2);
    
//     cache.set(1, 1);
//     cache.set(2, 2);
//     cache.get(1);       // returns 1
//     cache.set(3, 3);    // evicts key 2
//     cache.get(2);       // returns -1 (not found)
//     cache.get(3);       // returns 3.
//     cache.set(4, 4);    // evicts key 1.
//     cache.get(1);       // returns -1 (not found)
//     cache.get(3);       // returns 3
//     cache.get(4);       // returns 4
    
    
//     return 0;
// }