/**
 * 系统全局内存检查文件
 */

#include "cmctl.h"

static void display_e820s(e820map_t *e820s,u32_t size){
    for(u32_t i=0;i<size;i++){
    	kprint("%d\t",i);
        kprint("%d\t",e820s[i].saddr);
        kprint("%d\t",e820s[i].lsize);
        kprint("%d\n",e820s[i].type);
    }
}

/**
 * 初始化内存
 * 获取内存视图布局，填写machbstart_t对应字段，检查内存大小是否够用
 */
void init_mem(machbstart_t *mbsp){
    e820map_t *addr;  //e820数组指针
    u32_t len = 0;  //e820数组元素数量
    
    mmap(&addr, &len);  //获取内存视图的e820数组
    
    if (len == 0){  //e820数组元素个数为0，说明没有e820数组
        kerror("no e820map\n");
    }
    
    //根据e820数组中获取的内存视图检查内存大小（内存不能少于128MB）
    if (chk_memsize(addr, len, 0x100000, 0x8000000) == NULL){ //0x8000000=128MB，0x100000=1MB
        kerror("Your computer is low on memory, the memory cannot be less than 128MB!");
    }

    mbsp->mb_e820padr = (u64_t)((u32_t)(addr));         //记录e820map_t的首地址
    mbsp->mb_e820nr = (u64_t)len;                       //记录e820map_t结构体数组元素个数
    mbsp->mb_e820sz = len * (sizeof(e820map_t));        //记录e820map_t结构体数组大小(B)
    mbsp->mb_memsz = get_memsize(addr, len);            //根据e820结构体数组大小计算内存大小
    kprint("address:%x\n",mbsp->mb_e820padr);
    kprint("number:%d\n",mbsp->mb_e820nr);
    kprint("e820 size:%d\n",mbsp->mb_e820sz);
    kprint("mem size:%d\n",mbsp->mb_e820sz);
	//这个e820数组的大小8*20=160
    return;
}

/**
 * 检查CPU主函数
 */
void init_chkcpu(machbstart_t *mbsp){
    //检查是否支持cpuid指令
    if (!chk_cpuid()){
        kerror("Your CPU is not support CPUID sys is die!");
        CLI_HALT(); //主动死机
    }
    //使用cpuid指令检查cpu是否支持
    if (!chk_cpu_longmode()){
        kerror("Your CPU is not support 64bits mode sys is die!");
        CLI_HALT(); //主动死机
    }
    mbsp->mb_cpumode = 0x40;    //如果成功则设置机器信息结构的cpu模式为64位(0x40)
    return;
}

/**
 * 初始化内核栈，内核C语言运行环境需要栈
 */
void init_krlstack(machbstart_t *mbsp){
    if (1 > move_krlimg(mbsp, (u64_t)(0x8f000), 0x1001)){   //判断地址空间是否和内存中存放的内容有冲突
        kerror("init kernel moveimg err");
    }
    //内核栈地址空间不冲突，填写相关的machbstart_t字段
    mbsp->mb_krlinitstack = IKSTACK_PHYADR;
    mbsp->mb_krlitstacksz = IKSTACK_SIZE;
    return;
}

/**
 * 初始化MMU页表
 * 1个顶级页目录，1个二级页目录，16个三级页目录，也就是说顶级页目录只是使用了一个目录项p[0],二级页目录只是用了16个页表项pdpte[0-15]
 * (addr >> KPML4_SHIFT) & 0x1ff，一个虚拟地址经过这一步变换之后提取出的addr[48-40]得到的正好是顶级页目录索引
 */ 
void init_bstartpages(machbstart_t *mbsp){
    u64_t *p = (u64_t *)(KINITPAGE_PHYADR);     //一级页目录
    u64_t *pdpte = (u64_t *)(KINITPAGE_PHYADR + 0x1000);    //二级页目录
    u64_t *pde = (u64_t *)(KINITPAGE_PHYADR + 0x2000);      //三级页目录

    u64_t adr = 0;      //从物理地址0开始
    
    //判断存放页目录的地址是否可用，0x1000+0x1000+0x1000*16对应1个一级页目录，一个二级页目录和16个三级页目录占用的总内存大小
    if (1 > move_krlimg(mbsp, (u64_t)(KINITPAGE_PHYADR), (0x1000+0x1000+0x1000*16))){
        kerror("no memory to place mmu page catalog");
    }
    //将一级页目录，二级页目录清零
    for (uint_t i = 0; i < PGENTY_SIZE; i++){
        p[i] = 0;       //清空一级页目录
        pdpte[i] = 0;   //清空二级页目录
    }
    
    //映射(可以直接使用或运算写入标志是因为页面2MB对齐，页地址低21位为0)
    for (uint_t pdei = 0; pdei < 16; pdei++){
        pdpte[pdei] = (u64_t)((u32_t)pde | KPDPTE_RW | KPDPTE_P);
        for (uint_t pdeii = 0; pdeii < PGENTY_SIZE; pdeii++){   //三级页目录共512项
            pde[pdeii] = 0 | adr | KPDE_PS | KPDE_RW | KPDE_P;
            adr += 0x200000;    //指向下一个页
        }
        pde = (u64_t *)((u32_t)pde + 0x1000);   //指向下一个三级页目录
    }

    /**
     * 以下两步初始化正好将0xffff800000000000和0x0000000000000000映射到物理地址空间
     * 将虚拟地址0x0映射到物理地址0x0的操作就是平坦映射，它使得虚拟地址=物理地址，使用平坦映射的原因是，开启MMU之后，CPU看到的地址都认为是虚拟地址，
     * 但是这个时候之前的一些指令还是使用的物理地址，但是CPU会把这个地址当成虚拟地址进行访问，这个时候需要虚拟地址(被当成虚拟地址的物理地址)仍然映射到原来物理地址。也就相当于没有开启MMU
     */
    //使得内核虚拟空间起始地址0xffff80000000000的地址映射到16GB物理地址0x0处
    p[((KRNL_VIRTUAL_ADDRESS_START) >> KPML4_SHIFT) & 0x1ff] = (u64_t)((u32_t)pdpte | KPML4_RW | KPML4_P);
    //使得内核虚拟空间起始地址0x000000000000000的地址映射到16GB物理地址0x0处
    p[0] = (u64_t)((u32_t)pdpte | KPML4_RW | KPML4_P);  //顶级页目录只使用了第一个目录项p[0]
    
    //在machbstart_t中记录页表的相关信息
    mbsp->mb_pml4padr = (u64_t)(KINITPAGE_PHYADR);  //顶级页目录地址
    mbsp->mb_subpageslen = (u64_t)(0x1000 * 16 + 0x2000);   //页目录总占用内存空间大小
    mbsp->mb_kpmapphymemsz = (u64_t)(0x400000000);  //16GB的映射内存空间
    return;
}

/**
 * 暂时不确定
 */
void init_meme820(machbstart_t *mbsp){
    //源地址
    e820map_t *semp = (e820map_t *)((u32_t)(mbsp->mb_e820padr));
    //e820数组元素数
    u64_t senr = mbsp->mb_e820nr;
    //目标地址
    e820map_t *demp = (e820map_t *)((u32_t)(mbsp->mb_nextwtpadr));
    //
    if (1 > move_krlimg(mbsp, (u64_t)((u32_t)demp), (senr * (sizeof(e820map_t))))){
        kerror("move_krlimg err");
    }

    display_e820s((e820map_t*)((u32_t)(mbsp->mb_e820padr)),(int)mbsp->mb_e820nr);
    
    m2mcopy(semp, demp, (sint_t)(senr * (sizeof(e820map_t))));
    mbsp->mb_e820padr = (u64_t)((u32_t)(demp));
    mbsp->mb_e820sz = senr * (sizeof(e820map_t));
    mbsp->mb_nextwtpadr = P4K_ALIGN((u32_t)(demp) + (u32_t)(senr * (sizeof(e820map_t))));
    mbsp->mb_kalldendpadr = mbsp->mb_e820padr + mbsp->mb_e820sz;
    return;
}

/**
 * 通过调用实模式BIOS中断获取内存视图的e820数组
 */
void mmap(e820map_t **e820s, u32_t *size){
    realadr_call_entry(RLINTNR(0), 0, 0);   //进入实模式调用BIOS中断，获取e820数组，并存入指定地址下
    *size = *((u32_t *)(E80MAP_NR));        //使用指针共享汇编收集的数组大小信息，地址值只是一个无符号数，只有转换为具体类型的指针，才能正确访问和解释该地址下的数据
    *e820s = (e820map_t *)(*((u32_t *)(E80MAP_ADRADR)));    //使用指针，共享汇编收集的数组地址信息
    return;
}

/**
 * 检查内存大小
 */
e820map_t *chk_memsize(e820map_t *e8p, u32_t enr, u64_t sadr, u64_t size){
    u64_t len = sadr + size;
    if (enr == 0 || e8p == NULL){   //e820数组检查
        return NULL;
    }
    for (u32_t i = 0; i < enr; i++){
        if (e8p[i].type == RAM_USABLE){
            if ((sadr >= e8p[i].saddr) && (len < (e8p[i].saddr + e8p[i].lsize))){
                return &e8p[i];
            }
        }
    }
    return NULL;
}

/**
 * 获取可用内存(USEABLE)大小
 * 遍历e820数组，求出各个可用内存区域(USABSE)的大小总和
 */
u64_t get_memsize(e820map_t *e820s, u32_t size){
    u64_t sum = 0;
    if (size == 0 || e820s == NULL){   //参数错误
        return 0;
    }
    for (u32_t i = 0; i < size; i++){
        if (e820s[i].type == RAM_USABLE){   //只考虑卡通内存
            sum += e820s[i].lsize;
        }
    }
    return sum;
}

/**
 * 通过改写Eflags寄存器的第21位，观察其位的变化判断是否支持cpuid指令，为检查cpu是否支持长模式做准备
 */
int chk_cpuid(){
    int rets = 0;
    __asm__ __volatile__(
        "pushfl \n\t"
        "popl %%eax \n\t"
        "movl %%eax,%%ebx \n\t"
        "xorl $0x0200000,%%eax \n\t"
        "pushl %%eax \n\t"
        "popfl \n\t"
        "pushfl \n\t"
        "popl %%eax \n\t"
        "xorl %%ebx,%%eax \n\t"
        "jz 1f \n\t"
        "movl $1,%0 \n\t"
        "jmp 2f \n\t"
        "1: movl $0,%0 \n\t"
        "2: \n\t"
        : "=c"(rets)
        :
        :);
    return rets;
}

/**
 * 检查CPU是否支持长模式，检查是否支持长模式需要用到cpuid指令，因此需要先使用chk_cpuid()函数判断是否支持cpuid指令
 */
int chk_cpu_longmode(){
    int rets = 0;
    __asm__ __volatile__(
        "movl $0x80000000,%%eax \n\t"
        "cpuid \n\t"        //把eax中放入0x80000000调用CPUID指令
        "cmpl $0x80000001,%%eax \n\t"   //看eax中返回结果
        "setnb %%al \n\t"   //不为0x80000001,则不支持0x80000001号功能
        "jb 1f \n\t"
        "movl $0x80000001,%%eax \n\t"
        "cpuid \n\t"        //把eax中放入0x800000001调用CPUID指令，检查edx中的返回数据
        "bt $29,%%edx  \n\t"            // long mode  support 位
        "setcb %%al \n\t"
        "1: \n\t"
        "movzx %%al,%%eax \n\t"
        : "=a"(rets)
        :
        :);
    return rets;
}