/// @file heap.h
/// @synopsis 堆
/// @author Xinyu Woo <xinyu.wu@youlu-inc.com>
/// @version 1.0
/// @date 2012-05-15
#ifndef __HEAP_H__
#define __HEAP_H__

#include <stdint.h>

struct _heap_t
{/*{{{*/
    /* data */
    uint32_t size;
    uint32_t item_size;
    uint32_t len;
    void *data;
} /* optional variable list */;/*}}}*/
typedef struct _heap_t heap_t;
typedef struct _heap_t *heap_p;

int32_t heap_create(heap_p *heap, uint32_t size, uint32_t item_size);

int32_t heap_delete(heap_p *heap);

int32_t heap_push(heap_p heap, void *item, int32_t heap_item_cmp(void *, void *));

int32_t heap_pop(heap_p heap, int32_t heap_item_cmp(void *, void *));

int32_t heap_top(heap_p heap, void **item);

int32_t heap_reset(heap_p heap);

#endif /* end of include guard: __HEAP_H__ */

