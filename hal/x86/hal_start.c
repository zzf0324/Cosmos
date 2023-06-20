/**
 * 内核入口开始文件
 */

#include "cosmostypes.h"
#include "cosmosmctrl.h"

/**
 * 内核初始化总控函数
 */
void hal_start(){
    //初始化hal层
    init_hal();
    //初始化内核层
    init_krl();
    return;
}