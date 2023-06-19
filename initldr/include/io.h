/**
 * 输入/输出头文件
 */

#ifndef _IO_H
#define _IO_H

/**
 * 指定端口写入一个字节，用于文本模式显卡控制
 * out byte
 */
KLINE void out_u8(const u16_t port, const u8_t val){
    __asm__ __volatile__("outb  %1, %0\n"
             :
             : "dN"(port), "a"(val));
}

/**
 * 指定端口读取一个字节，用于文本模式显卡控制
 * in byte
 */
KLINE u8_t in_u8(const u16_t port){
    u8_t ret;
    __asm__ __volatile__("inb %1, %0\n"
             : "=a"(ret)
             : "dN"(port));
    return ret;
}

/**
 * 向端口写一个字，用于文本模式显卡控制
 * out word
 */
KLINE void out_u16(const u16_t port, const u16_t val){
    __asm__ __volatile__("outw  %1, %0\n"
             :
             : "dN"(port), "a"(val));
}

/**
 * 从端口读一个字，用于文本模式显卡控制
 * in word
 */
KLINE u16_t in_u16(const u16_t port){
    u16_t ret;

    __asm__ __volatile__("inw %1, %0\n"
             : "=a"(ret)
             : "dN"(port));
    return ret;
};

/**
 * 内存拷贝：从源地址拷贝指定长度子节到目标地址
 * sadr：源地址
 * dadr：目标地址
 * len：拷贝字节长度
 * 问题：这里的地址为什么用void*,这里的len为什么用sint_t而不是uint_t或size_t?
 * 问题：为什么要逐子节拷贝
 */
KLINE sint_t m2mcopy(void* sadr,void* dadr,sint_t len){
    if(NULL==sadr||NULL==dadr||1>len){
        return 0;
    }
    //sadr和dadr转换为8位指针，原先的void*指针每次+1，可能内存地址会加1B、2B、4B或8B，甚至更多，但是通过转换为八位数据类型的指针后，统一为每次增减1B
    u8_t* s=(u8_t*)sadr,*d=(u8_t*)dadr;
    if(s<d){
        for(sint_t i=(len-1);i>=0;i--){
            d[i]=s[i];  //将s[]逐子节拷贝到d[]中
        }
        return len;
    }
    if(s>d){
        for(sint_t j=0;j<len;j++){
            d[j]=s[j];
        }
        return len;
    }
    if(s==d){    //源地址和目标地址重合，没必要m2mcopy
        return len;
    }
    return 0;
}

/**
 * 功能同C库函数memset
 */
KLINE void memset(void* src,u8_t val,uint_t count){
    u8_t* ss=src;   //转换为u8_t类型，便于做子节处理
    for(uint_t i=0;i<count;i++){
        ss[i]=val;      //不能写成src[i]=val,这里就提现了src强转的价值了
    }
    return;
}

#endif
