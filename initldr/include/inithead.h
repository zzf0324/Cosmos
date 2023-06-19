/**
 * 
 */

#ifndef _INITHEAD_H
#define _INITHEAD_H


/**
 * 
 */
void inithead_entry();

/**
 * 
 */
void write_shellfile();

/**
 * 
 */
void write_kernelfile();

/**
 * 
 */
void write_realintsvefile();

/**
 * 从内核映像eki中查找文件
 */
fhdsc_t* find_file(char_t* fname);

/**
 * 
 */
void write_ldrkrlfile();

/**
 * 错误处理：提示错误信息
 */
void error(char_t* estr);

/**
 * 字符串比较：返回两个字符串的大小
 */
int strcmpl(const char *a,const char *b);


#endif // INITHEAD_H
