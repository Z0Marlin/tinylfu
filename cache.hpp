#ifndef CACHEPOLICY_HPP_
#define CACHEPOLICY_HPP_
#include <bits/stdc++.h>
typedef uint32_t cacheobj_t;

class CachePolicy{
public:
    virtual void add(cacheobj_t) = 0;
    virtual bool is_full() = 0;
    virtual bool has_obj(cacheobj_t) = 0;
    virtual cacheobj_t evict() = 0;
    virtual cacheobj_t next_victim() = 0;
};

class LRU: public CachePolicy{
private:
	size_t cache_size;
	std::list<cacheobj_t> cache_list;
    std::unordered_map<cacheobj_t, std::list<cacheobj_t>::iterator> ma;
public:
    LRU(size_t);
    void display();
	bool is_full();
	bool has_obj(cacheobj_t);
	bool is_empty();
	void  add(cacheobj_t);
	cacheobj_t evict();
	cacheobj_t next_victim();
}; 

LRU::LRU(size_t size){ 
	LRU::cache_size = size; 
} 


bool LRU::is_full(){
	return (LRU::cache_list.size() == LRU::cache_size);
} 

bool LRU::has_obj(cacheobj_t x){
    return !(LRU::ma.find(x) == LRU::ma.end());
}

bool LRU::is_empty(){
    return LRU::cache_list.empty();
} 

void LRU::add(cacheobj_t x){ 
	if (!LRU::has_obj(x)){ 
		if (LRU::is_full()) { 
			cacheobj_t last=LRU::evict();
		} 
	} 
	else{
		LRU::cache_list.erase(LRU::ma[x]);
	} 
	LRU::cache_list.push_front(x); 
	LRU::ma[x] = LRU::cache_list.begin(); 
}


cacheobj_t LRU::next_victim(){
    return LRU::cache_list.back();
}

cacheobj_t LRU::evict(){
    cacheobj_t last = LRU::next_victim(); 
	LRU::cache_list.pop_back(); 
	LRU::ma.erase(last); 
	return last;
} 

void LRU::display(){ 
    for (auto it = LRU::cache_list.begin(); it != LRU::cache_list.end();it++) 
        std::cout << (*it) << " "; 
    std::cout << std::endl;
}

class LFU : public CachePolicy{
private:
    size_t capacity;
    uint32_t _minFrequent;
    std::map<cacheobj_t,std::pair<cacheobj_t, uint32_t> > cache; // store key -> {value, frequent}
    std::map<uint32_t,std::list<uint32_t> > fList; // frequent -> list<uint32_t> keys
    std::map<uint32_t,std::list<uint32_t>::iterator> kIterTable; // key -> list::iterator
	cacheobj_t get(cacheobj_t);
    
public:
	LFU(size_t capacity) {
    	this->capacity = capacity;
	}
	bool is_full() {
      return cache.size() == capacity;
    }
	bool has_obj(cacheobj_t obj){
		return this->get(obj) != -1;
	}
	cacheobj_t next_victim() {
        return fList[_minFrequent].front();
    }

	void add(cacheobj_t);
	cacheobj_t evict();
};

cacheobj_t LFU::get(cacheobj_t key) {
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

cacheobj_t LFU::evict() {
    if(cache.size() == capacity){
        cacheobj_t evictedKey = fList[_minFrequent].front();
        fList[_minFrequent].pop_front();
        kIterTable.erase(evictedKey);
        cache.erase(evictedKey);
        return evictedKey;
    }
    else
    {
        throw std::underflow_error("Error! Cannot evict. Cache not full");
    }
    

    
}   

void LFU::add(cacheobj_t key) {
    cacheobj_t value  = key;
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
#endif
