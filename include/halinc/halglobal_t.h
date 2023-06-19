/*
 * HAL全局数据结构头文件
 */

#ifndef _HALGLOBAL_T_H
#define _HALGLOBAL_T_H

//全局变量定义变量放在data段
#define HAL_DEFGLOB_VARIABLE(vartype,varname) \
EXTERN  __attribute__((section(".data"))) vartype varname        //用于定义全局变量的宏，只需宏名和宏类型就可自动展开为一个完整的宏定义，并且声明了放在了data段


#endif // HALGLOBAL_T_H
