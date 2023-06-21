#include "cmctl.h"

/**
 * 二级引导器核心
 */
void ldrkrl_entry(){
    init_curs();
    close_curs();
    clear_screen(VGADP_DFVL);

    kprint("//======================= level 2 boostarp =======================//\n");
    init_bstartparm();

    return;
}


/**
 * 输出错误信息并终止执行
 */
void kerror(char_t* kestr){
    kprint("INITKLDR DIE ERROR:%s\n",kestr);
    while(1);
    return;
}
