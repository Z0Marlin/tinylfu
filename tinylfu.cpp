#include"bfilter.hpp"

typedef uint32_t cacheobj_t;

class Cache{
public:
    virtual void add(cacheobj_t) = 0;
    virtual bool is_full() = 0;
    virtual bool is_empty() = 0;
    virtual bool has_obj(cacheobj_t) = 0;
    virtual cacheobj_t evict() = 0;
    virtual cacheobj_t next_victim() = 0;
};