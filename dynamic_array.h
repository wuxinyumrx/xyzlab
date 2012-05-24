/// @file dynamic_array.h
/// @synopsis 动态数组
/// @author Xinyu Woo <xinyu.wu@youlu-inc.com>
/// @version 1.0
/// @date 2012-05-08
#ifndef __DYNAMIC_ARRAY_H__
#define __DYNAMIC_ARRAY_H__

#include <string.h>
#include <stdint.h>
#include <stdlib.h>

struct _dynamic_array_t
{/*{{{*/
    uint32_t capacity;
    uint32_t size;
    uint32_t item_size;
    void *data;
} /* optional variable list */;/*}}}*/
typedef struct _dynamic_array_t dynamic_array_t;
typedef struct _dynamic_array_t *dynamic_array_p;

int32_t dynamic_array_create(dynamic_array_p *dynamic_array, uint32_t init_capacity, uint32_t item_size);

int32_t dynamic_array_delete(dynamic_array_p *dynamic_array);

int32_t dynamic_array_insert(dynamic_array_p dynamic_array, void *value, int32_t (*item_compare)(void *, void *));

int32_t dynamic_array_append(dynamic_array_p dynamic_array, void *value);

int32_t dynamic_array_binary_find(dynamic_array_p dynamic_array, void *value, uint32_t *offset, int32_t (*item_compare)(void *, void *));

int32_t dynamic_array_binary_find_by_key(dynamic_array_p dynamic_array, int32_t (*item_compare)(void *, ...), ...);

int32_t dynamic_array_resize(dynamic_array_p dynamic_array);

int32_t dynamic_array_remove(dynamic_array_p dynamic_array, void *value, int32_t (*item_compare)(void *, void *));

int32_t dynamic_array_sort(dynamic_array_p dynamic_array, int32_t (*item_compare)(void *, void *));

int32_t dynamic_array_pop_from_start(dynamic_array_p dynamic_array, void *value);

int32_t dynamic_array_pop_from_end(dynamic_array_p dynamic_array, void *value);

int32_t dynamic_array_iterate(dynamic_array_p dynamic_array, void (* item_operate)(void *));

int32_t dynamic_array_get(dynamic_array_p dynamic_array, uint32_t offset, void **value);

int32_t dynamic_array_get_start(dynamic_array_p dynamic_array, void **value);

int32_t dynamic_array_get_end(dynamic_array_p dynamic_array, void **value);

int32_t dynamic_array_size(dynamic_array_p dynamic_array, uint32_t *size);

int32_t dynamic_array_reset(dynamic_array_p dynamic_array);

#endif /* end of include guard: __DYNAMIC_ARRAY_H__ */

