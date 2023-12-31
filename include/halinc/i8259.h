/**
 * i8259中断控制器芯片头文件
 */

#ifndef _I8259_H
#define _I8259_H


/**
 * 初始化i8259中断控制器
 */
void init_i8259();

/**
 * 
 */
void i8259_send_eoi();

/**
 * 
 */
void i8259_enabled_line(u32_t line);

/**
 * 
 */
void i8259_disable_line(u32_t line);

/**
 * 
 */
void i8259_save_disableline(u64_t* svline,u32_t line);

/**
 * 
 */
void i8259_rest_enabledline(u64_t* svline,u32_t line);


#endif
