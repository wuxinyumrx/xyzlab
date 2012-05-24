/// @file set.h
/// @synopsis 集合
/// @author Xinyu Woo <xinyu.wu@youlu-inc.com>
/// @version 1.0
/// @date 2012-05-08
#ifndef __SET_H__
#define __SET_H__

#include <stdint.h>

struct _set_t
{
    /* data */
    uint32_t size;
    uint32_t item_size;
    uint32_t len;
    void *data;
} /* optional variable list */;
typedef struct _set_t set_t;
typedef struct _set_t *set_p;

int32_t set_create(set_p *set, uint32_t size, uint32_t item_size);

int32_t set_delete(set_p *set);

int32_t set_reset(set_p set);

int32_t set_add(set_p set, void *item, int32_t set_item_dump_compare(void *, void *), int32_t set_item_sort_compare(void *, void *));

int32_t set_del(set_p set, void *item, int32_t set_item_dump_compare(void *, void *));

int32_t set_find(set_p set, void *item, uint32_t *offset, int32_t set_item_sort_compare(void *, void *));

int32_t set_get(set_p set, uint32_t offset, void **item);

int32_t set_len(set_p set, uint32_t *len);

#endif /* end of include guard: __SET_H__ */

