/**
 * 系统全局内存和CPU检查头文件
 */

#ifndef _CHKCPMM_H
#define _CHKCPMM_H


/**
 * 
 */
void init_mem(machbstart_t* mbsp);

/**
 * 
 */
void init_krlstack(machbstart_t* mbsp);

/**
 * 
 */
void init_meme820(machbstart_t* mbsp);

/**
 * 
 */
void init_chkcpu(machbstart_t* mbsp);

/**
 * 
 */
void mmap(e820map_t** retemp,u32_t* retemnr);

/**
 * 
 */
int chk_cpuid();

/**
 * 
 */
int chk_cpu_longmode();

/**
 * 
 */
e820map_t* chk_memsize(e820map_t* e8p,u32_t enr,u64_t sadr,u64_t size);

/**
 * 
 */
u64_t get_memsize(e820map_t* e8p,u32_t enr);

/**
 * 
 */
void init_bstartpages(machbstart_t *mbsp);

/**
 * 主动死机: 禁用中断，并使处理器进入停机状态，它会使处理器停止执行指令
 */
#define CLI_HALT() __asm__ __volatile__("cli; hlt": : :"memory")


#endif 
