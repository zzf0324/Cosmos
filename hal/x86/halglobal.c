/**
 * hal全局数据结构文件
 */

#define HALGOBAL_HEAD

#include "cosmostypes.h"
#include "cosmosmctrl.h"

#ifdef CFG_X86_PLATFORM

HAL_DEFGLOB_VARIABLE(descriptor_t,x64_gdt)[CPUCORE_MAX][GDTMAX];    //
HAL_DEFGLOB_VARIABLE(gate_t,x64_idt)[IDTMAX];                       //定义全局中断门描述符表
HAL_DEFGLOB_VARIABLE(x64tss_t,x64tss)[CPUCORE_MAX];                 //
HAL_DEFGLOB_VARIABLE(igdtr_t,x64_igdt_reg)[CPUCORE_MAX];            //
HAL_DEFGLOB_VARIABLE(iidtr_t,x64_iidt_reg);                         //
HAL_DEFGLOB_VARIABLE(machbstart_t, kmachbsp);                       //定义全局机器信息结构体
HAL_DEFGLOB_VARIABLE(dftgraph_t, kdftgh);                           //定义全局图形信息结构体

HAL_DEFGLOB_VARIABLE(intfltdsc_t, machintflt)[IDTMAX];              //

#endif

