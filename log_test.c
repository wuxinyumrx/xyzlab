/// @file log_test.c
/// @synopsis 日志测试
/// @author Xinyu Woo <xinyu.wu@youlu-inc.com>
/// @version 1.0
/// @date 2012-05-08
#include <stdio.h>
#include "log.h"
int main(int argc, const char *argv[])
{
    init_log("./log/", "log", "log_test");
    return 0;
}
