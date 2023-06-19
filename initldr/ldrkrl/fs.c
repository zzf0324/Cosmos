#include "cmctl.h"

/**
 * 字符串比较函数
 */
int strcmpl(const char *a, const char *b){
    while (*b && *a && (*b == *a)){
        b++;
        a++;
    }
    //循环退出，说明:1.有字符串提前结束；2.字符串出现不相等的字符
    return *b - *a;     //为什么不是*a - *b？
}


/**
 * 输出映像文件信息，包括映像文件头描述符，文件头描述符等
 */
void display(){
    mlosrddsc_t *mrddadrs = MRDDSC_ADR; //获取映像头描述符
    if (mrddadrs->mdc_endgic != MDC_ENDGIC ||
        mrddadrs->mdc_rv != MDC_RVGIC ||
        mrddadrs->mdc_fhdnr < 2 ||
        mrddadrs->mdc_filnr < 2){
        kerror("image file error");
    }

    fhdsc_t *fhdsc_p = (fhdsc_t *)((u32_t)(mrddadrs->mdc_fhdbk_s) + LDRFILEADR);

    //输出文件头描述符信息
    for(u64_t i=0;i<mrddadrs->mdc_fhdnr;i++){
        hint(fhdsc_p[i].fhd_name);
        hint("    %d\n", fhdsc_p[i].fhd_frealsz);
    }
    
    return ;
}


/**
 * 从映像eki中查找指定文件fname，返回文件句柄
 * 因为是32位C语言环境，所以这里面的地址使用u32_t
 */
fhdsc_t *get_fileinfo(char_t *fname, machbstart_t *mbsp){
    //映像文件头描述符结构体内存地址(源自直接保存在文件中的数据)
    mlosrddsc_t *mrddadrs = (mlosrddsc_t *)((u32_t)(mbsp->mb_imgpadr + MLOSDSC_OFF));   //32位模式,用32位地址u32_t
    //根据mlosrddsc_t校验字段检查映像文件
    if (mrddadrs->mdc_endgic != MDC_ENDGIC ||
        mrddadrs->mdc_rv != MDC_RVGIC ||
        mrddadrs->mdc_fhdnr < 2 ||
        mrddadrs->mdc_filnr < 2){
        kerror("no mrddsc");
    }

    s64_t rethn = -1;    //所匹配数组元素的下标
    //文件头描述符结构体指针(通过指针后移来遍历映像文件中的所有文件头描述)
    fhdsc_t *fhdscstart = (fhdsc_t *)((u32_t)(mrddadrs->mdc_fhdbk_s) + ((u32_t)(mbsp->mb_imgpadr)));

    //数组查找：遍历每一个文件头描述符，查看名字信息是否匹配
    for (u64_t i = 0; i < mrddadrs->mdc_fhdnr; i++){
        if (strcmpl(fname, fhdscstart[i].fhd_name) == 0){
            rethn = (s64_t)i;
            goto ok_l;
        }
    }
    rethn = -1;
ok_l:
    if (rethn < 0){ //查找失败
        kerror("not find file");
    }
    return &fhdscstart[rethn];    //返回查找到的文件头描述符地址：fhdscstart+rethn    
}

/**
 * 将内核文件移动到指定内存中
 * 主要用于判断一个地址空间，是否和内存中存放的内容有冲突
 */
int move_krlimg(machbstart_t *mbsp, u64_t cpyadr, u64_t cpysz){
    if (0xffffffff <= (cpyadr + cpysz) || 1 > cpysz){
        return 0;
    }
    void *toadr = (void *)((u32_t)(P4K_ALIGN(cpyadr + cpysz)));
    sint_t tosz = (sint_t)mbsp->mb_imgsz;
    if (0 != adrzone_is_ok(mbsp->mb_imgpadr, mbsp->mb_imgsz, cpyadr, cpysz)){
        if (NULL == chk_memsize((e820map_t *)((u32_t)(mbsp->mb_e820padr)), (u32_t)mbsp->mb_e820nr, (u64_t)((u32_t)toadr), (u64_t)tosz)){
            return -1;
        }
        m2mcopy((void *)((u32_t)mbsp->mb_imgpadr), toadr, tosz);
        mbsp->mb_imgpadr = (u64_t)((u32_t)toadr);
        return 1;
    }
    return 2;
}

/**
 * 放置内核文件
 */
void init_krlfile(machbstart_t *mbsp){
    //在映像中查找相应的文件，并复制到对应的地址上，返回文件的大小，这里查找kernel.bin文件
    u64_t sz = r_file_to_padr(mbsp, IMGKRNL_PHYADR, "Cosmos.bin");
    if (0 == sz){
        kerror("r_file_to_padr err");
    }
    //放置完后，更新机器信息中的数据
    mbsp->mb_krlimgpadr = IMGKRNL_PHYADR;
    mbsp->mb_krlsz = sz;
    //mbsp->mb_nextwtpadr始终指向下一段内存空间的首地址
    mbsp->mb_nextwtpadr = P4K_ALIGN(mbsp->mb_krlimgpadr + mbsp->mb_krlsz);  //根据尾地址，获取下一个4K对齐的地址
    mbsp->mb_kalldendpadr = mbsp->mb_krlimgpadr + mbsp->mb_krlsz;
    return;
}

/**
 * 放置字库文件
 */
void init_defutfont(machbstart_t *mbsp){
    u64_t sz = 0;
    //获取下一段空闲的内存空间首地址
    u32_t dfadr = (u32_t)mbsp->mb_nextwtpadr;
    //在映像中查找相应的文件，并复制到对应的地址，并返回文件大小，这里查找的是font.fnt文件
    sz = r_file_to_padr(mbsp, dfadr, "font.fnt");
    if (0 == sz){
        kerror("r_file_to_padr err");
    }
    //放置完成后更新及其信息结构中的数据
    mbsp->mb_bfontpadr = (u64_t)(dfadr);
    mbsp->mb_bfontsz = sz;
    //更新机器结构中下一段空闲内存首地址
    mbsp->mb_nextwtpadr = P4K_ALIGN((u32_t)(dfadr) + sz);   //保持内存的4K对齐，也就是间隙产生
    mbsp->mb_kalldendpadr = mbsp->mb_bfontpadr + mbsp->mb_bfontsz;
    return;
}

/**
 * 获取文件真实地址和大小
 * fname文件名
 * retadr返回文件地址，通过地址传递返回值
 * retsz返回文件大小，通过地址传递返回值
 */
void get_file_rpadrandsz(char_t *fname, machbstart_t *mbsp, u32_t *retadr, u32_t *retsz){
    u64_t padr = 0, fsz = 0;
    if (NULL == fname || NULL == mbsp){ //传入参数为空,查找失败
        *retadr = 0;    //返回NULL，地址为NULL说明文件没找到，size也就没有必要了
        return;
    }
    fhdsc_t *fhdsc = get_fileinfo(fname, mbsp);    //获取文件信息(实际就是文件句柄fhdsc_t)
    if (fhdsc == NULL){ //获取失败
        *retadr = 0;
        return;
    }
    padr = mbsp->mb_imgpadr + fhdsc->fhd_intsfsoff; //文件物理地址：eki基地址+文件eki内偏移
    if (padr > 0xffffffff){     //地址超过u32_t最大值，地址越界
        *retadr = 0;
        return;
    }
    fsz = (u32_t)fhdsc->fhd_frealsz;
    if (fsz > 0xffffffff){      //文件地址超过的u32_t最大值，文件过大
        *retadr = 0;
        return;
    }
    //这里定义的padr,fsz是u64_t类型，因为eki文件中的偏移是u64_t类型，但是程序现在是32位环境，需要使用32位地址，所以进行强转
    *retadr = (u32_t)padr;
    *retsz = (u32_t)fsz;
    return;
}

/**
 * 获取映像中fname文件大小
 */
u64_t get_filesz(char_t *fname, machbstart_t *mbsp){
    if (fname == NULL || mbsp == NULL){ //参数检查
        return 0;
    }
    fhdsc_t *fhdscstart = get_fileinfo(fname, mbsp);    //获取文件句柄
    if (fhdscstart == NULL){
        return 0;
    }
    return fhdscstart->fhd_frealsz; //返回文件大小（Byte）
}

/**
 * 获取映像Cosmos.eki的大小
 */
u64_t get_wt_imgfilesz(machbstart_t *mbsp){
    u64_t retsz = LDRFILEADR;   //映像文件地址
    mlosrddsc_t *mrddadrs = MRDDSC_ADR; //获取映像文件头描述符地址
    //映像文件校验：使用校验字段
    if (mrddadrs->mdc_endgic != MDC_ENDGIC ||
        mrddadrs->mdc_rv != MDC_RVGIC ||
        mrddadrs->mdc_fhdnr < 2 ||
        mrddadrs->mdc_filnr < 2){
        return 0;
    }
    //有什么用？eki映像文件前面16K可能是保留区域，不能用于文件数据存储
    if (mrddadrs->mdc_filbk_e < 0x4000){    //0x4000=16K
        return 0;
    }

    retsz += mrddadrs->mdc_filbk_e; //文件结束偏移对应映像文件的最后一个子节的偏移地址，所以他对应的就是文件的大小
    retsz -= LDRFILEADR;
    mbsp->mb_imgpadr = LDRFILEADR;  //机器信息结构体中记录eki文件内存地址
    mbsp->mb_imgsz = retsz;         //机器信息结构体中记录eki文件大小
    return retsz;
}

/**
 * 将映像eki中指定名称的文件复制到指定的内存中,并返回其大小
 */
u64_t r_file_to_padr(machbstart_t *mbsp, u32_t f2adr, char_t *fname){
    if (NULL == f2adr || NULL == fname || NULL == mbsp){    //参数检查
        return 0;
    }
    u32_t fpadr = 0, sz = 0;
    get_file_rpadrandsz(fname, mbsp, &fpadr, &sz);  //获取文件物理地址和大小并存入fpadr和sz
    if (0 == fpadr || 0 == sz){ //文件参数检查
        return 0;
    }
    //检查内存是否足够
    if (NULL == chk_memsize((e820map_t *)((u32_t)mbsp->mb_e820padr), (u32_t)(mbsp->mb_e820nr), f2adr, sz)){
        return 0;
    }
    //检查地址和大小是否可被接受
    if (0 != chkadr_is_ok(mbsp, f2adr, sz)){
        return 0;
    }
    //完成检查后将fname指定的文件从映像文件中拷贝到指定内存f2adr中
    m2mcopy((void *)fpadr, (void *)f2adr, (sint_t)sz);
    return sz;  //返回拷贝字节数，返回0说明拷贝失败
}