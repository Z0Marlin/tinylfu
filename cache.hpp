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
	int cache_size;
	std::list<cacheobj_t> cache_list;
    std::unordered_map<cacheobj_t, std::list<cacheobj_t>::iterator> ma;
public:
    LRU(int);
    void display();
	bool is_full();
	bool has_obj(cacheobj_t);
	bool is_empty();
	void  add(cacheobj_t);
	cacheobj_t evict();
	cacheobj_t next_victim();
}; 

LRU::LRU(int n){ 
	LRU::cache_size = n; 
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