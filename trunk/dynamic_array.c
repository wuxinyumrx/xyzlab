/// @file dynamic_array.c
/// @synopsis 动态数组
/// @author Xinyu Woo <xinyu.wu@youlu-inc.com>
/// @version 1.0
/// @date 2012-05-08
#ifndef __DYNAMIC_ARRAY_C__
#define __DYNAMIC_ARRAY_C__

#include "log.h"
#include "dynamic_array.h"

int32_t dynamic_array_create(dynamic_array_p *dynamic_array, uint32_t init_capacity, uint32_t item_size)
{/*{{{*/
    *dynamic_array = malloc(sizeof(dynamic_array_t));
    if(*dynamic_array == NULL)
    {
        LOG(LOG_ERROR, "dynamic_array malloc error", 0, NULL); 
        return -1;
    }
    (*dynamic_array)->size = 0;
    (*dynamic_array)->capacity = init_capacity;
    (*dynamic_array)->item_size = item_size;
    (*dynamic_array)->data = malloc(init_capacity * item_size);
    if((*dynamic_array)->data == NULL)
    {
        LOG(LOG_ERROR, "dynamic_array data malloc error", 0, NULL); 
        return -1;
    }
    return 0;
}/*}}}*/

int32_t dynamic_array_delete(dynamic_array_p *dynamic_array)
{/*{{{*/
    if(dynamic_array != NULL && (*dynamic_array) != NULL)
    {
        if((*dynamic_array)->data != NULL)
        {
            free((*dynamic_array)->data);
        }
        free(*dynamic_array);
        *dynamic_array = NULL;
    }
    return 0;
}/*}}}*/

int32_t dynamic_array_insert(dynamic_array_p dynamic_array, void *value, int32_t (*item_compare)(void *, void *))
{/*{{{*/
    if(dynamic_array == NULL)
    {
        LOG(LOG_ERROR, "dynamic_array is null error in dynamic_array_insert", 0, NULL); 
        return -1;
    }
    uint32_t offset;
    if(dynamic_array_binary_find(dynamic_array, value, &offset, item_compare) != 0)
    {
        LOG(LOG_ERROR, "dynamic_array_binary_find error in dynamic_array_insert", 0, NULL); 
        return -1;
    }


    if(dynamic_array->size == dynamic_array->capacity)
    {
        if(dynamic_array_resize(dynamic_array) != 0)
        {
            LOG(LOG_ERROR, "dynamic_array_resize error in dynamic_array_insert", 0, NULL); 
            return -1;
        }
    }

    if(offset != dynamic_array->size)
    {
        if(item_compare(value, dynamic_array->data + offset * dynamic_array->item_size) == 0)
        {
            LOG(LOG_DEBUG, "value exist in dynamic_array", 0, NULL);
            memcpy(dynamic_array->data + offset * dynamic_array->item_size, value, dynamic_array->item_size);
            return 0;
        }
        memmove(dynamic_array->data + (offset + 1) * dynamic_array->item_size,
               dynamic_array->data + offset * dynamic_array->item_size, 
               (dynamic_array->size - offset) * dynamic_array->item_size);
    }
    memcpy(dynamic_array->data + offset * dynamic_array->item_size, value, dynamic_array->item_size);
    dynamic_array->size ++;
    return 0;
}/*}}}*/

int32_t dynamic_array_append(dynamic_array_p dynamic_array, void *value)
{/*{{{*/
    if(dynamic_array == NULL)
    {
        LOG(LOG_ERROR, "dynamic_array is null error in dynamic_array_append", 0, NULL); 
        return -1;
    }
    if(dynamic_array->size == dynamic_array->capacity)
    {
        if(dynamic_array_resize(dynamic_array) != 0)
        {
            LOG(LOG_ERROR, "dynamic_array_resize error in dynamic_array_append", 0, NULL); 
            return -1;
        }
    }
    memcpy(dynamic_array->data + dynamic_array->size * dynamic_array->item_size, value, dynamic_array->item_size);
    dynamic_array->size ++;
    return 0;
}/*}}}*/

int32_t dynamic_array_binary_find(dynamic_array_p dynamic_array, void *value, uint32_t *offset, int32_t (*item_compare)(void *, void *))
{/*{{{*/
    if(dynamic_array == NULL)
    {
        LOG(LOG_ERROR, "dynamic_array is null error in dynamic_array_binary_find", 0, NULL); 
        return -1;
    }
    if(dynamic_array->data == NULL)
    {
        LOG(LOG_ERROR, "dynamic_array data is null error in dynamic_array_binary_find", 0, NULL); 
        return -1;
    }
    if(dynamic_array->size == 0)
    {
        *offset = 0;
        return 0;
    }
    uint32_t start = 0, end = dynamic_array->size - 1, mid;
    int32_t res;
    while(start < end)
    {
        mid = (start + end) / 2;
        res = item_compare(value, dynamic_array->data + mid * dynamic_array->item_size);
        if(res == 0)
        {
            *offset = mid;
            return 0;
        }
        else if(res > 0)
        {
            start = mid + 1;
        }
        else
        {
            end = mid;
        }
    }
    res = item_compare(value, dynamic_array->data + start * dynamic_array->item_size);
    if(res <= 0)
    {
        *offset = start;
        return 0;
    }
    else 
    {
        *offset = start + 1;
        return 0;
    }
}/*}}}*/

int32_t dynamic_array_resize(dynamic_array_p dynamic_array)
{/*{{{*/
    if(dynamic_array->size < dynamic_array->capacity)
    {
        LOG(LOG_ERROR, "dynamic_array need not resize in dynamic_array_resize", 0, NULL); 
        return -1;
    }
    uint32_t dynamic_array_len = dynamic_array->capacity * dynamic_array->item_size;
    if(dynamic_array_len & 0x80000000)
    {
        LOG(LOG_ERROR, "dynamic_array capacity is too large", 0, NULL); 
        return -1;
    }
    void *res = realloc(dynamic_array->data, dynamic_array_len << 1);
    if(res == NULL)
    {
        LOG(LOG_ERROR, "dynamic_array data realloc error in dynamic_array_resize", 0, NULL); 
        return -1;
    }
    dynamic_array->data = res;
    dynamic_array->capacity <<= 1;
    return 0;
}/*}}}*/

int32_t dynamic_array_remove(dynamic_array_p dynamic_array, void *value, int32_t (*item_compare)(void *, void *))
{/*{{{*/
    if(dynamic_array == NULL)
    {
        LOG(LOG_ERROR, "dynamic_array is null error in dynamic_array_remove", 0, NULL); 
        return -1;
    }
    if(dynamic_array->size == 0)
    {
        LOG(LOG_INFO, "dynamic_array size is 0 error in dynamic_array_remove", 0, NULL); 
        return -1;
    }
    uint32_t offset;
    if(dynamic_array_binary_find(dynamic_array, value, &offset, item_compare) != 0)
    {
        LOG(LOG_ERROR, "dynamic_array_binary_find error in dynamic_array_remove", 0, NULL); 
        return -1;
    }
    if(offset == dynamic_array->size)
    {
        LOG(LOG_INFO, "dynamic_array_binary_find result is not equal value error in dynamic_array_remove", 0, NULL); 
        return -1;
    }
    int32_t res = item_compare(value, dynamic_array->data + offset * dynamic_array->item_size);
    if(res != 0)
    {
        LOG(LOG_INFO, "dynamic_array_binary_find result is not equal value error in dynamic_array_remove", 0, NULL); 
        return -1;
    }
    if(offset != dynamic_array->size - 1)
    {
        memmove(dynamic_array->data + offset * dynamic_array->item_size, 
                dynamic_array->data + (offset + 1) * dynamic_array->item_size, 
                (dynamic_array->size - offset - 1) * dynamic_array->item_size);
    }
    dynamic_array->size--;
    return 0;
}/*}}}*/

int32_t dynamic_array_sort(dynamic_array_p dynamic_array, int32_t (*item_compare)(void *, void *))
{/*{{{*/
    uint32_t i, j, k;
    uint32_t item_size = dynamic_array->item_size;
    void *tmp_item = malloc(item_size);
    for(i = 0; i < dynamic_array->size; i++)
    {
        k = i;
        for(j = i + 1; j < dynamic_array->size; j++)
        {
            if(item_compare(dynamic_array->data + j * item_size, dynamic_array->data + k * item_size) < 0)
            {
                k = j;
            }
        }
        if(k != i)
        {
            memcpy(tmp_item, dynamic_array->data + i * item_size, item_size);
            memcpy(dynamic_array->data + i * item_size, dynamic_array->data + k * item_size, item_size);
            memcpy(dynamic_array->data + k * item_size, tmp_item, item_size);
        }
    }
    free(tmp_item);
    return 0;
}/*}}}*/

int32_t dynamic_array_pop_from_start(dynamic_array_p dynamic_array, void *value)
{/*{{{*/
    if(dynamic_array == NULL)
    {
        LOG(LOG_ERROR, "dynamic_array is null error in dynamic_array_pop_from_start", 0, NULL); 
        return -1;
    }
    if(dynamic_array->size == 0)
    {
        LOG(LOG_ERROR, "dynamic_array size is 0 error in dynamic_array_pop_from_start", 0, NULL); 
        return -1;
    }
    memcpy(value, dynamic_array->data, dynamic_array->item_size);
    if(dynamic_array->size != 1)
    {
        memmove(dynamic_array->data, dynamic_array->data + dynamic_array->item_size, (dynamic_array->size - 1) * dynamic_array->item_size);
    }
    dynamic_array->size--;
    return 0;
}/*}}}*/

int32_t dynamic_array_pop_from_end(dynamic_array_p dynamic_array, void *value)
{/*{{{*/
    if(dynamic_array == NULL)
    {
        LOG(LOG_ERROR, "dynamic_array is null error in dynamic_array_pop_from_end", 0, NULL); 
        return -1;
    }
    if(dynamic_array->size == 0)
    {
        LOG(LOG_ERROR, "dynamic_array size is 0 error in dynamic_array_pop_from_end", 0, NULL); 
        return -1;
    }
    memcpy(value, dynamic_array->data + (dynamic_array->size - 1) * dynamic_array->item_size, dynamic_array->item_size);
    dynamic_array->size--;
    return 0;
}/*}}}*/

int32_t dynamic_array_iterate(dynamic_array_p dynamic_array, void (* item_operate)(void *))
{/*{{{*/
    if(dynamic_array == NULL)
    {
        LOG(LOG_ERROR, "dynamic_array is null error in dynamic_array_iterate", 0, NULL);
        return -1;
    }
    if(item_operate == NULL)
    {
        LOG(LOG_ERROR, "item_operate is null error in dynamic_array_iterate", 0, NULL);
        return -1;
    }
    uint32_t i;
    for(i = 0; i < dynamic_array->size; i++)
    {
        item_operate((void *)((char *)(dynamic_array->data) + i * dynamic_array->item_size));
    }
    return 0;
}/*}}}*/

int32_t dynamic_array_get(dynamic_array_p dynamic_array, uint32_t offset, void **value)
{/*{{{*/
    if(dynamic_array == NULL)
    {
        LOG(LOG_ERROR, "dynamic_array is null error in dynamic_array_get", 0, NULL);
        return -1;
    }
    if(offset >= dynamic_array->size)
    {
        LOG(LOG_ERROR, "offset is bigger than size in dynamic_array_get", 0, NULL);
        return -1;
    }
    *value = (void *)((char *)(dynamic_array->data) + offset * dynamic_array->item_size);
    return 0;
}/*}}}*/

int32_t dynamic_array_get_start(dynamic_array_p dynamic_array, void **value)
{/*{{{*/
    if(dynamic_array == NULL)
    {
        LOG(LOG_ERROR, "dynamic_array is null error in dynamic_array_get_start", 0, NULL);
        return -1;
    }
    *value = dynamic_array->data;
    return 0;
}/*}}}*/

int32_t dynamic_array_get_end(dynamic_array_p dynamic_array, void **value)
{/*{{{*/
    if(dynamic_array == NULL)
    {
        LOG(LOG_ERROR, "dynamic_array is null error in dynamic_array_get_end", 0, NULL);
        return -1;
    }
    *value = (void *)((char *)(dynamic_array->data) + (dynamic_array->size - 1) * dynamic_array->item_size);
    return 0;
}/*}}}*/

int32_t dynamic_array_size(dynamic_array_p dynamic_array, uint32_t *size)
{/*{{{*/
    if(dynamic_array == NULL)
    {
        LOG(LOG_ERROR, "dynamic_array is null error in dynamic_array_size", 0, NULL);
        return -1;
    }
    *size = dynamic_array->size;
    return 0;
}/*}}}*/

int32_t dynamic_array_reset(dynamic_array_p dynamic_array)
{/*{{{*/
    dynamic_array->size = 0;
    return 0;
}/*}}}*/

#endif /* end of include guard: __DYNAMIC_ARRAY_C__ */

