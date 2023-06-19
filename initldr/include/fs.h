/**
 * 文件操作头文件 
 */

#ifndef _FS_H
#define _FS_H


/**
 * 
 */
int strcmpl(const char *a,const char *b);

/**
 * 
 */
void display();

/**
 * 
 */
int move_krlimg(machbstart_t* mbsp,u64_t cpyadr,u64_t cpysz);

/**
 * 
 */
void init_krlfile(machbstart_t* mbsp);

/**
 * 
 */
void init_defutfont(machbstart_t *mbsp);

/**
 * 
 */
fhdsc_t* get_fileinfo(char_t* fname,machbstart_t* mbsp);

/**
 * 
 */
void get_file_rpadrandsz(char_t *fname, machbstart_t* mbsp,u32_t* retadr,u32_t* retsz);

/**
 * 
 */
u64_t get_filesz(char_t* fname,machbstart_t* mbsp);

/**
 * 
 */
u64_t get_wt_imgfilesz(machbstart_t* mbsp);

/**
 * 
 */
u64_t r_file_to_padr(machbstart_t* mbsp,u32_t f2adr,char_t* fname);


#endif // FS_H
