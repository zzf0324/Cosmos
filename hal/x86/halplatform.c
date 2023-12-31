/**
 * 平台相关的文件
 */

#include "cosmostypes.h"
#include "cosmosmctrl.h"

/**
 * 虚拟地址转换为物理地址
 */
adr_t virtadr_to_phyadr(adr_t kviradr){
    if (kviradr < KRNL_MAP_VIRTADDRESS_START || kviradr > KRNL_MAP_VIRTADDRESS_END){    //判断输入的虚拟地址是否在内核虚拟地址空间中
        system_error("virtual address convert to physical address error\n");
        return KRNL_ADDR_ERROR;
    }
    return kviradr - KRNL_MAP_VIRTADDRESS_START;    //求出虚拟地址相对于起始地址的偏移量就是映射后的物理地址
}

/**
 * 物理地址转换为虚拟地址
 */
adr_t phyadr_to_viradr(adr_t kphyadr){
    if (kphyadr >= KRNL_MAP_PHYADDRESS_END){
        system_error("phyadr_to_viradr err\n");
        return KRNL_ADDR_ERROR;
    }
    return kphyadr + KRNL_MAP_VIRTADDRESS_START;
}

/**
 * 初始化机器信息结构体machbstart_t
 */
void machbstart_t_init(machbstart_t *initp){
    //清零
    memset(initp, 0, sizeof(machbstart_t)); //把用于存储机器信息的结构体对应的内存空间初始化为0
    return;
}

/**
 * 把原先通过二级引导器获得的机器信息结构体machbstart_t,复制到一个新地址下
 */
void init_machbstart(){
    machbstart_t *kmbsp = &kmachbsp;    //全局机器信息结构体
    machbstart_t *smbsp = MBSPADR;      //二级引导器对应的物理地址1MB
    machbstart_t_init(kmbsp);           //初始化用于存储machbstart_t的结构体变量
    /**
     * 复制
     * 要把地址转换为虚拟地址，因为这个时候已经进入长模式+页表了
     * CPU执行程序时会使用虚拟地址，然后把这个虚拟地址转换为物理地址1MB
     * 首先原来的machbstart_t结构体是存放在物理地址1MB处的，如果memcopy将1MB当做源地址，这地址会被当做虚拟地址进行映射，虚拟地址1MB映射后的地址就不一定对应物理地址1MB了
     **/
    memcopy((void *)phyadr_to_viradr((adr_t)smbsp), (void *)kmbsp, sizeof(machbstart_t));   //由于用到了memcopy因此要保证二级引导器和这里的machbstart_t结构体是完全一样的
    return;
}

/**
 * 平台初始化函数
 */
void init_halplatform(){
    //复制机器信息结构，把原先的结构体内存赋值到全局变量的内存地址下(通过内存拷贝给一个变变量直接赋值)
    init_machbstart();

    //初始化图形显示驱动
    init_bdvideo();

    hint("图形驱动初始化完毕\n");    
    return;
}

/**
 * 字符串转换函数
 */
int strcmpl(const char *a, const char *b){
    while (*b && *a && (*b == *a)){
        b++;
        a++;
    }

    return *b - *a;
}

/**
 * 获取映像文件中的文件信息
 */
fhdsc_t *get_fileinfo(char_t *fname, machbstart_t *mbsp){
    mlosrddsc_t *mrddadrs = (mlosrddsc_t *)phyadr_to_viradr((adr_t)(mbsp->mb_imgpadr + MLOSDSC_OFF));
    if (mrddadrs->mdc_endgic != MDC_ENDGIC ||
        mrddadrs->mdc_rv != MDC_RVGIC ||
        mrddadrs->mdc_fhdnr < 2 ||
        mrddadrs->mdc_filnr < 2){
        system_error("no mrddsc");
    }
    s64_t rethn = -1;
    fhdsc_t *fhdscstart = (fhdsc_t *)((uint_t)((mrddadrs->mdc_fhdbk_s) + (phyadr_to_viradr((adr_t)mbsp->mb_imgpadr))));

    for (u64_t i = 0; i < mrddadrs->mdc_fhdnr; i++){
        if (strcmpl(fname, fhdscstart[i].fhd_name) == 0){
            rethn = (s64_t)i;
            goto ok_l;
        }
    }
    rethn = -1;
ok_l:
    if (rethn < 0){
        system_error("not find file");
    }
    return &fhdscstart[rethn];
}

/**
 * 获取映像中某文件的地址和大小
 */
void get_file_rvadrandsz(char_t *fname, machbstart_t *mbsp, u64_t *retadr, u64_t *retsz){
    u64_t padr = 0, fsz = 0;
    if (NULL == fname || NULL == mbsp){
        *retadr = 0;
        return;
    }
    fhdsc_t *fhdsc = get_fileinfo(fname, mbsp);
    if (fhdsc == NULL){
        *retadr = 0;
        return;
    }
    padr = fhdsc->fhd_intsfsoff + phyadr_to_viradr((adr_t)mbsp->mb_imgpadr);
    fsz = fhdsc->fhd_frealsz;

    *retadr = padr;
    *retsz = fsz;
    return;
}

/**
 * 待定
 */
e820map_t *get_maxmappadr_e820map(machbstart_t *mbsp, u64_t mappadr){
    if (NULL == mbsp){
        return NULL;
    }
    u64_t enr = mbsp->mb_e820nr;
    e820map_t *emp = (e820map_t *)phyadr_to_viradr((adr_t)mbsp->mb_e820padr);
    e820map_t *retemp = NULL;
    u64_t maxadr = emp[0].saddr;
    for (u64_t i = 0; i < enr; i++){
        if (emp[i].type == RAM_USABLE){
            if (emp[i].saddr > maxadr && (mappadr > (emp[i].saddr + emp[i].lsize))){
                maxadr = emp[i].saddr;
                retemp = &emp[i];
            }
        }
    }
    return retemp;
}

/**
 * 待定
 */
e820map_t *ret_kmaxmpadrcmpsz_e820map(machbstart_t *mbsp, u64_t mappadr, u64_t cpsz){
    if (NULL == mbsp){
        return NULL;
    }
    u64_t enr = mbsp->mb_e820nr;
    e820map_t *emp = (e820map_t *)phyadr_to_viradr((adr_t)mbsp->mb_e820padr);
    e820map_t *retemp = NULL;
    u64_t maxadr = emp[0].saddr;
    for (u64_t i = 0; i < enr; i++){
        if (emp[i].type == RAM_USABLE){
            if (emp[i].saddr >= maxadr && (mappadr > (emp[i].saddr + emp[i].lsize)) && (emp[i].lsize >= cpsz)){
                maxadr = emp[i].saddr;
                retemp = &emp[i];
            }
        }
    }
    if ((mappadr > (retemp->saddr + retemp->lsize)) && (retemp->lsize >= cpsz)){
        return retemp;
    }
    return NULL;
}

/**
 * 待定
 */
void move_img2maxpadr(machbstart_t *mbsp){
    u64_t kmapadrend = mbsp->mb_kpmapphymemsz;
    e820map_t *emp = ret_kmaxmpadrcmpsz_e820map(mbsp, kmapadrend, mbsp->mb_imgsz);
    if (NULL == emp){
        system_error("move_img2maxpadr1 emp not ok");
    }

    u64_t imgtoadr = (emp->saddr + (emp->lsize - mbsp->mb_imgsz));
    imgtoadr &= ~(0xfffUL);
    if (initchkadr_is_ok(mbsp, imgtoadr, mbsp->mb_imgsz) != 0){
        system_error("initchkadr_is_ok not ok\n");
    }
    void *sadr = (void *)phyadr_to_viradr((adr_t)mbsp->mb_imgpadr);
    void *dadr = (void *)phyadr_to_viradr((adr_t)imgtoadr);
    if (m2mcopy(sadr, dadr, (sint_t)(mbsp->mb_imgsz)) != ((sint_t)(mbsp->mb_imgsz))){
        system_error("move_img2maxpadr1 m2mcopy not ok");
    }
    mbsp->mb_imgpadr = imgtoadr;
    return;
}


/**
 * 函数:move_lmosimg2maxpadr中检查相应的地址有问题
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
 * 待定
 */
int initchkadr_is_ok(machbstart_t *mbsp, u64_t chkadr, u64_t cksz){
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
    if (adrzone_is_ok(mbsp->mb_e820expadr, mbsp->mb_e820exsz, chkadr, cksz) != 0){
        return -8;
    }
    if ((chkadr + cksz) >= mbsp->mb_kpmapphymemsz){
        return -9;
    }
    return 0;
}

#pragma GCC push_options    //针对GCC编译器的编译器指令，`push_options`指令保存当前的编译器选择项
#pragma GCC optimize("O0")  //将优化(Optimize)级别设置为最低0，也就是不进行任何优化，这意味着代码将在没有应用任何优化的情况下编译

/**
 * 延时函数
 */
void die(u32_t dt){
    u32_t dttt = dt, dtt = dt;
    if (dt == 0){   //die=0那么将进行无限循环
        for (;;)
            ;
    }

    for (u32_t i = 0; i < dt; i++){
        for (u32_t j = 0; j < dtt; j++){
            for (u32_t k = 0; k < dttt; k++){
                ;   //执行空语句
            }
        }
    }

    return;
}

#pragma GCC pop_options  //恢复之前保存的编译器选项