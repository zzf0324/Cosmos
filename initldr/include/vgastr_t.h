/**
 * 转换显示字符串头文件
 * 头文件提供了一些常量和结构体类型，用于在操作 VGA 显示时使用
 */

#ifndef _VGASTR_T_H
#define _VGASTR_T_H


//VGA:Video Graphics Array
#define VGASTR_RAM_BASE (0xb8000)   //显存起始地址
#define VGASTR_RAM_END  (0xbffff)   //显存结束地址
#define VGADP_DFVL 0x0e00           //默认字符：高8位ASCII，低8位显示属性

//显卡控制相关
#define VGACTRL_REG_ADR 0x3d4       //显卡索引寄存器端口号，可以向它写入一个值，用来指定内部的某个寄存器
#define VGACTRL_REG_DAT 0x3d5       //显卡寄存器的数据端口，可以通过数据端口对指定的寄存器数据读写
#define VGACURS_REG_INX 0x0a        //VGA光标寄存器的索引端口，用于对光标寄存器进行读写操作
#define VGACURS_CLOSE 0x20          //定义了关闭VGA光标的控制码

//表示VGA光标的信息
typedef struct s_CURSOR{
    uint_t vmem_s;      //缓冲区的起始地址vga memory start 
    uint_t vmem_e;      //缓冲区的结束地址vga memory end
    uint_t cvmemadr;    //当前显示缓冲区的地址current vmem address
    uint_t x;   //光标在屏幕上的位置：x
    uint_t y;   //光标在屏幕上的位置：y
}cursor_t; 


#endif

