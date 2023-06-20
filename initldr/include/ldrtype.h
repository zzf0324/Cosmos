/**
 * 加载器基本类型定义头文件
 */

#ifndef _LDRTYPE_H
#define _LDRTYPE_H


/*========================================================================================================================*/
#define REALDRV_PHYADR 0x1000           //实模式模块initldrsve.bin地址
#define ILDRKRL_PHYADR 0x200000         //二级引导器initldrkrl.bin地址
#define IMGKRNL_PHYADR 0x2000000        //内核Cosmos.bin地址
#define IMGSHEL_PHYADR 0x30000          //

#define IKSTACK_PHYADR (0x90000-0x10)   //内核栈基地址
#define IKSTACK_SIZE 0x1000             //内核栈大小:4KB

#define KINITFRVM_PHYADR 0x800000       //内核初始化帧缓冲区地址
#define KINITFRVM_SZ 0x400000           //内核初始化帧缓冲区偏移

#define IMGFILE_PHYADR 0x4000000        //映像文件Cosmos.eki基地址
#define LDRFILEADR IMGFILE_PHYADR       //映像文件地址
#define MLOSDSC_OFF (0x1000)            //mlosrddsc_t的偏移地址4KB，OFF是offset    
#define MRDDSC_ADR (mlosrddsc_t*)(LDRFILEADR+0x1000)    //映像文件头描述符地址(映像文件地址偏移4KB=映像文件头描述符的地址)


/*===========================================================eki映像相关的结构==============================================*/
#define FHDSC_NMAX 192                  //文件头描述符中文件名最大长度
#define FHDSC_SZMAX 256                 //文件头描述符大小256B=64B+192B
#define MDC_ENDGIC 0xaaffaaffaaffaaff   //映像文件结束标志:mlosrddsc_t.mdc_endgic
#define MDC_RVGIC 0xffaaffaaffaaffaa    //映像文件版本:mlosrddsc_t.mdc_rv
//文件头描述符，用于描述内核映像中的文件
typedef struct s_fhdsc{
    u64_t fhd_type;             //文件类型
    u64_t fhd_subtype;          //文件子类型
    u64_t fhd_stuts;            //文件状态
    u64_t fhd_id;               //文件id
    u64_t fhd_intsfsoff;        //文件在映像文件中的开始偏移
    u64_t fhd_intsfend;         //文件在映像文件中的结束偏移
    u64_t fhd_frealsz;          //文件实际大小
    u64_t fhd_fsum;             //文件校验和
    char  fhd_name[FHDSC_NMAX]; //文件名
}fhdsc_t;

//映像文件头描述符，用与内核映像文件eki中的各个文件
typedef struct s_mlosrddsc{
    u64_t mdc_mgic;         //映像文件魔数
    u64_t mdc_sfsum;        //未使用
    u64_t mdc_sfsoff;       //未使用
    u64_t mdc_sfeoff;       //未使用
    u64_t mdc_sfrlsz;       //未使用
    //二级引导器initldrkrl.bin
    u64_t mdc_ldrbk_s;      //映像文件中二级引导器的开始偏移
    u64_t mdc_ldrbk_e;      //映像文件中二级引导器的结束偏移
    u64_t mdc_ldrbk_rsz;    //映像文件中二级引导器的实际大小
    u64_t mdc_ldrbk_sum;    //映像文件中二级引导器的校验和
    //file head description
    u64_t mdc_fhdbk_s;      //映像文件中文件头描述的开始偏移
    u64_t mdc_fhdbk_e;      //映像文件中文件头描述的结束偏移
    u64_t mdc_fhdbk_rsz;    //映像文件中文件头描述的实际大小
    u64_t mdc_fhdbk_sum;    //映像文件中文件头描述的校验和
    //data
    u64_t mdc_filbk_s;      //映像文件中文件数据的开始偏移
    u64_t mdc_filbk_e;      //映像文件中文件数据的结束偏移
    u64_t mdc_filbk_rsz;    //映像文件中文件数据的实际大小
    u64_t mdc_filbk_sum;    //映像文件中文件数据的校验和
    //others
    u64_t mdc_ldrcodenr;    //映像文件中二级引导器的文件头描述符的索引号
    //校验字段
    u64_t mdc_fhdnr;        //映像文件中文件头描述符有多少个
    u64_t mdc_filnr;        //映像文件中文件头有多少个
    u64_t mdc_endgic;       //映像文件结束标志
    u64_t mdc_rv;           //映像文件版本
}mlosrddsc_t;


/*==========================================================MMU页表相关的数据==================================================*/
#define KINITPAGE_PHYADR 0x1000000      //页表首地址:16MB处
#define KRNL_VIRTUAL_ADDRESS_START 0xffff800000000000   //内核虚拟地址空间起始地址
//顶级页目录标志字段
#define KPML4_P (1<<0)          //页面存在位
#define KPML4_RW (1<<1)         //页面读写位
#define KPML4_US (1<<2)         //用户/超级管理者页
#define KPML4_PWT (1<<3)        //写回，写直达
#define KPML4_PCD (1<<4)        //是否开启页级Cache
#define KPML4_A (1<<5)          //访问位，由MMU硬件设置
//页目录指针标志字段
#define KPDPTE_P (1<<0)         //页面存在位
#define KPDPTE_RW (1<<1)        //页面读写位
#define KPDPTE_US (1<<2)        //用户/超级管理者页
#define KPDPTE_PWT (1<<3)       //写回，写直达
#define KPDPTE_PCD (1<<4)       //是否开启页级Cache
#define KPDPTE_A (1<<5)         //访问位，由MMU硬件设置
//页目录项字段
#define KPDE_P (1<<0)           //页面存在位
#define KPDE_RW (1<<1)          //页面读写位
#define KPDE_US (1<<2)          //用户/超级管理者页
#define KPDE_PWT (1<<3)         //写回，写直达
#define KPDE_PCD (1<<4)         //是否开启页级Cache
#define KPDE_A (1<<5)           //访问位，由MMU硬件设置
#define KPDE_D (1<<6)           //页面脏位，页面写入数据时由MMU设置
#define KPDE_PS (1<<7)          //2MB分页下，PS必须为1
#define KPDE_G (1<<8)           //全局标志位
#define KPDE_PAT (1<<12)        //页面属性表

#define KPML4_SHIFT 39          //提取一级页目录索引右移位数
#define KPDPTTE_SHIFT 30        //提取二级页目录索引右移位数
#define KPDP_SHIFT 21           //提取三级页目录索引右移位数
#define PGENTY_SIZE 512         //页目录中页的个数2MB分页的话，4KB页目录可以记录512个页目录项


/*============================================================BIOS中断=========================================================*/
//实模式调用BIOS中断
#define RLINTNR(x) (x*2)    //用于调用realadr_call_entry时传入参数
//BIOS中断调用入口
void __attribute__((regparm(3))) realadr_call_entry(u16_t callint,u16_t val1,u16_t val2);   //__attribute__((regparm(3)))指定了函数的前三个参数使用寄存器传值


/*===========================================================E820===============================================================*/
//内存视图类型
#define RAM_USABLE 1    //可用内存
#define RAM_RESERV 2    //保留内存不可使用
#define RAM_ACPIREC 3   //ACPI data相关的
#define RAM_ACPINVS 4   //ACPI NVS空间
#define RAM_AREACON 5   //包含坏内存
//e820数组，存储内存视图
typedef struct s_e820{
    u64_t saddr;        //内存区域起始地址
    u64_t lsize;        //内存区域大小
    u32_t type;         //内存区域类型
}__attribute__((packed)) e820map_t;


/*============================================================ACPI============================================================*/
//ACPI相关结构体
typedef struct s_MRSDP{
    u64_t rp_sign;
    u8_t rp_chksum;
    u8_t rp_oemid[6];
    u8_t rp_revn;
    u32_t rp_rsdtphyadr;
    u32_t rp_len;
    u64_t rp_xsdtphyadr;
    u8_t rp_echksum;
    u8_t rp_resv[3];
}__attribute__((packed)) mrsdp_t;


/*===========================================================VBE模式===========================================================*/
//vbe相关的设置
#define VBE_DISPI_IOPORT_INDEX (0x01CE)
#define VBE_DISPI_IOPORT_DATA (0x01CF)
#define VBE_DISPI_INDEX_ID (0)
#define VBE_DISPI_INDEX_XRES (1)
#define VBE_DISPI_INDEX_YRES (2)
#define VBE_DISPI_INDEX_BPP (3)
#define VBE_DISPI_INDEX_ENABLE (4)
#define VBE_DISPI_INDEX_BANK (5)
#define VBE_DISPI_INDEX_VIRT_WIDTH (6)
#define VBE_DISPI_INDEX_VIRT_HEIGHT (7)
#define VBE_DISPI_INDEX_X_OFFSET (8)
#define VBE_DISPI_INDEX_Y_OFFSET (9)
#define BGA_DEV_ID0 (0xB0C0)    //- setting X and Y resolution and bit depth (8 BPP only), banked mode
#define BGA_DEV_ID1 (0xB0C1)    //- virtual width and height, X and Y offset0
#define BGA_DEV_ID2 (0xB0C2)    //- 15, 16, 24 and 32 BPP modes, support for linear frame buffer, support for retaining memory contents on mode switching
#define BGA_DEV_ID3 (0xB0C3)    //- support for getting capabilities, support for using 8 bit DAC
#define BGA_DEV_ID4 (0xB0C4)    //- VRAM increased to 8 MB
#define BGA_DEV_ID5 (0xB0C5)    //- VRAM increased to 16 MB? [TODO: verify and check for other changes]
#define VBE_DISPI_BPP_4 (0x04)
#define VBE_DISPI_BPP_8 (0x08)
#define VBE_DISPI_BPP_15 (0x0F)
#define VBE_DISPI_BPP_16 (0x10)
#define VBE_DISPI_BPP_24 (0x18)
#define VBE_DISPI_BPP_32 (0x20)
#define VBE_DISPI_DISABLED (0x00)
#define VBE_DISPI_ENABLED (0x01)
#define VBE_DISPI_LFB_ENABLED (0x40)

//VBE的基本信息
typedef struct s_VBEINFO{
    char vbesignature[4];       //VBE版本标志符
    u16_t vbeversion;           //VBE版本号
    u32_t oemstringptr;         //OEM字符串指针
    u32_t capabilities;         //显卡的功能和特性
    u32_t videomodeptr;         //支持视频模式列表的指针
    u16_t totalmemory;          //显存的总大小
    u16_t oemsoftwarerev;       //OEM软件版本号
    u32_t oemvendornameptr;     //OEM厂商名称的指针
    u32_t oemproductnameptr;    //OEM产品名称的指针
    u32_t oemproductrevptr;     //OEM产品版本号的指针
    u8_t reserved[222];         //保留字段
    u8_t oemdata[256];          //OEM数据
}__attribute__((packed)) vbeinfo_t;

//VBE的详细信息
typedef struct s_VBEOMINFO{
    u16_t ModeAttributes;           //显示模式的属性
    u8_t  WinAAttributes;           //Window A的属性
    u8_t  WinBAttributes;           //Window B的属性
    u16_t WinGranularity;           //Window的粒度
    u16_t WinSize;                  //Window的大小
    u16_t WinASegment;              //Window A的段地址
    u16_t WinBSegment;              //Window B的段地址
    u32_t WinFuncPtr;               //Window函数指针
    u16_t BytesPerScanLine;         //每行的字节数
    u16_t XResolution;              //水平分辨率
    u16_t YResolution;              //垂直分辨率
    u8_t  XCharSize;
    u8_t  YCharSize;
    u8_t  NumberOfPlanes;
    u8_t  BitsPerPixel;
    u8_t  NumberOfBanks;
    u8_t  MemoryModel;
    u8_t  BankSize;
    u8_t  NumberOfImagePages;
    u8_t  Reserved;
    u8_t  RedMaskSize;
    u8_t  RedFieldPosition;
    u8_t  GreenMaskSize;
    u8_t  GreenFieldPosition;
    u8_t  BlueMaskSize;
    u8_t  BlueFieldPosition;
    u8_t  RsvdMaskSize;
    u8_t  RsvdFieldPosition;
    u8_t  DirectColorModeInfo;
    u32_t PhysBasePtr;
    u32_t Reserved1;
    u16_t Reserved2;
    u16_t LinBytesPerScanLine;
    u8_t  BnkNumberOfImagePages;
    u8_t  LinNumberOfImagePages;
    u8_t  LinRedMaskSize;
    u8_t  LinRedFieldPosition;
    u8_t  LinGreenMaskSize;
    u8_t  LinGreenFieldPosition;
    u8_t  LinBlueMaskSize;
    u8_t  LinBlueFieldPosition;
    u8_t  LinRsvdMaskSize;
    u8_t  LinRsvdFieldPosition;
    u32_t MaxPixelClock;
    u8_t  Reserved3[189];
}__attribute__((packed)) vbeominfo_t;


/*=====================================================================图形化控制相关的=================================================================*/
//由于二级引导器只是初始化了vbe模式并没有使用它来向屏幕输出，因此像素结构未被使用
typedef u32_t pixl_t;   //使用一个32位的数据来表示4字节的像素
#define ARGB(a,r,g,b) ((a<<24)|(r<<16)|(g<<8)|b)  //数据顺序ARGB，通常情况下使用pixl_t和BGRA宏

#define VBEMODE 1
#define GPUMODE 2
#define BGAMODE 3
//图形信息结构体
typedef struct s_GRAPH{
    u32_t gh_mode;          //图形模式
    u32_t gh_x;             //水平像素点
    u32_t gh_y;             //垂直像素点
    u32_t gh_framphyadr;    //显存物理地址
    u32_t gh_onepixbits;    //一个像素字占的位数
    u32_t gh_vbemodenr;     //vbe模式号
    u32_t gh_vifphyadr;     //
    u32_t gh_vmifphyadr;    //
    u32_t gh_bank;          //显存bank数
    u32_t gh_curdipbnk;     //当前bank
    u32_t gh_nextbnk;       //下一个bank
    u32_t gh_banksz;        //bank大小
    u32_t gh_logophyadrs;   //
    u32_t gh_logophyadre;   //
    u32_t gh_fontadr;       //字库大小
    u32_t gh_ftsectadr;     //
    u32_t gh_ftsectnr;      //
    u32_t gh_fnthight;      //字体高度
    u32_t gh_nxtcharsx;     //下一字符显示的x坐标
    u32_t gh_nxtcharsy;     //下一字符显示的y坐标
    u32_t gh_linesz;        //字符行高
    vbeinfo_t gh_vbeinfo;   //VBE基本信息
    vbeominfo_t gh_vminfo;  //VBE详细信息
}__attribute__((packed)) graph_t;


/*=======================================================================机器信息相关================================================================*/
//机器信息结构体machbstart_t的地址，1MB地址处
#define MBSPADR ((machbstart_t*)(0x100000))
//机器信息结构体的魔数：LOMSMBSP:lomsmbsp,正好是8个ASCII码，存入64位的数据中
#define MBS_MIGC (u64_t)((((u64_t)'L')<<56)|(((u64_t)'M')<<48)|(((u64_t)'O')<<40)|(((u64_t)'S')<<32)|(((u64_t)'M')<<24)|(((u64_t)'B')<<16)|(((u64_t)'S')<<8)|((u64_t)'P'))
//机器信息结构体
typedef struct s_MACHBSTART{
    u64_t   mb_migc;                //魔数
    u64_t   mb_chksum;              //校验和
    u64_t   mb_krlinitstack;        //内核栈顶地址
    u64_t   mb_krlitstacksz;        //内核栈大小
    u64_t   mb_imgpadr;             //操作系统映像(eki文件)地址
    u64_t   mb_imgsz;               //操作系统映像大小
    u64_t   mb_krlimgpadr;          //内核(Cosmos.bin)地址
    u64_t   mb_krlsz;               //内核(Cosmos.bin)大小
    u64_t   mb_krlvec;              //
    u64_t   mb_krlrunmode;          //
    u64_t   mb_kalldendpadr;        //
    u64_t   mb_ksepadrs;            //
    u64_t   mb_ksepadre;            //
    u64_t   mb_kservadrs;           //
    u64_t   mb_kservadre;           //
    u64_t   mb_nextwtpadr;          //下一段空闲内存首地址（4K对齐）
    u64_t   mb_bfontpadr;           //操作系统字体地址
    u64_t   mb_bfontsz;             //操作系统字体大小
    u64_t   mb_fvrmphyadr;          //机器显存地址
    u64_t   mb_fvrmsz;              //机器显存大小
    u64_t   mb_cpumode;             //机器CPU工作模式
    u64_t   mb_memsz;               //机器内存大小
    u64_t   mb_e820padr;            //机器e80数组地址
    u64_t   mb_e820nr;              //机器e80数组元素个数
    u64_t   mb_e820sz;              //机器e80数组大小
    u64_t   mb_e820expadr;          //
    u64_t   mb_e820exnr;            //
    u64_t   mb_e820exsz;            //
    u64_t   mb_memznpadr;           //
    u64_t   mb_memznnr;             //
    u64_t   mb_memznsz;             //
    u64_t   mb_memznchksum;         //
    u64_t   mb_memmappadr;          //
    u64_t   mb_memmapnr;            //
    u64_t   mb_memmapsz;            //
    u64_t   mb_memmapchksum;        //
    u64_t   mb_pml4padr;            //机器顶级页目录地址
    u64_t   mb_subpageslen;         //一、二、三级页目录总占用的内存大小
    u64_t   mb_kpmapphymemsz;       //操作系统映射空间大小
    u64_t   mb_ebdaphyadr;          //ACPI的edba存储区内存地址，大小通常为1KB
    mrsdp_t mb_mrsdp;               //
    graph_t mb_ghparm;              //图形信息
}__attribute__((packed)) machbstart_t;


#endif // LDRTYPE_H
