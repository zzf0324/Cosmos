/**
 * HAL全局初始化文件halinit.c
 */     

#include "cosmostypes.h"
#include "cosmosmctrl.h"


/**
 * 初始化函数
 * · 管理检查CPU模式
 * · 收集内存信息
 * · 设置内核栈
 * · 设置内核字体
 * · 建立内核MMU页表数据
 */
void init_hal(){
    
    //初始化平台
    init_halplatform();
    hint("平台初始化完毕\n");
    
    //复制文件
    move_img2maxpadr(&kmachbsp);
    hint("文件复制完毕\n");

    //初始化内存
    init_halmm();
    hint("内存初始化完毕\n");
    
    //初始化中断
    init_halintupt();
    hint("中断初始化完毕\n");

    return;
}