;实现调用BIOS中断功能,16位实模式下的asm代码

%include "ldrasm.inc"	;包含一个名为“ldrasmd.inc”的外部文件
global _start		;声明_start为全局标号，以便其他模块引用
[section .text]
[bits 16]		;设置代码位数为16位
_start:
_16_mode:
	mov	bp,0x20 		;0x20是指向GDT中的16位数据段描述符k16da_dsc
	mov	ds, bp
	mov	es, bp
	mov	ss, bp
	mov	ebp, cr0
	and	ebp, 0xfffffffe
	mov	cr0, ebp 		;CR0.P=0关闭保护模式，使用0xfffffffe将cr0寄存器最低位置0，其余位置1
	jmp	0:real_entry 	;刷新CS影子寄存器，真正进入实模式

real_entry:		;实模式入口标签
	mov bp, cs
	mov ds, bp
	mov es, bp
	mov ss, bp
	mov sp, 08000h	;实模式栈指针
	mov bp,func_table
	call [bp] 
	add bp,ax			;调用函数表中的汇编函数，ax是C函数传递过来的，用于指定使用哪个函数
	
	cli
	call disable_nmi
	;关不可屏蔽中断
	mov	ebp, cr0
	or	ebp, 1
	mov	cr0, ebp 		;CR0.P=1开启保护模式
	jmp dword 0x8 :_32bits_mode	;返回32位模式
[BITS 32]
_32bits_mode:
	mov bp, 0x10 		;kcode_dsc
	mov ds, bp
	mov ss, bp
	mov esi,[PM32_EIP_OFF]	;恢复中断前的EIP
	mov esp,[PM32_ESP_OFF]	;恢复中断前的ESP
	jmp esi			;返回到调用处（save_eip_jmp）继续执行pop gs，以恢复更多的寄存器，继而恢复到调用前的状态

[BITS 16]
DispStr:			;用于显示字符串的函数
	mov bp,ax
	mov cx,23
	mov ax,01301h
	mov bx,000ch
	mov dh,10
	mov dl,25
	mov bl,15
	int 10h
	ret
cleardisp:			;清屏函数
	mov ax,0600h     	;这段代码是为了清屏
	mov bx,0700h
	mov cx,0
	mov dx,0184fh
	int 10h			;调用的BOIS的10号
	ret

_getmmap:			;获取内存布局视图e820数组的函数
	push ds
	push es
	push ss
	mov esi,0
	mov dword[E80MAP_NR],esi
	mov dword[E80MAP_ADRADR],E80MAP_ADR

	xor ebx,ebx
	mov edi,E80MAP_ADR
loop:
	mov eax,0e820h	
	mov ecx,20 	
	mov edx,0534d4150h 
	int 15h 	
	jc .1

	add edi,20
	cmp edi,E80MAP_ADR+0x1000
	jg .1

	inc esi

	cmp ebx,0
	jne loop		;循环获取e820map结构

	jmp .2

.1:
	mov esi,0

.2:
	mov dword[E80MAP_NR],esi
	pop ss
	pop es
	pop ds
	ret
_read:			;读取硬盘的函数
	push ds
	push es
	push ss
	xor eax,eax
	mov ah,0x42
	mov dl,0x80
	mov si,RWHDPACK_ADR	;将货物获取的磁盘信息放入RWHDPACK_ADR下
	int 0x13
	jc  .err
	pop ss
	;else
	pop es
	pop ds
	ret
.err:
	mov ax,int131errmsg	;将字符串地址存入ax寄存器，然后调用DispStr子程序显示字符串
	call DispStr
	jmp $
	pop ss
	pop es
	pop ds
	ret

_getvbemode:		;获取显卡vbe模式的函数
        push es
        push ax
        push di
        mov di,VBEINFO_ADR
 	;将获取到的vbe信息存入VBEINFO_ADR下
        mov ax,0
        mov es,ax
        mov ax,0x4f00
        int 0x10
        cmp ax,0x004f
        jz .ok
        mov ax,getvbmodeerrmsg
	;else
        call DispStr
        jmp $
 .ok:
        pop di
        pop ax
        pop es
        ret
_getvbeonemodeinfo:	;获取显卡vbe模式数据的函数
        push es
        push ax
        push di
        push cx
        mov di,VBEMINFO_ADR
	;将获取到的信息存入VBEMINFO_ADR下
        mov ax,0
        mov es,ax
        mov cx,0x118
        mov ax,0x4f01
        int 0x10
        cmp ax,0x004f
        jz .ok
        mov ax,getvbmodinfoeerrmsg
	;else
        call DispStr
        jmp $
 .ok:
        pop cx
        pop di
        pop ax
        pop es
        ret
_setvbemode:		;设置显卡vbe模式的函数
        push ax
        push bx
        mov bx,0x4118
        mov ax,0x4f02
        int 0x10
        cmp ax,0x004f
        jz .ok
        mov ax,setvbmodeerrmsg
	;else
        call DispStr
        jmp $
 .ok:
        pop bx
        pop ax
        ret
disable_nmi:		;禁用nmi(不可屏蔽中断)的函数，不可屏蔽中断通常用于错误处理
	push ax
	in al, 0x70     ; port 0x70NMI_EN_PORT
	or al, 0x80	; disable all NMI source
	out 0x70,al	; NMI_EN_PORT
	pop ax
	ret

func_table:		;函数表，包含要调用的汇编函数的地址(标号)
	dw _getmmap		;获取内存布局视图的函数
	dw _read		;读取硬盘的函数
     	dw _getvbemode 		;获取显卡vbe模式
     	dw _getvbeonemodeinfo	;获取显卡vbe模式的数据
     	dw _setvbemode		;设置显卡的vbe模式

;下面是错误消息字符串
int131errmsg: db     "int 13 read hdsk error"
        db 0
getvbmodeerrmsg: db     "get vbemode err"
        db 0
getvbmodinfoeerrmsg: db     "get vbemodeinfo err"
      	db 0
setvbmodeerrmsg: db     "set vbemode err"
        db 0
