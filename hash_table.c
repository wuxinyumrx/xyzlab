/// @file hash_table.c
/// @synopsis hash表
/// @author Xinyu Woo <xinyu.wu@youlu-inc.com>
/// @version 1.0
/// @date 2012-05-08
#ifndef __HASH_TABLE_C__
#define __HASH_TABLE_C__

#include "hash_table.h"
#include "log.h"

int32_t hash_table_create(hash_table_p *hash_table, uint32_t size)
{/*{{{*/
    *hash_table = malloc(sizeof(hash_table_t));
    if(*hash_table == NULL)
    {
        LOG(LOG_ERROR, "malloc error in hash_table_create", 0, NULL);
        return -1;
    }
    (*hash_table)->size = size;
    (*hash_table)->slots = (hash_table_item_p *)calloc(sizeof(void *), size);
    if((*hash_table)->slots == NULL)
    {
        LOG(LOG_ERROR, "slots malloc error in hash_table_create", 0, NULL);
        free(*hash_table);
        return -1;
    }
    if(slab_cache_create(&((*hash_table)->cache), sizeof(hash_table_item_t)) != 0)
    {
        LOG(LOG_ERROR, "slab_cache_create error in hash_table_create", 0, NULL);
        free((*hash_table)->slots);
        free(*hash_table);
        return -1;
    }
    return 0;
}/*}}}*/

int32_t hash_table_delete(hash_table_p *hash_table, void (item_operate)(hash_table_item_p))
{/*{{{*/
    if(hash_table != NULL && *hash_table != NULL)
    {
        if(item_operate != NULL)
        {
            uint32_t i;
            hash_table_item_p item;
            for(i = 0; i < (*hash_table)->size; i++)
            {
                item = (*hash_table)->slots[i];
                while(item != NULL)
                {
                    item_operate(item);
                    item = item->next;
                }
            }
        }
        free((*hash_table)->slots);
        slab_cache_delete(&(*hash_table)->cache);
        free(*hash_table);
        *hash_table = NULL;
    }
    return 0;
}/*}}}*/

int32_t hash_table_insert(hash_table_p hash_table, void *key, void *value, hash_table_item_p *item, uint32_t (* hash)(void *key))
{/*{{{*/

    uint32_t key_hash = hash(key);
    uint32_t slot_num = key_hash % hash_table->size;
    hash_table_item_p cur;
    if(slab_cache_alloc(hash_table->cache, (void **)&cur) != 0)
    {
        LOG(LOG_ERROR, "slab_cache_alloc error in hash_table_insert", 0, NULL);
        return -1;
    }
    cur->key = key;
    cur->value = value;
    cur->next = hash_table->slots[slot_num];
    hash_table->slots[slot_num] = cur;
    *item = cur;
    return 0;
}/*}}}*/

int32_t hash_table_search(hash_table_p hash_table, void *key, hash_table_item_p *res, uint32_t (* hash)(void *key), int32_t (* key_cmp)(void *, void *))
{/*{{{*/
    uint32_t key_hash = hash(key);
    uint32_t slot_num = key_hash % hash_table->size;
    hash_table_item_p it = hash_table->slots[slot_num];
    while(it != NULL)
    {
        if(key_cmp(it->key, key) != 0)
        {
            it = it->next;
        }
        else
        {
            *res = it;
            return 0;
        }
    }
    *res = NULL;
    LOG(LOG_DEBUG, "key don't exist in hash_table info in hash_table_search", 0, NULL);
    return -1;
}/*}}}*/

int32_t hash_table_remove(hash_table_p hash_table, void *key, void **res_key, void **res_value, uint32_t (* hash)(void *key), int32_t (* key_cmp)(void *, void *))
{/*{{{*/
    uint32_t key_hash = hash(key);
    uint32_t slot_num = key_hash % hash_table->size;
    hash_table_item_p it = hash_table->slots[slot_num];

    if(it == NULL)
    {
        LOG(LOG_DEBUG, "key don't exist in hash_table error in hash_table_remove", 0, NULL);
        return -1;
    }
    else if(key_cmp(it->key, key) == 0)
    {
        *res_key = it->key;
        *res_value = it->value;
        hash_table->slots[slot_num] = it->next;
        slab_cache_free(hash_table->cache, (void **)&it);
        return 0;
    }

    hash_table_item_p it_next = it->next;
    while(it_next != NULL)
    {
        if(key_cmp(it_next->key, key) != 0)
        {
            it = it->next;
            it_next = it->next;
        }
        else
        {
            it->next = it_next->next;
            *res_key = it_next->key;
            *res_value = it_next->value;
            slab_cache_free(hash_table->cache, (void **)&it_next);
            return 0;
        }
    }
    LOG(LOG_INFO, "key don't exist in hash_table", 0, NULL);
    return -1;
}/*}}}*/

#endif /* end of include guard: __HASH_TABLE_C__ */

