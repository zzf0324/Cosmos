/**
 * 平台相关的宏定义文件
 */

#ifndef _PLATFORM_H
#define _PLATFORM_H


/**
 * 虚拟地址转换为物理地址
 */
adr_t viradr_to_phyadr(adr_t kviradr);

/**
 * 物理地址转换为虚拟地址
 */
adr_t phyadr_to_viradr(adr_t kphyadr);

/**
 * 初始化机器信息
 */
void machbstart_t_init(machbstart_t *initp);

/**
 * 复制机器信息结构到指定内存中
 */
void init_machbstart();

/**
 * 初始化硬件平台
 */
void init_halplatform();

/**
 *
 */
fhdsc_t *get_fileinfo(char_t *fname, machbstart_t *mbsp);

/**
 * 
 */
void get_file_rvadrandsz(char_t *fname, machbstart_t *mbsp, u64_t *retadr, u64_t *retsz);

/**
 * 
 */
void move_img2maxpadr(machbstart_t *mbsp);

/**
 * 
 */
int adrzone_is_ok(u64_t sadr, u64_t slen, u64_t kadr, u64_t klen);

/**
 * 
 */
int initchkadr_is_ok(machbstart_t *mbsp, u64_t chkadr, u64_t cksz);

#endif
