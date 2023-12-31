;GRUB头的汇编部分: 1.让GRUB识别 2.设置C语言运行环境
;GRUB需要的两个头

MBT_HDR_FLAGS	EQU 0x00010003
MBT_HDR_MAGIC	EQU 0x1BADB002
MBT2_MAGIC	EQU 0xe85250d6
global _start
extern inithead_entry 		;全局标号_start
[section .text]				;声明调用的C语言函数
[bits 32]
_start:
	jmp _entry
align 4
mbt_hdr:
	dd MBT_HDR_MAGIC
	dd MBT_HDR_FLAGS
	dd -(MBT_HDR_MAGIC+MBT_HDR_FLAGS)
	dd mbt_hdr
	dd _start
	dd 0
	dd 0
	dd _entry
	;
	; multiboot header
	;
ALIGN 8
mbhdr:
	DD	0xE85250D6
	DD	0
	DD	mhdrend - mbhdr
	DD	-(0xE85250D6 + 0 + (mhdrend - mbhdr))
	DW	2, 0
	DD	24
	DD	mbhdr
	DD	_start
	DD	0
	DD	0
	DW	3, 0
	DD	12
	DD	_entry 
	DD  0  
	DW	0, 0
	DD	8
mhdrend:

_entry:
	cli

	in al, 0x70
	or al, 0x80	
	out 0x70,al

	lgdt [GDT_PTR]							;加载GDT地址和界限到GDTR
	jmp dword 0x8 :_32bits_mode				;从实模式切换到保护模式，长跳转刷新影子寄存器

_32bits_mode:			;执行C语言环境初始化，初始化所需要的寄存器
	mov ax, 0x10
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	xor eax,eax
	xor ebx,ebx
	xor ecx,ecx
	xor edx,edx
	xor edi,edi
	xor esi,esi
	xor ebp,ebp
	xor esp,esp
	mov esp,0x7c00
	;完成C语言环境初始化，调用C函数inithead_entry,加载模块initldrkrl.bin和initldrsve.bin
	call inithead_entry	
	jmp 0x200000 			;调用返回后，跳转二级引导器模块initldrkrl.bin


;GDT全局段描述符表
GDT_START:
knull_dsc: dq 0
kcode_dsc: dq 0x00cf9e000000ffff		;32位代码段描述符
kdata_dsc: dq 0x00cf92000000ffff		;32位数据段描述符
k16cd_dsc: dq 0x00009e000000ffff		;16为代码段描述符
k16da_dsc: dq 0x000092000000ffff		;16为数据段描述符
GDT_END:
;GDT表寄存器
GDT_PTR:
GDTLEN	dw GDT_END-GDT_START-1	;GDT界限
GDTBASE	dd GDT_START
