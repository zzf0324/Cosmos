/**
 * 屏幕上格式化输出头文件
 * 定义可变参数相关的宏
 **/

#ifndef _HALPRINTFK_T_H
#define _HALPRINTFK_T_H


//将编译器内置的可变参数类型重新命名为va_list
typedef __builtin_va_list va_list;

//可变参数开始
#define va_start(ap,np) __builtin_va_start(ap,np)

#define va_arg(ap,ty) __builtin_va_arg(ap,ty)

//可变参数结束
#define va_end(ap) __builtin_va_end(ap)


#endif
