/**
 * 图形化头文件
 */

#ifndef _GRAPH_H
#define _GRAPH_H

/**
 * 初始化图形模式
 */
void init_graph(machbstart_t* mbsp);

/**
 * 初始化图形对象
 */
void graph_t_init(graph_t* initp);

/**
 * 将虚拟地址转换为线性地址，并返回转换后的线性地址
 */
u32_t vfartolineadr(u32_t vfar);

/**
 * 初始化内核虚拟模式，并分配内核虚拟化堆栈
 */
void init_kinitfvram(machbstart_t* mbsp);

/**
 * 获取VBE模式信息，并将其存储在machbstart_t中
 */
void get_vbemode(machbstart_t* mbsp);

/**
 * 用于获取VBE模式的详细信息，并将其存储在machbstart_t中
 */
void get_vbemodeinfo(machbstart_t* mbsp);

/**
 * 该函数用于设置VBE模式信息，但没有接受任何参数。
 */
void set_vbemodeinfo();

/**
 * 该函数用于向BGA寄存器中写入数据，它接受两个的参数，index表示寄存器索引，data表示要写入的数据 
 */
void bga_write_reg(u16_t index, u16_t data);

/**
 * 该函数用于从BGA寄存器读取数据，并返回读取的值，index表示要读取的寄存器的索引
 */
u16_t bga_read_reg(u16_t index);

/**
 * 获取BGA设备信息，并返回设备信息的值
 */
u32_t get_bgadevice();

/**
 * 检查BGA设备的最大版本号，并返回设备信息
 * maxver:maximum version
 */
u32_t chk_bgamaxver();

/**
 * 初始化BGA设备，并接受一个指向machbstart_t结构的指针参数
 */
void init_bgadevice(machbstart_t* mbsp);

#endif
