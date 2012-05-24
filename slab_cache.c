/// @file slab_cache.c
/// @synopsis 固定大小内存分配器
/// @author Xinyu Woo <xinyu.wu@youlu-inc.com>
/// @version 1.0
/// @date 2012-05-08
#ifndef __SLAB_CACHE_C__
#define __SLAB_CACHE_C__

#include "slab_cache.h"
#include "log.h"
#define ONCE_ALLOC_NUM 1000
#define BUCKET_INIT_SIZE 300

void bucket_free(void *bucket)
{/*{{{*/
    free(*(char **)bucket);
}/*}}}*/

int32_t slab_cache_create(slab_cache_p *slab_cache, uint32_t block_size)
{/*{{{*/
    *slab_cache = malloc(sizeof(slab_cache_t));
    if(*slab_cache == NULL)
    {
        LOG(LOG_ERROR, "slab_cache_create malloc error", 0, NULL);
        return -1;
    }
    int32_t res;
    res = dynamic_array_create(&((*slab_cache)->buckets), BUCKET_INIT_SIZE, sizeof(void *));
    if(res != 0)
    {
        LOG(LOG_ERROR, "dynamic_array_create error in slab_cache_create", 0, NULL);
        free(*slab_cache);
        return -1;
    }
    (*slab_cache)->free_header = NULL;
    (*slab_cache)->free_tail= NULL;
    if(block_size < sizeof(void *))
    {
        (*slab_cache)->block_size = sizeof(void *);
    }
    else
    {
        (*slab_cache)->block_size = block_size;
    }
    (*slab_cache)->cur_bucket_fence = (*slab_cache)->block_size * ONCE_ALLOC_NUM;
    return 0;
}/*}}}*/

int32_t slab_cache_delete(slab_cache_p *slab_cache)
{/*{{{*/
    if(slab_cache != NULL && *slab_cache != NULL)
    {
        dynamic_array_iterate((*slab_cache)->buckets, bucket_free);
        dynamic_array_delete(&((*slab_cache)->buckets));
        free(*slab_cache);
    }
    return 0;
}/*}}}*/

int32_t slab_cache_alloc(slab_cache_p slab_cache, void **mem)
{/*{{{*/
    if(slab_cache == NULL)
    {
        LOG(LOG_ERROR, "slab_cache is null error in slab_cache_alloc", 0, NULL);
        return -1;
    }
    if(slab_cache->free_header != NULL)
    {
        *mem = slab_cache->free_header;
        slab_cache->free_header = *(void **)(slab_cache->free_header);
        if(slab_cache->free_header == NULL)
        {
            slab_cache->free_tail = NULL;
        }
        return 0;
    }
    uint32_t bucket_size = slab_cache->block_size * ONCE_ALLOC_NUM;
    if(slab_cache->cur_bucket_fence == bucket_size)
    {
        void *bucket = malloc(bucket_size);
        if(bucket == NULL)
        {
            LOG(LOG_ERROR, "bucket malloc error in slab_cache_alloc", 0, NULL);
            return -1;
        }
        *mem = bucket;
        dynamic_array_append(slab_cache->buckets, (void *)(&bucket));
        slab_cache->cur_bucket_fence = slab_cache->block_size;
        return  0;
    }
    else
    {
        void *cur_bucket;
        dynamic_array_get_end(slab_cache->buckets, &cur_bucket);
        *mem = (void *)(*(char **)cur_bucket + slab_cache->cur_bucket_fence);
        slab_cache->cur_bucket_fence += slab_cache->block_size;
        return 0;
    }
}/*}}}*/

int32_t slab_cache_free(slab_cache_p slab_cache, void **mem)
{/*{{{*/
    if(mem == NULL || *mem == NULL)
    {
        LOG(LOG_ERROR, "mem to free is nil", 0, NULL);
        return -1;
    }
    *(void **)(*mem) = NULL;
    if(slab_cache->free_header == NULL)
    {
        slab_cache->free_header = slab_cache->free_tail = *mem;
    }
    else
    {
        *(void **)(slab_cache->free_tail) = *mem;
        slab_cache->free_tail = *mem;
    }
    *mem = NULL;
    return 0;
}/*}}}*/

#endif /* end of include guard: __SLAB_CACHE_C__ */

