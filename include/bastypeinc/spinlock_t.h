/**
 * 自旋锁头文件
 */

#ifndef _SPINLOCK_T_H
#define _SPINLOCK_T_H


typedef struct{
	 volatile u32_t lock;  //volatile关键字可以防止编译器优化，保证其他代码始终都是从内存加载lock的值
} spinlock_t;


#endif
