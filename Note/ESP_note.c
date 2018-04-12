/*
*		
*/
1 SPI概述
      SPI是英语Serial Peripheral interface的缩写，顾名思义就是串行外围设备接口，是Motorola首先在其MC68HCXX系列处理器上定义的。SPI接口主要应用在 EEPROM，FLASH，实时时钟，AD转换器，还有数字信号处理器和数字信号解码器之间。SPI是一种高速的，全双工，同步的通信总线，并且在芯片的管脚上只占用四根线，节约了芯片的管脚，同时为PCB的布局上节省空间，提供方便。
      SPI的通信原理很简单，它以主从方式工作，这种模式通常有一个主设备和一个或多个从设备，需要4根线，事实上3根也可以。也是所有基于SPI的设备共有的，它们是SDI(数据输入)，SDO(数据输出)，SCLK(时钟)，CS(片选)。
      MOSI(SDO)：主器件数据输出，从器件数据输入。
      MISO(SDI)：主器件数据输入，从器件数据输出。
      SCLK ：时钟信号，由主器件产生。
      CS：从器件使能信号，由主器件控制。
      其中CS是控制芯片是否被选中的，也就是说只有片选信号为预先规定的使能信号时（高电位或低电位），对此芯片的操作才有效，这就允许在同一总线上连接多个SPI设备成为可能。需要注意的是，在具体的应用中，当一条SPI总线上连接有多个设备时，SPI本身的CS有可能被其他的GPIO脚代替，即每个设备的CS脚被连接到处理器端不同的GPIO，通过操作不同的GPIO口来控制具体的需要操作的SPI设备，减少各个SPI设备间的干扰。
      SPI是串行通讯协议，也就是说数据是一位一位从MSB或者LSB开始传输的，这就是SCK时钟线存在的原因，由SCK提供时钟脉冲，MISO、MOSI则基于此脉冲完成数据传输。 SPI支持4-32bits的串行数据传输，支持MSB和LSB，每次数据传输时当从设备的大小端发生变化时需要重新设置SPI Master的大小端。



/*
 * 		spi_device 结构体
 */
1.SPI通用接口层用spi_device结构来表示一个spi设备，它的各个字段的意义如下：
struct device           	dev 	代表该spi设备的device结构
struct spi_master       	*master 	指向该spi设备所使用的控制器
u32     max_speed_hz 		该设备的最大工作时钟频率
u8      chip_select 		在控制器中的片选引脚编号索引
u16     mode 				设备的工作模式，包括时钟格式，片选信号的有效电平等等
u8      bits_per_word 		设备每个单位数据所需要的比特数
int     irq 				设备使用的irq编号
char    modalias[SPI_NAME_SIZE] 	该设备的名字，用于spi总线和驱动进行配对
int     cs_gpio 			片选信号的gpio编号，通常不用我们自己设置，接口层会根据上面的chip_select字段在spi_master结构中进行查找并赋值

/*
 * 	Linux　device driver
 * 
 */
第二章
1.新符号
lsmod  	// 查看系统模块列表
insmod　 // 模块的安装　	　　使用方法：　insmod 模块文件名  
modinfo // 查看内核模块信息　　使用方法：　modinfo 模块名/文件名  
modprobe// 模块的安装　	　　使用方法：　modprobe 模块文件名  
rmmod	// 模块的卸载		　　使用方法：　rmmod 模块名 　　// modprobe -r 模块名  删除模块 

2.用于装载模块到运行的内核和移除模块的用户空间工具
#include <linux/init.h>
module_init(init_function);	// 是一个宏　  #define module_init(x)  __initcall(x);  
module_exit(cleanup_function);
// 用来创建模块参数的宏，用户可在装载模块时，调整这些参数的值
// 其中类型可以是bool,charp,int,invbool,long,short,ushort,uint,ulong或者intarray
module_param(variable, type, perm); 


3.用于指定模块的初始化和清楚函数的宏
__init
__initdata
__exit
__exitdata
仅用于模块初始化或清除阶段的函数(__init和__exit)和数据(__initdata/__exitdata)

4.最重要的头文件之一
#include <linux/sched.h>
该文件包含驱动程序使用的大部分内核API的定义，包括睡眠函数以及各种变量声明
#include <linux/module.h> // 必要的头文件，它必须包含在模块源代码中。

5.获取当前进程
struct task_struct *current;
current->pid
current->comm
当前进程的ID和命令名

6.目录
/sys/module是sysfs目录层次结构中包含当前已装载模块信息的目录
/proc/modules是早起用法
vermagic.o　：　内核源代码目录中的一个目标文件，它描述了模块的构造环境

7.版本info
#include <linux/version.h> // 包含所构造内核版本信息的头文件
LINUX_VERSION_CODE	整数宏，在处理版本依赖的预处理条件语句中非常有用

EXPORT_SYMBOL (symbol);　　// 用来到处单个符号到内核的宏
EXPORT_SYMBOL_GPL (symbol); //　将导出符号的使用限于GPL许可证下的模块

MODULE_AUTHOR(author)；
MODULE_DESCRIPTION(description);
MODULE_VERSION(version_string);
MODULE_DEVICE_TABLE(table_info);
MODULE_ALIAS(alternate_name);
//在目标文件中添加关于模块的文档信息

8.内核打印
#include <linux/kernel.h>
int printk(const char* fmt, ...); // 函数printf的内核代码


/*
 * 		第三章　　字符设备驱动程序
 *  
*/
1.主设备号和次设备号
	对字符设备的访问时通过文件系统内的设备名称进行的,那些名称被称为特殊文件
设备文件或者简单称之为文件系统树的节点。它们通常位于/dev目录，
字符设备驱动程序的设备文件
可通过ls -l命令输出的第一列中的　“c”来识别, 
块设备也在/dev下，但它们由字符“b”标识
ls -l
crw-rw-rw- 1 media     media        227,   0 2018-03-31 18:00 fm
crw-rw---- 1 system    system        10,  39 2018-03-31 18:00 gsensor
crw-rw-rw- 1 root      root          10,  51 2018-03-31 18:00 hwbinder
drwxr-xr-x 2 root      root              180 2018-03-31 18:00 input
crw------- 1 root      root           3,   0 2018-03-31 18:00 tty1
crw------- 1 root      root           3,   0 2018-03-31 18:00 ttys0
crw------- 1 root      root           3,   1 2018-03-31 18:00 ttyS1

通过查看上方第一列的第一个区分是字符设备还是块设备，
时间前面两个整数,分别是　主设备号　跟　次设备号

主设备号标识设备对应的驱动程序，
例子:
/dev/gsensor 和　/dev/hwbinder　由驱动程序10管理
而虚拟控制台和串口终端由驱动程序3管理

1.1.现在的Linux内核允许多个驱动程序共享一个主设备号，但我们看到的大多数
设备仍然按照"一个主设备号对应一个驱动程序"的原则组织.

1.2次设备号由内核使用，用于正确确定设备文件所指的设备，依赖于驱动程序的编写方式。

1.3设备编号的内部表达
在内核在宏,　dev_t类型（在<linux/tyep.h>中定义）用来保存设备编号————包括
主设备号和次设备号，dev_t是一个32位的数，
其中12为用来表示	主设备号
其余的20位用来表示　次设备号

要获得dev_t的主设备号或次设备号:应使用
#include <linux/kdev_t.h>
MAJOR(dev_t dev);
MINOR(dev_t dev);
相反，如果需要将主设备号和次设备号转换成dev_t类型，则使用:
MKDEV(int major, int minor);

// 主设备号用来区分 不同种类 的设备，而次设备号用来区分 同一类型 的多个设备

1.4分配和释放设备编号
驱动程序首先要做的事情就是获得一个或者多个设备编号，完成该工作的必要函数register_chrdev_region
#include <linux/fs.h>
int register_chrdev_region (dev_t first, unsigned int count, char* name);

first:	次设备号经常被置为0
count:	是所请求的连续设备编号的个数
name:	是和该编号范围关联的设备名称，它将出现在/proc/devices和sysfs中。
返回值: 分配成功返回0，错误返回负的错误码，并且不能使用所请求的编号区域

int alloc_chrdev_region(dev_t *dev, unsigned int firstminor, unsigned int count, char ８name);
在上面这个函数中,dev是仅用于输出的参数，在成功完成调用后将保存已分配范围第一个编号
firstminor:		应该时要使用被请求的第一个次设备号,它通常时０
count,name参数：	与register_chrdev_region函数是一样的。

释放设备号使用以下函数：
	void unregister_chrdev_region(dev_t first, unsigned int count);
通常，只要在模块的清楚函数中调用　unregister_chrdev_region　函数

// 注：　驱动程序应该始终使用动态分配 alloc_chrdev_region而不是register_chrdev_region函数
动态分配的缺点：　由于分配的主设备号不能保证始终一致，所以无法预先创建设备节点，对于驱动程序的一般用法，这倒不是什么问题，因为
一旦分配了设备号，就可以从/proc/devices中读取到

