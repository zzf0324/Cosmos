/**
 * 实模式/保护模式参数
 * RM:real mode，实模式
 * PM:protected mode，保护模式
 */

#ifndef REALPARM_H
#define REALPARM_H


#define ETYBAK_ADR 0x2000               // 基地址，参照
#define PM32_EIP_OFF (ETYBAK_ADR)       // 保护模式下指令指针在ETYBAK_ADR上的偏移
#define PM32_ESP_OFF (ETYBAK_ADR+4)     // 保护模式下栈指针在ETYBAK_ADR上的偏移
#define RM16_EIP_OFF (ETYBAK_ADR+8)     // 实模式下指令指针在ETYBAK_ADR上的偏移
#define RM16_ESP_OFF (ETYBAK_ADR+12)    // 实模式下栈指针在ETYBAK_ADR上的偏移
//skip some bytes
#define RWHDPACK_ADR (ETYBAK_ADR+32)    // 
#define E80MAP_NR (ETYBAK_ADR+64)       // 保存e820数组元素number的地址
#define E80MAP_ADRADR (ETYBAK_ADR+68)   // 保存e820数组地址的指针地址，然后当将E80MAP_ADRADR赋值给指针时，这个指针就是结构体数组的指针，对应数组的寻址
#define E80MAP_ADR (0x5000)             // e820数组开始地址
#define VBEINFO_ADR (0x6000)            // vbeinfo开始地址
#define VBEMINFO_ADR (0x6400)           // vbeminfo开始地址

#define READHD_BUFADR 0x3000            //


#endif // REALPARM_H
