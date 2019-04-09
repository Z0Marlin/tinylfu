#include <bits/stdc++.h> 
#include "cache.hpp"

using namespace std; 

typedef uint32_t cacheobj_t;

class LRU: public Cache { 

    //max cache size
    int cache_size;
    
    //cache key
	list<cacheobj_t> cache_list; 

    //hash map for references of keys
    unordered_map<cacheobj_t, list<cacheobj_t>::iterator> ma; 

public:
    LRUCache(int);
    void display();
}; 

LRUCache::LRUCache(int n) 
{ 
	LRUCache::cache_size = n; 
} 


bool LRUCache::is_full(){
	return (LRUCache::cache_list.size() == LRUCache::cache_size);
} 

bool LRUCache::has_obj(cacheobj_t x){
    return !(LRUCache::ma.find(x) == LRUCache::ma.end());
}

bool LRUCache::is_empty(){
    return LRUCache::cache_list.empty();
} 

void LRUCache::add(cacheobj_t x){
    //not present in Cache
	if (!LRUCache::has_obj(x)) 
	{ 
		if (LRUCache::is_full()) 
		{ 
        	//delete least recently used element 
			cacheobj_t last=LRUCache::evict();
		} 
	} 
	// present in cache 
	else
		LRUCache::cache_list.erase(LRUCache::ma[x]); 

	// update reference of key 
	LRUCache::cache_list.push_front(x); 
	LRUCache::ma[x] = LRUCache::cache_list.begin(); 
}


cacheobj_t LRUCache::next_victim(){
    	return LRUCache::cache_list.back();
}

cacheobj_t LRUCache::evict(){
    	cacheobj_t last = LRUCache::next_victim(); 
		LRUCache::cache_list.pop_back(); 
	    LRUCache::ma.erase(last); 
		return last;
} 

void LRUCache::display() 
{ 
    for (auto it = LRUCache::cache_list.begin(); it != LRUCache::cache_list.end();it++) 
        cout << (*it) << " "; 
    cout << endl; 
} 

// Driver program to test above functions 
int main() 
{ 
	LRUCache ca(4); 
	ca.add(1); 
	ca.add(2); 
	ca.add(3); 
	ca.add(1); 
	ca.add(4); 
	ca.add(5); 
	ca.display(); 

	return 0; 
}