/**
 * 声明基本数据类型,后面加_t表示是类型
 */

#ifndef _BASTYPE_T_H
#define _BASTYPE_T_H


//定义无符号类型数据:unsigied
typedef unsigned char u8_t;
typedef unsigned short u16_t;
typedef unsigned int u32_t;
typedef unsigned long u64_t;    //由于经过二级引导器加载后，已经跳转到64位长模式，所以这里的long是64位的
//定义有符号数据类型:signed
typedef char s8_t;
typedef short s16_t;
typedef int s32_t;
typedef long s64_t;

//以下全为long类型
typedef long sint_t;            //signed long类型
typedef unsigned long uint_t;   //unsigned long类型
typedef u64_t cpuflg_t;         //用于定义CPU Flags类型的变量
typedef unsigned long adr_t;    //用于定义64地址类型

typedef sint_t bool_t;          //boolean类型
typedef u32_t dev_t;            //device类型
typedef const char* str_t;      //字符串指针常量类型
typedef char char_t;            //字符类型
typedef unsigned long drv_t;    //driver类型
typedef unsigned long mrv_t;    //
typedef sint_t drvstus_t;       //驱动器状态
typedef sint_t sysstus_t;       //系统状态
typedef sint_t hand_t;          //句柄
typedef void* buf_t;            //缓冲区类型
typedef unsigned long size_t;   //大小类型
typedef u32_t reg_t;            //寄存器类型


typedef void (*inthandler_t)();
typedef drv_t (*i_handle_t)(uint_t int_nr);
typedef drv_t (*f_handle_t)(uint_t int_nr,void* sframe);
typedef drvstus_t (*intflthandle_t)(uint_t ift_nr,void* device,void* sframe);
typedef u64_t mmstus_t;

#define KLINE static inline     //将函数声明为静态内联，编译时直接将函数插入到调用点，提高执行效率
#define PUBLIC                  //PUBLIC是一个空定义，通常用于标记公共的函数或变量
#define private	static          //将static关键字当做private使用，表示他们仅在当前文件中可用
#define EXTERN extern           //将extern当做EXTERN使用
#define KEXTERN extern          //将extern当做KEXTERN
#define NULL	0               //定义类似C库的NULL，表示0值
#define TRUE    1               //boolean:true
#define	FALSE	0               //boolean:false
#define DFCERRSTUS (-1)         //status:ERROR
#define DFCOKSTUS (0)           //status:OK
#define NO_HAND (-1)            //无效的句柄值
#define ALIGN(x, a)     (((x) + (a) - 1) & ~((a) - 1))  //用于内存对齐：将x向上调整为a的整数倍

#define LKHEAD_T __attribute__((section(".head.text")))
#define LKHEAD_D __attribute__((section(".head.data")))
#define LKINIT                  //空定义，用于标记初始化代码


#endif
