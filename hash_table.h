/// @file hash_table.h
/// @synopsis hash表
/// @author Xinyu Woo <xinyu.wu@youlu-inc.com>
/// @version 1.0
/// @date 2012-05-08
#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#include "slab_cache.h"

typedef struct _hash_table_item_t hash_table_item_t;
typedef struct _hash_table_item_t *hash_table_item_p;
struct _hash_table_item_t
{
    void *key;
    void *value;
    hash_table_item_p next;
};

struct _hash_table_t
{
    /* data */
    uint32_t size;
    hash_table_item_p *slots;
    slab_cache_p cache;
} /* optional variable list */;
typedef struct _hash_table_t hash_table_t;
typedef struct _hash_table_t *hash_table_p;

int32_t hash_table_create(hash_table_p *hash_table, uint32_t size);

int32_t hash_table_delete(hash_table_p *hash_table, void (item_operate)(hash_table_item_p));

int32_t hash_table_insert(hash_table_p hash_table, void *key, void *value, hash_table_item_p *item, uint32_t (* hash)(void *key));

int32_t hash_table_search(hash_table_p hash_table, void *key, hash_table_item_p *res, uint32_t (* hash)(void *key), int32_t (* key_cmp)(void *, void *));

int32_t hash_table_remove(hash_table_p hash_table, void *key, void **res_key, void **res_value, uint32_t (* hash)(void *key), int32_t (* key_cmp)(void *, void *));

#endif /* end of include guard: __HASH_TABLE_H__ */

