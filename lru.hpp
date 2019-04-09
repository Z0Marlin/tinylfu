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
    LRU(int);
    void display();
}; 

LRU::LRU(int n) 
{ 
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
    //not present in Cache
	if (!LRU::has_obj(x)) 
	{ 
		if (LRU::is_full()) 
		{ 
        	//delete least recently used element 
			cacheobj_t last=LRU::evict();
		} 
	} 
	// present in cache 
	else
		LRU::cache_list.erase(LRU::ma[x]); 

	// update reference of key 
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

void LRU::display() 
{ 
    for (auto it = LRU::cache_list.begin(); it != LRU::cache_list.end();it++) 
        cout << (*it) << " "; 
    cout << endl; 
} 

// // Driver program to test above functions 
// int main() 
// { 
// 	LRU ca(4); 
// 	ca.add(1); 
// 	ca.add(2); 
// 	ca.add(3); 
// 	ca.add(1); 
// 	ca.add(4); 
// 	ca.add(5); 
// 	ca.display(); 

// 	return 0; 
// }