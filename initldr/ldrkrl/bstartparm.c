/**
 * 实现收集机器信息建立页面数据
 * 起到一个总控的作用，bstartparm.c文件并不具体实现每一个初始化函数，而是统一调用
 */

#include "cmctl.h"

void init_bstartparm(){
    display();
    
    machbstart_t *mbsp = MBSPADR;   //1MB的内存地址,专门用来存放机器信息信息结构体bmachstart
    
    //初始化机器信息结构体
    machbstart_t_init(mbsp);        							        				
    //在1MB内存地址处，初始化一个用于保存机器信息的结构体machbstart_t，后续的收集到的信息都放在这个结构体中
    kprint("[OK] finishing initializing the machbstart_t.\n");
    
    //检查CPU：能执行什么形式的代码？，支持长模式吗？
    init_chkcpu(mbsp);
    kprint("[OK] finishing checking CPU.\n");
    
    //获取内存布局e820数组
    init_mem(mbsp);
    if (0 == get_wt_imgfilesz(mbsp)){   //获取映像文件Cosmos.eki的大小
        kerror("img file size is 0");
    }
    kprint("[OK] finishing obtaining the memory view.\n");
    
    //初始化内核栈
    init_krlstack(mbsp);
    kprint("[OK] finishing initializing the core stack.\n");
    
    //放置内核文件
    init_krlfile(mbsp);
    kprint("[OK] finishing setting the kernel file.\n");
    
    //放置字库文件
    init_defutfont(mbsp);
    kprint("[OK] finishing setting the font file.\n");
    
    //初始化e820数组
    init_meme820(mbsp);
    kprint("[OK] finishing initializing the e820 array.\n");
    
    //建立MMU页表
    init_bstartpages(mbsp);
    kprint("[OK] finishing establishing the MMU page table.\n");
    
    die(0x0);     //延时
    //设置图形模式
    init_graph(mbsp);
    return;
}

/**
 * 初始化machbstart_t结构体，清零，并设置一个标志
 */
void machbstart_t_init(machbstart_t *initp){
    memset(initp, 0, sizeof(machbstart_t)); //在mbspadr地址下初始化一片内存空间作为mbspstart_t的内存，（必须初始化清零）
    initp->mb_migc = MBS_MIGC;  //打上标志：machbstart_t首部放入模数
    return;
}

/**
 * 功能待定
 */
int adrzone_is_ok(u64_t sadr, u64_t slen, u64_t kadr, u64_t klen){
    if (kadr >= sadr && kadr <= (sadr + slen)){
        return -1;
    }
    if (kadr <= sadr && ((kadr + klen) >= sadr)){
        return -2;
    }

    return 0;
}

/**
 * 功能待定
 */
int chkadr_is_ok(machbstart_t *mbsp, u64_t chkadr, u64_t cksz){
    //u64_t len=chkadr+cksz;
    if (adrzone_is_ok((mbsp->mb_krlinitstack - mbsp->mb_krlitstacksz), mbsp->mb_krlitstacksz, chkadr, cksz) != 0){
        return -1;
    }
    if (adrzone_is_ok(mbsp->mb_imgpadr, mbsp->mb_imgsz, chkadr, cksz) != 0){
        return -2;
    }
    if (adrzone_is_ok(mbsp->mb_krlimgpadr, mbsp->mb_krlsz, chkadr, cksz) != 0){
        return -3;
    }
    if (adrzone_is_ok(mbsp->mb_bfontpadr, mbsp->mb_bfontsz, chkadr, cksz) != 0){
        return -4;
    }
    if (adrzone_is_ok(mbsp->mb_e820padr, mbsp->mb_e820sz, chkadr, cksz) != 0){
        return -5;
    }
    if (adrzone_is_ok(mbsp->mb_memznpadr, mbsp->mb_memznsz, chkadr, cksz) != 0){
        return -6;
    }
    if (adrzone_is_ok(mbsp->mb_memmappadr, mbsp->mb_memmapsz, chkadr, cksz) != 0){
        return -7;
    }
    return 0;
}
