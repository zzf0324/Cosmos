/**
 * 加载内核入口头文件
 */

#ifndef LDRKRLENTRY_H
#define LDRKRLENTRY_H


/**
 * 内核入口主函数
 */
void ldrkrl_entry();

/**
 * 输出内核出错信息
 * kestr:kernal error string
 */
void kerror(char_t* kestr);

/**
 * 延时
 */
void die(u32_t dt);


#endif // LDRKRLENTRY_H
