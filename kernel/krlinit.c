/**
 * kernel全局初始化文件
 */

#include "cosmostypes.h"
#include "cosmosmctrl.h"


/**
 * 内核初始化函数
 */
void init_krl(){
	// init_krlmm();		//初始化内存
	hint("内存初始化完成\n");

	// init_krldevice();	//初始化设备
	hint("设备初始化完成\n");
	
	// init_krldriver();	//初始化驱动程序
	hint("驱动程序初始化完成\n");
	
	// init_krlsched();		//初始化调度器
	hint("调度器初始化完成\n");
	
	// init_ktime();		//初始化内核定时器
	hint("内核定时器初始化完成\n");
	
	// init_thread();		//初始化线程管理
	hint("线程管理初始化完成\n");
	
	// init_task();			//初始化任务
	hint("任务初始化完成\n");
	
	// init_krlcpuidle();	//初始化CPU空闲状态
	hint("CPU空闲状态初始化完成\n");
	
	// hal_enable_irqfiq();
	hint("即将进入桌面...\n");

	fill_fullbitmap("background.bmp");

	die(0);	//不返回
	return;
}