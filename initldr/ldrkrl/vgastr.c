/**
 * 显卡文本模式函数
 */

#include "cmctl.h"
__attribute__((section(".data"))) cursor_t curs;    //新建一个光标结构体变量，并指定这个变量存储位置为数据段DS
//使用curs来记录当前屏幕的信息，如光标位置，缓冲区地址等

/**
 * 滚动屏幕
 */
static void scroll_screen(){
    u8_t* p = (u8_t*)VGASTR_RAM_BASE;   //获取显存地址
    for(uint_t i=160;i<4000;i+=2){ //只移动ASCII，不移动显示属性
        p[i-160] = p[i];
    }
    for(uint_t i=24*80*2;i<25*80*2;i+=2){
           p[i] = (u8_t)0x00;   //清零
    }
    return;
}

/**
 * 向屏幕写入字符串，如果没有清空屏幕的话，将会接着上一次的进行输出
 * 传统的文本模式显下，显示器大小是25行*80列，
 * 每个字符占2B，低地址的1B的ASCII，高地址的1B的字符显示属性
 * 显存大小是25*80*2=4000B
 * str:要显示的字符串
 * cursptr:当前光标结构体指针
 */
static void write_str(char_t *str,cursor_t* cursor){
    uint_t index = cursor->x+cursor->y*80;  		//字符位置
    char_t *addr = (char_t *)(cursor->vmem_s+index*2);    //实际显存中物理偏移
    while(*str) {
        switch(*str){
            case '\n':  //换行
                cursor->y++;
                cursor->x = 0;
                if (cursor->y > 24) {
                    cursor->y--;
                    scroll_screen();    //滚动屏幕
                }
                break;
            case '\r':  //回车
                cursor->x=0;    //光标回到行首
                break;
            case '\t':  //制表符
                cursor->x = ALIGN(cursor->x+1,8)-1; //制表符按照4倍数对齐
                if(cursor->x> 79){
                    cursor->y++; //另起一行
                    cursor->x=0;    //x坐标取0
                }
                break;
            case '\v':  //垂直制表符
                ;
                break;                
            case '\b':  //退格
                if(cursor->x>0){
                    cursor->x--;
                }
                break;
            default:    //普通字符
                cursor->x++;
                if (cursor->x > 79) {   //x>80*2-1,重起一行
                    cursor->x = 0;
                    cursor->y++;
                    if (cursor->y > 24) { //超过最后一行，清屏，从头显示
                        cursor->y--;
                        scroll_screen();
                    }
                }
                *addr = *str;   //将字符填入ASCII位置
                break;
        }
        str++;		// 1c
        addr+=2;	// 2B
    }
    return;
}

/**
 * 设置光标位置
 */
void set_curs(u32_t x, u32_t y){
    curs.x = x;
    curs.y = y;
    return;
}

/**
 * 初始化屏幕，将光标初始化(0,0),指向vga[0]位置，然后初始化vga[x]数组
 * 16位的srrv对应1B的ASCII码和1B的显示属性
 * 初始化的目的是为了统一所有字符的显示属性，并将ASCII属性置为0，因为在后续输出字符串的时候是不修改字符的显示属性的，所以这里要统一设置
 * 小端模式：例如srrv=0x0700时，0x00放在p[i]的低字节(ASCII属性)，0x07放在的p[i]的高子节(显示属性)
 */
void clear_screen(u16_t value){
    curs.x = 0;
    curs.y = 0;

    //使用16位地址是因为文本模式一个ASCII字符需要两字节表示，使用u16_t*指针p，这样p++的时候正好地址偏移2B
    u16_t *p = (u16_t *)VGASTR_RAM_BASE;    //用16位p指针指向显存起始地址

    //文本模式支持25*80=2000个字符
    for (uint_t i = 0; i < 2000; i++){       //使用for重置显存值
        p[i] = value;       //初始化数组元素，注意GCC一般都是使用小端模式，
    }

    close_curs();   //关闭光标
    return;
}


/**
 * 初始化光标结构体变量
 */
void init_curs(){
    curs.vmem_s = VGASTR_RAM_BASE;  //缓冲区起始地址
    curs.vmem_e = VGASTR_RAM_END;   //缓冲区结束地址
    curs.x = 0;
    curs.y = 0;
    return;
}

/**
 * 关闭光标
 */
void close_curs(){
    //通过向寄存器中写入值来控制文本模式
    out_u8(VGACTRL_REG_ADR, VGACURS_REG_INX);
    out_u8(VGACTRL_REG_DAT, VGACURS_CLOSE);
    return;
}

/**
 * 类似于C库printf()函数
 */
void kprint(const char_t *fmt, ...){
    char_t buf[512];
    va_list ap;     //可变参数列表
    va_start(ap, fmt);  //可变参数列表开始
    vsprintfk(buf, fmt, ap);    //使用可变参数   
    va_end(ap);         //可变参数列表结束
    write_str(buf, &curs); //将字符显示在屏幕上
    return;
}

/**
 * 字符串拷贝函数
 * des:buf
 * src:str_s
 */
static char_t *strcopyk(char_t *buf, char_t *str_s){
    while (*str_s){
        *buf = *str_s;
        buf++;
        str_s++;
    }
    return buf;
}

/**
 * 对n转换为base进制的字符串
 * 将一个无符号整数n，按照指定进制base进行转换，将转换结果存入str中
 * 返回值就是字符串尾指针
 */
static char_t *numberk(char_t *str, uint_t n, sint_t base){
    register char_t *p; //缓冲区指针
    char_t strbuf[36];  //字符串缓冲区，用于临时存放中间计算结果
    p = &strbuf[36];    //p=strbuf+36,指向strbuf字符串最后一个元素的后一个元素
    *--p = 0;
    if (n == 0){
        *--p = '0';
    }
    else{
        do{
            *--p = "0123456789abcdef"[n % base];    //p始终指向最后填充的数字
        } while (n /= base);
    }
    while (*p != 0){    //字符串结束符为0
        *str++ = *p++;
    }
    return str; //写入完成后返回下一个字符位置
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

#pragma GCC push_options
#pragma GCC optimize ("O0")
void die(u32_t dt){
    if(dt==0){
        while(1);
    }

    for(u32_t i=0;i<dt;i++){
        for(u32_t j=0;j<dt;j++){
            for(u32_t k=0;k<dt;k++){
                ;
            }
        }
    }

    return;
}
#pragma GCC pop_options