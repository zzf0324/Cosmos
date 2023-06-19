/**
 * 引导调式屏幕上显示输出文件
 */

#ifndef _BDVIDEO_H
#define _BDVIDEO_H


/**
 * 初始化图形数据结构（dftgraph_t），里面放有图形模式，分辨率，图形驱动指针
 */
void init_dftgraph();

/**
 * 将位图显示在屏幕上
 */
void fill_fullbitmap(char_t *fname);

/**
 * 显示hal的logo
 */
void hal_logo();

/**
 * 
 */
void red();

/**
 * 
 */
void gre();

/**
 * 
 */
void blu();

/**
 * 
 */
void blk();

/**
 * 
 */
void wri();

/**
 * 初始化图形显示驱动
 */
void init_bdvideo();

/**
 * 初始化bga图形显卡的函数指针
 */
void init_bga();

/**
 * 初始化vbe图形显卡的函数指针
 */
void init_vbe();

/**
 * 使用特定颜色清空屏幕
 */
void fill_graph(dftgraph_t* kghp,pixl_t pix);

/**
 * 
 */
void write_pixcolor(dftgraph_t* kghp,u32_t x,u32_t y,pixl_t pix);

/**
 * 
 */
void drxw_pixcolor(dftgraph_t* kghp,u32_t x,u32_t y,pixl_t pix);

/**
 * 刷新显存
 */
void flush_videoram(dftgraph_t* kghp);

/**
 * 
 */
u32_t utf8_to_unicode(utf8_t* utfp,int* retuib);

/**
 * 
 */
u8_t* ret_charsinfo(dftgraph_t* kghp,u32_t unicode,int* retchwx,int* retchhx,int* retlinb);

/**
 * 
 */
void put_pixonechar(dftgraph_t* kghp,u32_t unicode,pixl_t pix);

/**
 * 
 */
void drxput_pixonechar(dftgraph_t* kghp,u32_t unicode,pixl_t pix);

/**
 * 
 */
void gstr_write(dftgraph_t* kghp,char_t* buf);


/*====================================BGA Mode========================================*/

/**
 * 
 */
size_t bga_read(void* ghpdev,void* outp,size_t rdsz);

/**
 * 
 */
size_t bga_write(void* ghpdev,void* inp,size_t wesz);

/**
 * 
 */
sint_t bga_ioctrl(void* ghpdev,void* outp,uint_t iocode);

/**
 * 
 */
void bga_flush(void* ghpdev);

/**
 * 
 */
sint_t bga_set_bank(void* ghpdev, sint_t bnr);

/**
 * 
 */
pixl_t bga_readpix(void* ghpdev,uint_t x,uint_t y);

/**
 * 
 */
void bga_writepix(void* ghpdev,pixl_t pix,uint_t x,uint_t y);

/**
 * 
 */
pixl_t bga_dxreadpix(void* ghpdev,uint_t x,uint_t y);

/**
 * 
 */
void bga_dxwritepix(void* ghpdev,pixl_t pix,uint_t x,uint_t y);

/**
 * 
 */
sint_t bga_set_xy(void* ghpdev,uint_t x,uint_t y);

/**
 * 
 */
sint_t bga_set_vwh(void* ghpdev,uint_t vwt,uint_t vhi);

/**
 * 
 */
sint_t bga_set_xyoffset(void* ghpdev,uint_t xoff,uint_t yoff);

/**
 * 
 */
sint_t bga_get_xy(void* ghpdev,uint_t* rx,uint_t* ry);

/**
 * 
 */
sint_t bga_get_vwh(void* ghpdev,uint_t* rvwt,uint_t* rvhi);

/**
 * 
 */
sint_t bga_get_xyoffset(void* ghpdev,uint_t* rxoff,uint_t* ryoff);



/*=========================================VBE Mode==========================================*/

/**
 * 
 */
size_t vbe_read(void* ghpdev,void* outp,size_t rdsz);

/**
 * 
 */
size_t vbe_write(void* ghpdev,void* inp,size_t wesz);

/**
 * 
 */
sint_t vbe_ioctrl(void* ghpdev,void* outp,uint_t iocode);

/**
 * 
 */
void vbe_flush(void* ghpdev);

/**
 * 
 */
sint_t vbe_set_bank(void* ghpdev, sint_t bnr);

/**
 * 
 */
pixl_t vbe_readpix(void* ghpdev,uint_t x,uint_t y);

/**
 * 
 */
void vbe_writepix(void* ghpdev,pixl_t pix,uint_t x,uint_t y);

/**
 * 
 */
pixl_t vbe_dxreadpix(void* ghpdev,uint_t x,uint_t y);

/**
 * 
 */
void vbe_dxwritepix(void* ghpdev,pixl_t pix,uint_t x,uint_t y);

/**
 * 
 */
sint_t vbe_set_xy(void* ghpdev,uint_t x,uint_t y);

/**
 * 
 */
sint_t vbe_set_vwh(void* ghpdev,uint_t vwt,uint_t vhi);

/**
 * 
 */
sint_t vbe_set_xyoffset(void* ghpdev,uint_t xoff,uint_t yoff);

/**
 * 
 */
sint_t vbe_get_xy(void* ghpdev,uint_t* rx,uint_t* ry);

/**
 * 
 */
sint_t vbe_get_vwh(void* ghpdev,uint_t* rvwt,uint_t* rvhi);

/**
 * 
 */
sint_t vbe_get_xyoffset(void* ghpdev,uint_t* rxoff,uint_t* ryoff);


#endif // BDVIDEO_H
