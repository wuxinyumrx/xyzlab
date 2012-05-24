/// @file slab_cache.h
/// @synopsis 固定大小内存分配器
/// @author Xinyu Woo <xinyu.wu@youlu-inc.com>
/// @version 1.0
/// @date 2012-05-08
#ifndef __SLAB_CACHE_H__
#define __SLAB_CACHE_H__

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "dynamic_array.h"

struct _slab_cache_t
{/*{{{*/
    dynamic_array_p buckets;
    void *free_header;
    void *free_tail;
    uint32_t block_size;
    uint32_t cur_bucket_fence;
}; /*}}}*/
typedef struct _slab_cache_t slab_cache_t;
typedef struct _slab_cache_t *slab_cache_p;

int32_t slab_cache_create(slab_cache_p *slab_cache, uint32_t block_size);

int32_t slab_cache_delete(slab_cache_p *slab_cache);

int32_t slab_cache_alloc(slab_cache_p slab_cache, void **mem);

int32_t slab_cache_free(slab_cache_p slab_cache, void **mem);

#endif /* end of include guard: __SLAB_CACHE_H__ */

