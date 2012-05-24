/// @file slab_cache_test.c
/// @synopsis 固定大小内存分配器测试
/// @author Xinyu Woo <xinyu.wu@youlu-inc.com>
/// @version 1.0
/// @date 2012-05-08
#include <stdio.h>
#include "slab_cache.h"
#include <time.h>
#include "log.h"

int main(int argc, const char *argv[])
{
    init_log("./log/", "log", "slab_cache_test");
    slab_cache_p slab_cache;
    slab_cache_create(&slab_cache, sizeof(int));
    void *mem_1, *mem_2, *mem_3, *mem_4;
    slab_cache_alloc(slab_cache, &mem_1);
    slab_cache_alloc(slab_cache, &mem_2);
    slab_cache_alloc(slab_cache, &mem_3);
    mem_4 = mem_2;
    slab_cache_free(slab_cache, &mem_4);
    slab_cache_alloc(slab_cache, &mem_2);
    slab_cache_free(slab_cache, &mem_2);
    slab_cache_free(slab_cache, &mem_1);
    slab_cache_alloc(slab_cache, &mem_2);
    slab_cache_free(slab_cache, &mem_2);
    slab_cache_free(slab_cache, &mem_3);
    //uint32_t i;
    //clock_t start, end;
    //start = clock();
    //for(i = 0; i < 10000000; i++)
    //{
    //    slab_cache_alloc(slab_cache, &mem);
    //    *(int *)mem = i;
    //    slab_cache_free(slab_cache, &mem);
    //}
    //end = clock();
    //printf("time : %ld\n", end - start);
    //start = clock();
    //for(i = 0; i < 10000000; i++)
    //{
    //    mem = malloc(4);
    //    *(int *)mem = i;
    //    free(mem);
    //}
    //end = clock();
    //printf("time : %ld\n", end - start);
    slab_cache_delete(&slab_cache);
    exit_log();
    return 0;
}
