#include "cmctl.h"

/**
 * 加载内核映像文件
 */
void inithead_entry(){
    init_curs();
    close_curs();
    clear_screen(VGADP_DFVL);   //清空屏幕

    //将BIOS调用子模块加载到指定内存
    write_realintsvefile();

    //将二级引导器模块加载到指定内存中
    write_ldrkrlfile();

    return;
}


/**
 * 将调用BIOS的模块加载到指定内存中
 */
void write_realintsvefile(){
    fhdsc_t *fp = find_file("initldrsve.bin");  //实模式调用模块
    if (fp == NULL){
        error("not file initldrsve.bin");
    }
    m2mcopy((void *)((u32_t)(fp->fhd_intsfsoff) + LDRFILEADR),
            (void *)REALDRV_PHYADR, (sint_t)fp->fhd_frealsz);
    return;
}

/**
 * 在eki映像文件中查找文件
 */
fhdsc_t *find_file(char_t *fname){
    mlosrddsc_t *mrddadrs = MRDDSC_ADR;
    if (mrddadrs->mdc_endgic != MDC_ENDGIC ||
        mrddadrs->mdc_rv != MDC_RVGIC ||
        mrddadrs->mdc_fhdnr < 2 ||
        mrddadrs->mdc_filnr < 2){
        error("no mrddsc");
    }

    s64_t rethn = -1;
    fhdsc_t *fhdscstart = (fhdsc_t *)((u32_t)(mrddadrs->mdc_fhdbk_s) + LDRFILEADR);

    for (u64_t i = 0; i < mrddadrs->mdc_fhdnr; i++){
        if (strcmpl(fname, fhdscstart[i].fhd_name) == 0){
            rethn = (s64_t)i;
            goto ok_l;
        }
    }
    rethn = -1;
ok_l:
    if (rethn < 0){
        error("not find file");
    }
    return &fhdscstart[rethn];
}

/**
 * 将二级引导器核心文件加载到指定内存
 */
void write_ldrkrlfile(){
    fhdsc_t *fhdscstart = find_file("initldrkrl.bin");  //加载二级引导器文件
    if (fhdscstart == NULL){
        error("not file initldrkrl.bin");
    }
    m2mcopy((void *)((u32_t)(fhdscstart->fhd_intsfsoff) + LDRFILEADR),
            (void *)ILDRKRL_PHYADR, (sint_t)fhdscstart->fhd_frealsz);
    
    return;
}

/**
 * 错误处理
 */
void error(char_t *estr){
    kprint("INITLDR DIE ERROR:%s\n", estr);
    while(1) ;
    return;
}

/**
 * 同C库的strcmp()
 */
int strcmpl(const char *a, const char *b){
    while (*b && *a && (*b == *a)){
        b++;
        a++;
    }
    return *b - *a;
}