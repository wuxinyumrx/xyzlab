/// @file set.c
/// @synopsis 集合
/// @author Xinyu Woo <xinyu.wu@youlu-inc.com>
/// @version 1.0
/// @date 2012-05-08
#ifndef __SET_C__
#define __SET_C__

#include <stdlib.h>
#include <string.h>
#include "set.h"
#include "log.h"

int32_t set_create(set_p *set, uint32_t size, uint32_t item_size)
{/*{{{*/
    *set = malloc(sizeof(set_t));
    if(*set == NULL)
    {
        LOG(LOG_ERROR, "malloc error in set_create", 0, NULL);
        return -1;
    }
    (*set)->data = malloc(size * item_size);
    if((*set)->data == NULL)
    {
        LOG(LOG_ERROR, "malloc data error in set_create", 0, NULL);
        return -1;
    }
    (*set)->size = size;
    (*set)->item_size = item_size;
    (*set)->len = 0;
    return 0;
}/*}}}*/

int32_t set_delete(set_p *set)
{/*{{{*/
    free((*set)->data);
    free(*set);
    *set = NULL;
    return 0;
}/*}}}*/

int32_t set_reset(set_p set)
{/*{{{*/
    set->len = 0;
    return 0;
}/*}}}*/

int32_t set_add(set_p set, void *item, int32_t set_item_dump_compare(void *, void *), int32_t set_item_sort_compare(void *, void *))
{/*{{{*/
    uint32_t i, data_len = set->item_size * set->len;
    for(i = 0; i < data_len; i += set->item_size)
    {
        if(set_item_dump_compare(item, set->data + i) == 0)
        {
            memmove(set->data + i, set->data + i + set->item_size, data_len - i - set->item_size);
            set->len--;
            break;
        }
    }
    data_len = set->item_size * set->len;
    if(set->len == set->size)
    {
        if(set_item_sort_compare(item, set->data + data_len - set->item_size) > 0)
        {
            set->len--;
        }
        else
        {
            return 0;
        }
    }
    data_len = set->item_size * set->len;
    for(i = 0; i < data_len; i += set->item_size)
    {
        if(set_item_sort_compare(item, set->data + i) < 0)
        {
            memmove(set->data + i + set->item_size, set->data + i, data_len - i);
            memcpy(set->data + i, item, set->item_size);
            set->len++;
            return 0;
        }
    }
    memcpy(set->data + data_len, item, set->item_size);
    set->len++;
    return 0;
}/*}}}*/

int32_t set_del(set_p set, void *item, int32_t set_item_dump_compare(void *, void *))
{/*{{{*/
    uint32_t i, data_len = set->item_size * set->len;
    for(i = 0; i < data_len; i += set->item_size)
    {
        if(set_item_dump_compare(item, set->data + i) == 0)
        {
            memmove(set->data + i, set->data + i + set->item_size, data_len - i - set->item_size);
            set->len--;
            return 0;
        }
    }
    LOG(LOG_DEBUG, "item not exist in set", 0, NULL);
    return -1;
}/*}}}*/

int32_t set_find(set_p set, void *item, uint32_t *offset, int32_t set_item_sort_compare(void *, void *))
{/*{{{*/
    *offset = 0;
    uint32_t i;
    for (i = 0; i < set->len; i++)
    {
        if(set_item_sort_compare(item, set->data + i * set->item_size) <= 0)
        {
            *offset = i;
            return 0;
        }
    }
    LOG(LOG_DEBUG, "item not exist in set", 0, NULL);
    return -1;
}/*}}}*/

int32_t set_get(set_p set, uint32_t offset, void **item)
{/*{{{*/
    if(offset >= set->len)
    {
        LOG(LOG_ERROR, "offset overflowed", 0, NULL);
        return -1;
    }
    *item = set->data + set->item_size * offset;
    return 0;
}/*}}}*/

int32_t set_len(set_p set, uint32_t *len)
{/*{{{*/
    *len = set->len;
    return 0;
}/*}}}*/

#endif /* end of include guard: __SET_C__ */

