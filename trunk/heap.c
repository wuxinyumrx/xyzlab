/// @file heap.c
/// @synopsis 堆
/// @author Xinyu Woo <xinyu.wu@youlu-inc.com>
/// @version 1.0
/// @date 2012-05-15
#ifndef __HEAP_C__
#define __HEAP_C__

#include <stdlib.h>
#include <string.h>
#include "log.h"
#include "heap.h"

int32_t heap_create(heap_p *heap, uint32_t size, uint32_t item_size)
{/*{{{*/
    *heap = malloc(sizeof(heap_t));
    if(*heap == NULL)
    {
        LOG(LOG_ERROR, "malloc error in heap_create", 0, NULL);
        return -1;
    }
    (*heap)->data = malloc(size * item_size);
    if((*heap)->data == NULL)
    {
        LOG(LOG_ERROR, "malloc data error in heap_create", 0, NULL);
        return -1;
    }
    (*heap)->len = 0;
    (*heap)->size = size;
    (*heap)->item_size = item_size;
    return 0;
}/*}}}*/

int32_t heap_delete(heap_p *heap)
{/*{{{*/
    free((*heap)->data);
    free(*heap);
    *heap = NULL;
    return 0;
}/*}}}*/

int32_t heap_push(heap_p heap, void *item, int32_t heap_item_cmp(void *, void *))
{/*{{{*/
    if(heap->len == heap->size)
    {
        if(heap_item_cmp(item, heap->data) > 0)
        {
            uint32_t cur_pos = 0, min_son;
            for(;;)
            {
                min_son = (cur_pos << 1) + 1;
                if(min_son >= heap->len)
                {
                    break;
                }
                if(min_son + 1 < heap->len && heap_item_cmp(heap->data + heap->item_size * min_son, heap->data + heap->item_size * (min_son + 1)) > 0)
                {
                    min_son++;
                }
                if(heap_item_cmp(item, heap->data + heap->item_size * min_son) <= 0)
                {
                    break;
                }
                memcpy(heap->data + heap->item_size * cur_pos, heap->data + heap->item_size * min_son, heap->item_size);
                cur_pos = min_son;
            }
            memcpy(heap->data + heap->item_size * cur_pos, item, heap->item_size);
            return 0;
        }
        else
        {
            LOG(LOG_DEBUG, "do not need to push", 0, NULL);
            return 0;
        }
    }
    else
    {
        uint32_t cur_pos = heap->len, parent;
        while(cur_pos != 0)
        {
            parent = (cur_pos - 1) >> 1;
            if(parent < 0)
            {
                break;
            }
            if(heap_item_cmp(item, heap->data + heap->item_size * parent) < 0)
            {
                memcpy(heap->data + heap->item_size * cur_pos, heap->data + heap->item_size * parent, heap->item_size);
                cur_pos = parent;
            }
            else
            {
                break;
            }
        }
        memcpy(heap->data + heap->item_size * cur_pos, item, heap->item_size);
        heap->len ++;
        return 0;
    }
}/*}}}*/

int32_t heap_pop(heap_p heap, int32_t heap_item_cmp(void *, void *))
{/*{{{*/
    if(heap->len == 0)
    {
        LOG(LOG_ERROR, "heap is empty", 0, NULL);
        return -1;
    }
    heap->len--;
    uint32_t cur_pos = 0, min_son;
    for(;;)
    {
        min_son = (cur_pos << 1) + 1;
        if(min_son >= heap->len)
        {
            break;
        }
        if(min_son + 1 < heap->len && heap_item_cmp(heap->data + heap->item_size * min_son, heap->data + heap->item_size * (min_son + 1)) > 0)
        {
            min_son++;
        }
        if(heap_item_cmp(heap->data + heap->item_size * heap->len, heap->data + heap->item_size * min_son) <= 0)
        {
            break;
        }
        memcpy(heap->data + heap->item_size * cur_pos, heap->data + heap->item_size * min_son, heap->item_size);
        cur_pos = min_son;
    }
    memcpy(heap->data + heap->item_size * cur_pos, heap->data + heap->item_size * heap->len, heap->item_size);
    return 0;
}/*}}}*/

int32_t heap_top(heap_p heap, void **item)
{/*{{{*/
    if(heap->len == 0)
    {
        LOG(LOG_DEBUG, "heap is empty", 0, NULL);
        return -1;
    }
    *item = heap->data;
    return 0;
}/*}}}*/

int32_t heap_reset(heap_p heap)
{/*{{{*/
    heap->len = 0;
    return 0;
}/*}}}*/

#endif /* end of include guard: __HEAP_C__ */

