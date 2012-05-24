/// @file hash_table_test.c
/// @synopsis hash表测试
/// @author Xinyu Woo <xinyu.wu@youlu-inc.com>
/// @version 1.0
/// @date 2012-05-08
#include <stdio.h>
#include "hash_table.h"
#include "log.h"
uint32_t hash(void *key)
{
    return *(uint32_t *)key;
}
int32_t key_cmp(void *left, void *right)
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
void item_operate(hash_table_item_p item)
{
    free(item->key);
}
int main(int argc, const char *argv[])
{
    init_log("./log", "log", "hash_table_test");
    hash_table_p hash_table;
    hash_table_create(&hash_table, 100);
    uint32_t i;
    for(i = 0; i < 200; i++)
    {
        uint32_t *ii = malloc(sizeof(uint32_t));
        *ii = i;
        hash_table_item_p item;
        hash_table_insert(hash_table, ii, ii, &item, hash);
        printf("%d %d\n", *(uint32_t *)(item->key), *(uint32_t *)(item->value));
    }
    hash_table_delete(&hash_table, item_operate);
    exit_log();
    return 0;
}
