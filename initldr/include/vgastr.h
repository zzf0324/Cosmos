/**
 * 显卡文本模式下的字符串输出
 */

#ifndef _VGASTR_H
#define _VGASTR_H

/**
 * 初始化光标
 */
void init_curs();

/**
 * 设置光标的位置
 */
void set_curs(u32_t x, u32_t y);

/**
 * 清屏，并设置背景色
 */
void clear_screen(u16_t value);

/**
 * 关闭光标显示
 */
void close_curs();

/**
 * 格式化字符串
 */
void vsprintfk(char_t *buf, const char_t *fmt, va_list_t args);

/**
 * 打印格式化字符串
 */
void kprint(const char_t *fmt, ...);

/**
 * 输出提示信息
 */
void hint(const char_t *fmt,...);


#endif
