/**
 * 显卡vbe模式
 */

#include "cmctl.h"

/**
 * 
 */
void init_graph(machbstart_t* mbsp){
    //初始化图形数据结构
    graph_t_init(&mbsp->mb_ghparm);

    init_bgadevice(mbsp);
    if(mbsp->mb_ghparm.gh_mode!=BGAMODE){
        get_vbemode(mbsp);      //获取VBE模式（通过BIOS中断）
        get_vbemodeinfo(mbsp);  //获取一个具体的VBE模式的信息（通过BIOS中断）
        set_vbemodeinfo();      //设置VBE模式，（通过BIOS中断）
    }

    init_kinitfvram(mbsp);
    return;
}

/**
 * 初始化图形结构体
 */
void graph_t_init(graph_t* initp){
    memset(initp,0,sizeof(graph_t));
    return;
}

/**
 * 
 */
void init_kinitfvram(machbstart_t* mbsp){
    mbsp->mb_fvrmphyadr=KINITFRVM_PHYADR;
    mbsp->mb_fvrmsz=KINITFRVM_SZ;
    memset((void*)KINITFRVM_PHYADR,0,KINITFRVM_SZ);

    return;
}

/**
 * 
 */
u32_t vfartolineadr(u32_t vfar){
    u32_t tmps=0,sec=0,off=0;
    off=vfar&0xffff;
    tmps=(vfar>>16)&0xffff;
    sec=tmps<<4;
    return (sec+off);
}

/**
 * 
 */
void get_vbemode(machbstart_t* mbsp){
    realadr_call_entry(RLINTNR(2),0,0);
    vbeinfo_t* vbeinfoptr=(vbeinfo_t*)VBEINFO_ADR;
    u16_t* mnm;

    if(vbeinfoptr->vbesignature[0]!='V'||
            vbeinfoptr->vbesignature[1]!='E'||
            vbeinfoptr->vbesignature[2]!='S'||
            vbeinfoptr->vbesignature[3]!='A'){
        kerror("vbe is not VESA");
    }
    kprint("vbe vbever:%x\n",vbeinfoptr->vbeversion);
    if(vbeinfoptr->vbeversion<0x0200){
        kerror("vbe version not vbe3");
    }

    if(vbeinfoptr->videomodeptr>0xffff){
        mnm=(u16_t*)vfartolineadr(vbeinfoptr->videomodeptr);//
    }
    else{
        mnm=(u16_t*)(vbeinfoptr->videomodeptr);
    }

    int bm=0;
    for(int i=0;mnm[i]!=0xffff;i++){
        if(mnm[i]==0x118){
            bm=1;
        }
        if(i>0x1000){
            break;
        }

    }

    if(bm==0){
        kerror("getvbemode not 118");
    }
    mbsp->mb_ghparm.gh_mode=VBEMODE;
    mbsp->mb_ghparm.gh_vbemodenr=0x118;
    mbsp->mb_ghparm.gh_vifphyadr=VBEINFO_ADR;
    m2mcopy(vbeinfoptr,&mbsp->mb_ghparm.gh_vbeinfo,sizeof(vbeinfo_t));
    return;
}

/**
 * 
 */
void bga_write_reg(u16_t index, u16_t data){
    out_u16(VBE_DISPI_IOPORT_INDEX, index);
    out_u16(VBE_DISPI_IOPORT_DATA, data);
    return;
}

/**
 * 
 */
u16_t bga_read_reg(u16_t index){
    out_u16(VBE_DISPI_IOPORT_INDEX, index);
    return in_u16(VBE_DISPI_IOPORT_DATA);
}

/**
 * 
 */
u32_t get_bgadevice(){
    u16_t bgaid=bga_read_reg(VBE_DISPI_INDEX_ID);
    if(BGA_DEV_ID0<=bgaid&&BGA_DEV_ID5>=bgaid){
        bga_write_reg(VBE_DISPI_INDEX_ID,bgaid);
        if(bga_read_reg(VBE_DISPI_INDEX_ID)!=bgaid){
            return 0;
        }
        return (u32_t)bgaid;
    }
    return 0;
}

/**
 * 
 */
u32_t chk_bgamaxver(){
    bga_write_reg(VBE_DISPI_INDEX_ID,BGA_DEV_ID5);
    if(bga_read_reg(VBE_DISPI_INDEX_ID)==BGA_DEV_ID5){
        return (u32_t)BGA_DEV_ID5;
    }
    bga_write_reg(VBE_DISPI_INDEX_ID,BGA_DEV_ID4);
    if(bga_read_reg(VBE_DISPI_INDEX_ID)==BGA_DEV_ID4){
        return (u32_t)BGA_DEV_ID4;
    }
    bga_write_reg(VBE_DISPI_INDEX_ID,BGA_DEV_ID3);
    if(bga_read_reg(VBE_DISPI_INDEX_ID)==BGA_DEV_ID3){
        return (u32_t)BGA_DEV_ID3;
    }
    bga_write_reg(VBE_DISPI_INDEX_ID,BGA_DEV_ID2);
    if(bga_read_reg(VBE_DISPI_INDEX_ID)==BGA_DEV_ID2){
        return (u32_t)BGA_DEV_ID2;
    }
    bga_write_reg(VBE_DISPI_INDEX_ID,BGA_DEV_ID1);
    if(bga_read_reg(VBE_DISPI_INDEX_ID)==BGA_DEV_ID1){
        return (u32_t)BGA_DEV_ID1;
    }
    bga_write_reg(VBE_DISPI_INDEX_ID,BGA_DEV_ID0);
    if(bga_read_reg(VBE_DISPI_INDEX_ID)==BGA_DEV_ID0){
        return (u32_t)BGA_DEV_ID0;
    }
    return 0;
}


/**
 * 
 */
void init_bgadevice(machbstart_t* mbsp){
    u32_t retdevid=get_bgadevice();
    if(0==retdevid){
        return;
    }
    retdevid=chk_bgamaxver();
    if(0==retdevid){
        return;
    }
    bga_write_reg(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_DISABLED);
    bga_write_reg(VBE_DISPI_INDEX_XRES, 1024);
    bga_write_reg(VBE_DISPI_INDEX_YRES, 768);
    bga_write_reg(VBE_DISPI_INDEX_BPP, 0x20);
    bga_write_reg(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_ENABLED|(VBE_DISPI_LFB_ENABLED)); 
    mbsp->mb_ghparm.gh_mode=BGAMODE;
    mbsp->mb_ghparm.gh_vbemodenr=retdevid;
    mbsp->mb_ghparm.gh_x=1024;
    mbsp->mb_ghparm.gh_y=768;
    mbsp->mb_ghparm.gh_framphyadr=0xe0000000;   //vbe模式显存地址
    mbsp->mb_ghparm.gh_onepixbits=0x20;
    mbsp->mb_ghparm.gh_bank=4;
    mbsp->mb_ghparm.gh_curdipbnk=0;
    mbsp->mb_ghparm.gh_nextbnk=0;
    mbsp->mb_ghparm.gh_banksz=(mbsp->mb_ghparm.gh_x*mbsp->mb_ghparm.gh_x*4);
    return;
}


/**
 * 
 */
void get_vbemodeinfo(machbstart_t* mbsp){
    realadr_call_entry(RLINTNR(3),0,0);		//调用BIOS中断，获取硬件的VBE信息
    vbeominfo_t* vomif=(vbeominfo_t*)VBEMINFO_ADR;
    u32_t x=vomif->XResolution,y=vomif->YResolution;
    u32_t* phybass=(u32_t*)(vomif->PhysBasePtr);
    if(vomif->BitsPerPixel<24){
        kerror("vomif->BitsPerPixel!=32");
    }
    if(x!=1024||y!=768){
        kerror("xy not");
    }
    if((u32_t)phybass<0x100000){
        kerror("phybass not");
    }
    mbsp->mb_ghparm.gh_x=vomif->XResolution;
    mbsp->mb_ghparm.gh_y=vomif->YResolution;
    mbsp->mb_ghparm.gh_framphyadr=vomif->PhysBasePtr;
    mbsp->mb_ghparm.gh_onepixbits=vomif->BitsPerPixel;
    mbsp->mb_ghparm.gh_vmifphyadr=VBEMINFO_ADR;
    m2mcopy(vomif,&mbsp->mb_ghparm.gh_vminfo,sizeof(vbeominfo_t));

    return;
}

/**
 * 调用BIOS中断，设置显卡为VBE模式
 */
void set_vbemodeinfo(){
    realadr_call_entry(RLINTNR(4),0,0);
    return;
}