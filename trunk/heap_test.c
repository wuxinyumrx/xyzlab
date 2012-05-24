/// @file heap_test.c
/// @synopsis 堆测试
/// @author Xinyu Woo <xinyu.wu@youlu-inc.com>
/// @version 1.0
/// @date 2012-05-16
#ifndef __HEAP_TEST_C__
#define __HEAP_TEST_C__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "heap.h"
#include "log.h"

int32_t num_cmp(void *left, void *right)
{
    if(*(uint32_t *)left > *(uint32_t *)right)
    {
        return 1;
    }
    if(*(uint32_t *)left < *(uint32_t *)right)
    {
        return -1;
    }
    return 0;
}

int main(int argc, const char *argv[])
{
    if(init_log("./log/", "log", "heap_test")!= 0)
    {
        exit(1);
    }
    heap_p heap;
    heap_create(&heap, 10, sizeof(uint32_t));
    srand(time(NULL));
    uint32_t i, num;
    for(i = 0; i < 100; i++)
    {
        num = rand() % 200;
        heap_push(heap, &num, num_cmp);
    }
    heap_delete(&heap);
    exit_log();
    return 0;
}

#endif /* end of include guard: __HEAP_TEST_C__ */

