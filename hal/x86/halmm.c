/**
 * HAL内存初始化头文件
 * phymmarge的作用就是把对e820数组进行遍历，将同类内存放在一块
 */

#include "cosmostypes.h"
#include "cosmosmctrl.h"


/**
 * 初始化phymmarge_t结构体
 */
void phymmarge_t_init(phymmarge_t *initp){
    if (NULL == initp){ //参数检查
        return;
    }
    //设置自旋锁
    hal_spinlock_init(&initp->pmr_lock);
    //清空各字段
    initp->pmr_type = 0;
    initp->pmr_stype = 0;
    initp->pmr_dtype = 0;
    initp->pmr_flgs = 0;
    initp->pmr_stus = 0;
    initp->pmr_saddr = 0;
    initp->pmr_lsize = 0;
    initp->pmr_end = 0;
    initp->pmr_rrvmsaddr = 0;
    initp->pmr_rrvmend = 0;
    initp->pmr_prip = NULL;
    initp->pmr_extp = NULL;
    return;
}


/**
 * 获取phymmarge的地址和大小
 */
void ret_phymmarge_adrandsz(machbstart_t *mbsp, phymmarge_t **retpmrvadr, u64_t *retpmrsz){
    if (NULL == mbsp || 0 == mbsp->mb_e820sz || NULL == mbsp->mb_e820padr || 0 == mbsp->mb_e820nr){
        *retpmrsz = 0;
        *retpmrvadr = NULL;
        return;
    }
    u64_t tmpsz = mbsp->mb_e820nr * sizeof(phymmarge_t);
    u64_t tmpphyadr = mbsp->mb_nextwtpadr;
    if (0 != initchkadr_is_ok(mbsp, tmpphyadr, tmpsz)){
        *retpmrsz = 0;
        *retpmrvadr = NULL;
        return;
    }
    *retpmrsz = tmpsz;
    *retpmrvadr = (phymmarge_t *)((adr_t)tmpphyadr);
    return;
}


/**
 * 把e820中的信息复制到phymmarge_t结构中来
 */
bool_t init_one_pmrge(e820map_t *e8p, phymmarge_t *pmargep){
    u32_t type = 0, stype = 0;
    if (NULL == e8p || NULL == pmargep){    //参数检查
        return FALSE;
    }
    //初始化phymmarge_t结构体
    phymmarge_t_init(pmargep);
    switch (e8p->type){
    case RAM_USABLE:    //可用内存
        type = PMR_T_OSAPUSERRAM;
        stype = RAM_USABLE;
        break;
    case RAM_RESERV:    //保留内存
        type = PMR_T_RESERVRAM;
        stype = RAM_RESERV;
        break;
    case RAM_ACPIREC:   //ACPI data
        type = PMR_T_HWUSERRAM;
        stype = RAM_ACPIREC;
        break;
    case RAM_ACPINVS:   //APCI NVS
        type = PMR_T_HWUSERRAM;
        stype = RAM_ACPINVS;
        break;
    case RAM_AREACON:   //不可用内存
        type = PMR_T_BUGRAM;
        stype = RAM_AREACON;
        break;
    default:
        break;
    }
    if (0 == type){
        return FALSE;
    }
    //填写相应的字段
    pmargep->pmr_type = type;
    pmargep->pmr_stype = stype;
    pmargep->pmr_flgs = PMR_F_X86_64;
    pmargep->pmr_saddr = e8p->saddr;    //内存区域起始地址
    pmargep->pmr_lsize = e8p->lsize;    //内存区域大小
    pmargep->pmr_end = e8p->saddr + e8p->lsize - 1; //内存区域结束地址
    return TRUE;
}

/**
 * 交换两个phymmarge_t结构体对象
 */
static void phymmarge_swap(phymmarge_t *src, phymmarge_t *dst){
    phymmarge_t tmp;
    phymmarge_t_init(&tmp);
    memcopy(src, &tmp, sizeof(phymmarge_t));
    memcopy(dst, src, sizeof(phymmarge_t));
    memcopy(&tmp, dst, sizeof(phymmarge_t));
    return;
}

/**
 * 对phymmarge_t结构体中的地址空间进行升序排序
 */
void phymmarge_sort(phymmarge_t *argp, u64_t nr){
    u64_t i, j, k = nr - 1;
    //交换排序
    for (j = 0; j < k; j++){
        for (i = 0; i < k - j; i++){
            if (argp[i].pmr_saddr > argp[i + 1].pmr_saddr){
                phymmarge_swap(&argp[i], &argp[i + 1]);
            }
        }
    }
    return;
}


/**
 * 
 */
u64_t initpmrge_core(e820map_t *e8sp, u64_t e8nr, phymmarge_t *pmargesp){
    u64_t retnr = 0;
    if (NULL == e8sp || NULL == pmargesp || e8nr < 1){  //参数检查
        return 0;
    }
    //循环遍历e820数组元素
    for (u64_t i = 0; i < e8nr; i++){
        //根据收集的e820数组建立一个phymmarge_t结构
        if (init_one_pmrge(&e8sp[i], &pmargesp[i]) == FALSE){
            return retnr;
        }
        retnr++;
    }
    return retnr;
}

/**
 * 
 */
void init_phymmarge(){
    machbstart_t *mbsp = &kmachbsp; //全局机器信息结构
    phymmarge_t *pmarge_adr = NULL; //开始地址
    u64_t pmrgesz = 0;              //大小
    //根据machbstart_t机器信息结构计算获得phymmarge_t结构的开始地址和大小
    ret_phymmarge_adrandsz(mbsp, &pmarge_adr, &pmrgesz);
    if (NULL == pmarge_adr || 0 == pmrgesz){
        system_error("init_phymmarge->NULL==pmarge_adr||0==pmrgesz\n");
        return;
    }
    u64_t tmppmrphyadr = mbsp->mb_nextwtpadr;
    if ((adr_t)tmppmrphyadr != ((adr_t)pmarge_adr)){
        system_error("init_phymmarge->tmppmrphyadr!=pmarge_adr2phyadr\n");
        return;
    }
    e820map_t *e8p = (e820map_t *)((adr_t)(mbsp->mb_e820padr));
    //建立phymmarge_t结构
    u64_t ipmgnr = initpmrge_core(e8p, mbsp->mb_e820nr, pmarge_adr);
    if (ipmgnr == 0){
        system_error("init_phymmarge->initpmrge_core ret 0\n");
        return;
    }
    if ((ipmgnr * sizeof(phymmarge_t)) != pmrgesz){
        system_error("init_phymmarge->ipmgnr*sizeof(phymmarge_t))!=pmrgesz\n");
        return;
    }
    //把phymmarge_t结构地址大小个数保存到machbstart_t机器信息结构体
    mbsp->mb_e820expadr = tmppmrphyadr;
    mbsp->mb_e820exnr = ipmgnr;
    mbsp->mb_e820exsz = ipmgnr * sizeof(phymmarge_t);
    mbsp->mb_nextwtpadr = PAGE_ALIGN(mbsp->mb_e820expadr + mbsp->mb_e820exsz);    //4k对齐
    //phymmarge_t结构中的地址空间从低到高进行排序
    phymmarge_sort(pmarge_adr, ipmgnr);
    return;
}

/**
 * 内存初始化总控函数
 */
void init_halmm(){
    init_phymmarge();
    //init_memmgr();        //内存管理函数
    return;
}