/**
 * 平台相关的宏定义文件
 */

#ifndef _PLATFORM_T_H
#define _PLATFORM_T_H


#include "bdvideo_t.h"

#ifdef CFG_X86_PLATFORM
#define CPUCORE_MAX 1
#define SDRAM_MAPVECTPHY_ADDR 0x30000000

#define KRNL_INRAM_START 0x30000000
#define LINK_VIRT_ADDR 0x30008000
#define LINK_LINE_ADDR 0x30008000
#define KERNEL_VIRT_ADDR 0x30008000
#define KERNEL_PHYI_ADDR 0x30008000
#define PAGE_TLB_DIR 0x30004000
#define PAGE_TLB_SIZE 4096
#define INIT_HEAD_STACK_ADDR 0x34000000

#define CPU_VECTOR_PHYADR 0x30000000
#define CPU_VECTOR_VIRADR 0


#define PTE_SECT_AP (3<<10)
#define PTE_SECT_DOMAIN (0<<5)
#define PTE_SECT_NOCW (0<<2)
#define PTE_SECT_BIT (2)

#define PLFM_ADRSPCE_NR 29

#define INTSRC_MAX 32
/*======================================================================================================================================*/
#define KRNL_VIRTUAL_ADDRESS_START 0xffff800000000000           //虚拟地址空间起始地址
#define KRNL_VIRTUAL_ADDRESS_END 0xffffffffffffffff             //虚拟地址空间结束地址
#define KRNL_MAP_VIRTADDRESS_SIZE 0x400000000                   //虚拟地址空间大小16GB

#define USER_VIRTUAL_ADDRESS_START 0                            //用户虚拟地址空间起始地址
#define USER_VIRTUAL_ADDRESS_END 0x00007fffffffffff             //用户虚拟地址空间结束地址

#define KRNL_MAP_PHYADDRESS_START 0                             //物理地址空间起始地址
#define KRNL_MAP_PHYADDRESS_END 0x400000000                     //物理地址空间结束地址
#define KRNL_MAP_PHYADDRESS_SIZE 0x400000000                    //物理地址空间大小16GB

#define KRNL_MAP_VIRTADDRESS_START KRNL_VIRTUAL_ADDRESS_START   //内核虚拟地址空间映射用的起始地址
#define KRNL_MAP_VIRTADDRESS_END (KRNL_MAP_VIRTADDRESS_START+KRNL_MAP_VIRTADDRESS_SIZE)     //内核虚拟地址空间映射用的结束地址
#define KRNL_ADDR_ERROR 0xf800000000000                         //表示一个出错地址，当地址映射出错时返回该地址


/*======================================================================================================================================*/
#define MBSPADR ((machbstart_t*)(0x100000))     //机器信息结构体地址:1MB处
//机器信息结构体魔数
#define MBS_MIGC (u64_t)((((u64_t)'L')<<56)|(((u64_t)'M')<<48)|(((u64_t)'O')<<40)|(((u64_t)'S')<<32)|(((u64_t)'M')<<24)|(((u64_t)'B')<<16)|(((u64_t)'S')<<8)|((u64_t)'P'))
//机器信息结构数组
typedef struct s_MACHBSTART{
    u64_t   mb_migc;            //魔数
    u64_t   mb_chksum;          //校验和
    u64_t   mb_krlinitstack;    //内核栈地址
    u64_t   mb_krlitstacksz;    //内核栈大小
    u64_t   mb_imgpadr;         //操作系统映像(eki文件)地址
    u64_t   mb_imgsz;           //操作系统映像大小
    u64_t   mb_krlimgpadr;      //内核(Cosmos.bin)地址
    u64_t   mb_krlsz;           //内核(Cosmos.bin)大小
    u64_t   mb_krlvec;          //
    u64_t   mb_krlrunmode;      //
    u64_t   mb_kalldendpadr;    //
    u64_t   mb_ksepadrs;        //
    u64_t   mb_ksepadre;        //
    u64_t   mb_kservadrs;       //
    u64_t   mb_kservadre;       //
    u64_t   mb_nextwtpadr;      //下一段空闲内存首地址（4K对齐）
    u64_t   mb_bfontpadr;       //操作系统字体地址
    u64_t   mb_bfontsz;         //操作系统字体大小
    u64_t   mb_fvrmphyadr;      //机器显存地址
    u64_t   mb_fvrmsz;          //机器显存大小
    u64_t   mb_cpumode;         //CPU工作模式
    u64_t   mb_memsz;           //机器内存大小
    u64_t   mb_e820padr;        //机器e80数组地址
    u64_t   mb_e820nr;          //机器e80数组元素个数
    u64_t   mb_e820sz;          //机器e80数组大小
    u64_t   mb_e820expadr;      //
    u64_t   mb_e820exnr;        //
    u64_t   mb_e820exsz;        //
    u64_t   mb_memznpadr;       //
    u64_t   mb_memznnr;         //
    u64_t   mb_memznsz;         //
    u64_t   mb_memznchksum;     //
    u64_t   mb_memmappadr;      //
    u64_t   mb_memmapnr;        //
    u64_t   mb_memmapsz;        //
    u64_t   mb_memmapchksum;    //
    u64_t   mb_pml4padr;        //
    u64_t   mb_subpageslen;     //
    u64_t   mb_kpmapphymemsz;   //
    graph_t mb_ghparm;          //
}__attribute__((packed)) machbstart_t;


#define BFH_RW_R 1
#define BFH_RW_W 2

#define BFH_BUF_SZ 0x1000
#define BFH_ONERW_SZ 0x1000
#define BFH_RWONE_OK 1
#define BFH_RWONE_ER 2
#define BFH_RWALL_OK 3

/*=======================================================================================================================================*/
#define RAM_USABLE 1        //可用内存
#define RAM_RESERV 2        //保留内存
#define RAM_ACPIREC 3       //ACPI data
#define RAM_ACPINVS 4       //ACPI NVS
#define RAM_AREACON 5       //不可用内存
//内存视图的e820数组
typedef struct s_e820{
    u64_t saddr;        //内存段起始地址
    u64_t lsize;        //内存段大小
    u32_t type;         //内存段类型
}__attribute__((packed)) e820map_t;


/*========================================================================================================================================*/
#define FHDSC_NMAX 192                  //文件头描述符中文件名最大长度
#define FHDSC_SZMAX 256                 //文件头描述符大小256B=64B+192B
#define MDC_ENDGIC 0xaaffaaffaaffaaff   //映像文件结束标志:mlosrddsc_t.mdc_endgic
#define MDC_RVGIC 0xffaaffaaffaaffaa    //映像文件版本:mlosrddsc_t.mdc_rv
#define MLOSDSC_OFF (0x1000)            //eki中文件描述符偏移

//文件句柄
typedef struct s_fhdsc{
    u64_t fhd_type;
    u64_t fhd_subtype;
    u64_t fhd_stuts;
    u64_t fhd_id;
    u64_t fhd_intsfsoff;
    u64_t fhd_intsfend;
    u64_t fhd_frealsz;
    u64_t fhd_fsum;
    char   fhd_name[FHDSC_NMAX];
}fhdsc_t;

//映像文件描述符
typedef struct s_mlosrddsc{
    u64_t mdc_mgic;
    u64_t mdc_sfsum;
    u64_t mdc_sfsoff;
    u64_t mdc_sfeoff;
    u64_t mdc_sfrlsz;

    u64_t mdc_ldrbk_s;
    u64_t mdc_ldrbk_e;
    u64_t mdc_ldrbk_rsz;
    u64_t mdc_ldrbk_sum;

    u64_t mdc_fhdbk_s;
    u64_t mdc_fhdbk_e;
    u64_t mdc_fhdbk_rsz;
    u64_t mdc_fhdbk_sum;
    
    u64_t mdc_filbk_s;
    u64_t mdc_filbk_e;
    u64_t mdc_filbk_rsz;
    u64_t mdc_filbk_sum;
    
    u64_t mdc_ldrcodenr;
    
    u64_t mdc_fhdnr;
    u64_t mdc_filnr;
    u64_t mdc_endgic;
    u64_t mdc_rv;
}mlosrddsc_t;
#endif


#endif 
