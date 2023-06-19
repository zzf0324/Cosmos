/**
 * 数据类型文件
 */

#ifndef _TYPE_H
#define _TYPE_H

//定义无符号整数
typedef unsigned char u8_t;
typedef unsigned short u16_t;
typedef unsigned int u32_t;
typedef unsigned long long u64_t; //Linux中，32位模式下long long表示64子节，long表示32子节

//定义有符号整数,signed是默认的
typedef char s8_t;
typedef short s16_t;
typedef int s32_t;
typedef long long s64_t;

typedef long sint_t;            //有符号int类型，32位
typedef unsigned long uint_t;   //无符号int类型，32位


typedef u64_t cpuflg_t;         //cpu标志寄存器类型，32位保护模式，eflags为32位

typedef unsigned long adr_t;    //地址类型
typedef unsigned long size_t;   //大小类型

typedef u64_t bool_t;   //64位无符号整数，表示boolean类型
typedef u64_t dev_t;    //64位无符号整数，表示device类型
typedef const char* str_t;  //字符串类型（字符串常量指针）
typedef char char_t;        //字符类型
typedef unsigned long drv_t;    //driver，表示驱动器类型
typedef unsigned long mrv_t;    //内存区域类型
typedef sint_t drvstus_t;       //驱动器状态
#define KLINE static inline     //静态内置函数

typedef void (*inthandler_t)();
typedef drv_t (*i_handle_t)(uint_t int_nr);
typedef drv_t (*f_handle_t)(uint_t int_nr,void* sframe);
typedef void (*syscall_t)();
typedef drv_t (*dev_op_t)();
typedef mrv_t (*msg_op_t)();
typedef void* exception_t;
typedef char_t* va_list_t;      //可变参数列表



#define PUBLIC              //
#define private	static      //私有域
#define EXTERN extern       //两个宏定义的区别在于它们的命名习惯和用途上的差异，本质是一样的
#define KEXTERN extern      //
#define NULL	0           //空指针NULL=0
#define TRUE    1           //boolean:true
#define	FALSE	0           //boolean:false
#define REGCALL __attribute__((regparm(3)))
#define ALIGN(x, a)     (((x) + (a) - 1) & ~((a) - 1))  //取整对齐


#define P4K_ALIGN(x) ALIGN(x,0x1000)    //页地址4K对齐，4K=0x1000


#endif