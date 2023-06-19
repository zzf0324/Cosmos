/**
 * 屏幕上格式化输出文件halprint.c
 */

#include "cosmostypes.h"
#include "cosmosmctrl.h"

/**
 * 向屏幕提示初始化信息
 */
void hint(const char_t* fmt,...){
	kprint(fmt);
	die(0x200);		//延时，便于用户查看
	return;
}

/**
 * 类似于C库printf()函数
 */
void kprint(const char_t *fmt, ...){
	char_t buf[512];
	va_list ap;		//可变参数列表
	va_start(ap, fmt);	//可变参数列表开始
	vsprintfk(buf, fmt, ap);	//使用可变参数
	gstr_write(&kdftgh, buf);	
	va_end(ap);			//可变参数列表结束
	return;
}

/**
 * 字符串拷贝函数
 * des:buf
 * src:str_s
 */
char_t *strcopyk(char_t *buf, char_t *str_s){
	while (*str_s){
		*buf = *str_s;
		buf++;
		str_s++;
	}
	return buf;
}

/**
 * 功能类似于C标准库sprintf()函数
 */
void vsprintfk(char_t *buf, const char_t *fmt, va_list args){
	char_t *p = buf;
	while (*fmt){
		if (*fmt != '%'){
			*p++ = *fmt++;
			continue;
		}
		fmt++;
		switch (*fmt){
		case 'x':
			p = numberk(p, va_arg(args, uint_t), 16);
			fmt++;
			break;
		case 'd':
			p = numberk(p, va_arg(args, uint_t), 10);
			fmt++;
			break;
		case 's':
			p = strcopyk(p, (char_t *)va_arg(args, uint_t));
			fmt++;
			break;
		default:
			break;
		}
	}
	*p = 0;
	return;
}

/**
 * 将一个整数n转换为base进制的字符串
 */
char_t *numberk(char_t *str, uint_t n, sint_t base){
	register char_t *p;
	char_t strbuf[36];
	p = &strbuf[36];
	*--p = 0;
	if (n == 0){
		*--p = '0';
	}
	else{
		do{
			*--p = "0123456789abcdef"[n % base];
		} while (n /= base);
	}
	while (*p != 0){
		*str++ = *p++;
	}
	return str;
}
