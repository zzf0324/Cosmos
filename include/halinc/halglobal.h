/**
 * HAL全局数据结构头文件
 */

#ifndef _HALGLOBAL_H
#define _HALGLOBAL_H


#ifdef	HALGOBAL_HEAD
#undef	EXTERN
#define EXTERN
#endif

#ifdef CFG_X86_PLATFORM
void	exc_divide_error();
void	exc_single_step_exception();
void	exc_nmi();
void	exc_breakpoint_exception();
void	exc_overflow();
void	exc_bounds_check();
void	exc_inval_opcode();
void	exc_copr_not_available();
void	exc_double_fault();
void	exc_copr_seg_overrun();
void	exc_inval_tss();
void	exc_segment_not_present();
void	exc_stack_exception();
void	exc_general_protection();
void	exc_page_fault();
void	exc_copr_error();
void	exc_alignment_check();
void	exc_machine_check();
void	exc_simd_fault();
void	hxi_exc_general_intpfault();
void	hxi_hwint00();
void	hxi_hwint01();
void	hxi_hwint02();
void	hxi_hwint03();
void	hxi_hwint04();
void	hxi_hwint05();
void	hxi_hwint06();
void	hxi_hwint07();
void	hxi_hwint08();
void	hxi_hwint09();
void	hxi_hwint10();
void	hxi_hwint11();
void	hxi_hwint12();
void	hxi_hwint13();
void	hxi_hwint14();
void	hxi_hwint15();
void	hxi_hwint16();
void	hxi_hwint17();
void	hxi_hwint18();
void	hxi_hwint19();
void	hxi_hwint20();
void	hxi_hwint21();
void	hxi_hwint22();
void	hxi_hwint23();
void	hxi_apic_svr();
void	hxi_apic_ipi_schedul();
void	hxi_apic_timer();
void	hxi_apic_thermal();
void	hxi_apic_performonitor();
void	hxi_apic_lint0();
void	hxi_apic_lint1();
void	hxi_apic_error();
void	exi_sys_call();
void	asm_ret_from_user_mode();


HAL_DEFGLOB_VARIABLE(descriptor_t,x64_gdt)[CPUCORE_MAX][GDTMAX];
HAL_DEFGLOB_VARIABLE(gate_t,x64_idt)[IDTMAX];               //定义中断表，IDTMAX=256，X86平台最多支持256个中断
HAL_DEFGLOB_VARIABLE(x64tss_t,x64tss)[CPUCORE_MAX]; 
HAL_DEFGLOB_VARIABLE(igdtr_t,x64_igdt_reg)[CPUCORE_MAX];
HAL_DEFGLOB_VARIABLE(iidtr_t,x64_iidt_reg);
HAL_DEFGLOB_VARIABLE(machbstart_t,kmachbsp);                //声明全局机器信息结构体
HAL_DEFGLOB_VARIABLE(dftgraph_t,kdftgh);                    //声明全局图形信息结构体
HAL_DEFGLOB_VARIABLE(memmgrob_t,memmgrob);
HAL_DEFGLOB_VARIABLE(intfltdsc_t,machintflt)[IDTMAX];       //中断异常描述符表；256个
#endif

/**
 * 
 */
void die(u32_t dt);


#endif // HALGLOBAL_H
