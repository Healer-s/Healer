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
MAJOR(dev_t dev); //提取主设备号的方法
MINOR(dev_t dev); //提取次设备号的方法
相反，如果需要将主设备号和次设备号转换成dev_t类型，则使用:
MKDEV(int major, int minor); //major为主设备号 minor为次设备号

// 主设备号用来区分 不同种类 的设备，而次设备号用来区分 同一类型 的多个设备

1.4分配和释放设备编号
驱动程序首先要做的事情就是获得一个或者多个设备编号，完成该工作的必要函数register_chrdev_region
#include <linux/fs.h>
int register_chrdev_region (dev_t first, unsigned int count, char* name);

first:	次设备号经常被置为0
count:	是所请求的连续设备编号的个数
name:	是和该编号范围关联的设备名称，它将出现在/proc/devices和sysfs中。
返回值: 分配成功返回0，错误返回负的错误码，并且不能使用所请求的编号区域

动态分配:
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

/*   
 *		文件操作 中基本的数据结构 	
 **/
1.大部分基本的驱动程序操作涉及到三个重要的内核数据结构，分别是 file_operations, file 和 inode

1.1当获得一些设备编号，要将驱动程序操作链接到这些编号时，需要用到 file_operations 结构来建立这种连接
该结构定义在 #include <linux/fs.h>中
struct file_operations {
//1.该字段不是操作，相反，它是指向“拥有”该结构的模块的指针，内核使用这个字段可以避免在模块操作正在被使用时卸载该模块。
	struct module *owner;
// llseek 用来修改文件的当前读写位置，将新的位置作为(正的)返回值返回，loff_t是一个偏移量，即使在32位平台上也占用64位的数据宽度
// 出错返回一个负值，如果该函数指针为NULL,对seek的调用将会以某种不可语气的方式修改file结构中的位置计数器
	loff_t (*llseek) (struct file *, loff_t, int);
// 用来从设备中都去数据，该函数指针被赋为NULL时，将导致read系统调用出做，并返回 -EINVAL(Invalid argument非法参数)
// 函数返回非负数表示成功读取的字节数，（返回值：signed size 数据类型），通常就是目标平台的固有整数
	ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
//	向设备发送数据没如果没有这个函数，write系统调用会向程序返回一个 -EINVAL,如果返回值非负数，则表示成功写入的字节数
	ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
//	初始化一个一步的读取操作--即在函数返回之前可能不会完成的读取操作，如果该方法为NULL，所有操作将通过read同步处理
	ssize_t (*aio_read) (struct kiocb *, const struct iovec *, unsigned long, loff_t);
//	初始化设备上的异步写入操作
	ssize_t (*aio_write) (struct kiocb *, const struct iovec *, unsigned long, loff_t);
//	对于设备文件来说，这个字段应该为NULL，它仅用于读取目录，之对文件系统有用
	int (*readdir) (struct file *, void *, filldir_t);
//	poll方法是poll，epoll和select这个三个系统调用的后端实现，可以查询是否被阻塞，如果此处为NULL,则设备会被认为i可读可写
//	并且不会被阻塞
	unsigned int (*poll) (struct file *, struct poll_table_struct *);
//	系统调用ioctl提供了一种执行设备特定命令的方法
	long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
	long (*compat_ioctl) (struct file *, unsigned int, unsigned long);
//	用于请求将设备内存映射到进程地址空间，如果设备没有实现这个方法，那么mmap系统调用将返回 —ENODEV
	int (*mmap) (struct file *, struct vm_area_struct *);
//	尽管这个始终是对设备文件执行的第一个操作，然后却并不要求驱动一定要声明一个对应的方法，如果这个入
//	口为NULL,设备的打开操作永远成功，但是系统不会通知驱动程序
	int (*open) (struct inode *, struct file *);
//	对flush操作的调用发生在进程关闭设备文件描述符副本的适合，它应该执行设备上尚未完结的操作。
	int (*flush) (struct file *, fl_owner_t id);
//	当file结构被释放时，将调用这个操作，与open相仿，也可以将release设置为NULL
	int (*release) (struct inode *, struct file *);
//	该方法是fsync系统调用的后端实现，用户调用它来刷新待处理的数据，如果驱动程序没有实现这一方法，fsync系统调用返回-EINVAL
	int (*fsync) (struct file *, loff_t, loff_t, int datasync);
//	这是fsync方法的异步版本
	int (*aio_fsync) (struct kiocb *, int datasync);
//	这个操作用来通知设备其FASYNC标志发生了变化，异步同志是比较高级的话题，
	int (*fasync) (int, struct file *, int);
//	lock方法用于实现文件锁定，锁定是常规文件不可缺少的特性，但是设备驱动几乎从来不会实现这个方法
	int (*lock) (struct file *, int, struct file_lock *);
//	sendpage是sendfile系统调用的另外一般，它由内核调用以将数据发送到对应文件，每次一个数据页，设备驱动程序通常也不需要实现sendpage
	ssize_t (*sendpage) (struct file *, struct page *, int, size_t, loff_t *, int);
//	该方法的目的是在进程的地址空间中找到一个合适的位置，以便底层设备中的内存段映射到该位置，大部分驱动可设置该方法为NULL
	unsigned long (*get_unmapped_area)(struct file *, unsigned long, unsigned long, unsigned long, unsigned long);
//	该方法允许模块检查传递给fcntl (F_SETFL)调用的标志
	int (*check_flags)(int);
	int (*flock) (struct file *, int, struct file_lock *);
	ssize_t (*splice_write)(struct pipe_inode_info *, struct file *, loff_t *, size_t, unsigned int);
	ssize_t (*splice_read)(struct file *, loff_t *, struct pipe_inode_info *, size_t, unsigned int);
	int (*setlease)(struct file *, long, struct file_lock **);
	long (*fallocate)(struct file *file, int mode, loff_t offset,
			  loff_t len);
};
这里里面包含了一组函数指针，每个打开的文件，在内部由一个file结构表示，和一组函数关联，通过包含指向一个file_operations
结构的f_op字段，这些操作主要用来实现系统调用，命名为open ,read 等。我么么可以认为文件是一个“对象”，而操作它的函数是“方法”。
如果采用面对对象变成的术语来表达就是，对象声明的动作将作用于其本身。

1.设备驱动程序所实现的最重要的设备方法，它的file_operations结构被初始化成如下形式：
static const struct file_operations __fops = {				\
	.owner	 = THIS_MODULE,						\
	.open	 = scull_open,					\
	.release = scull_release,					\
	.read	 = scull_read,					\
	.write	 = scull_write,					\
	.llseek	 = sculle_llseek,					\
	.unlocked_ioctl = sculle_ioctl,				\
};
以上声明采用了标准C的标记化结构初始化语法，这种语法是值得被采用的，因为它是驱动程序在结构的定义发生变化时更具有可移植性


/*
 * 
 * 				file 结构
 * 
 */
在<linux/fs.h>中定义的struct file是设备驱动程序所使用的第二个最重要的数据结构。
注： file 结构与用户空间程序中的 FILE 没有任何关联. 互不涉及。

在内核源码中，指向 struct file 的指针通常被称为 file 或 filp（文件指针）
file 指的是结构本身， filp 则是指向该结构的指针
重要成员：
mode_t f_mode:	文件模式，它通过FMODE_READ和FMODE_WRITE位来表示文件是否可读或可写
注：	一般会认为自己需要在 open/ioctl 函数中查看该字段，以便查看是否有读写访问权限，驱动程序不需要做这个操作，如果
没有对文件的读写操作权限，会被内核直接拒绝。

loff_t	f_pos:		当前的读写位置，loff_t是一个64位的数，如果需要知道文件的当前位置，可以读取该值

unsigned int f_flags	:文件标志，如：O_RDONLY,O_NONBLOCK和O_SYNC,用来检查用户请求的是否非阻塞式的操作
驱动程序一般只用得到O_NONBLOCK标志，,检查读写权限应查看 f_mode/f_flags,定义在<linux/fcntl.h>

struct file_operations *f_op:
	与文件相关的操作，内核在执行open操作时对这个指针赋值，以后需要处理这写操作时就读取该指针，

void *private_data	:open系统调用在调用驱动程序的open方法前将这个指针置为NULL
struct dentry *f_dentry:		文件对应的目录项结构.设备驱动程序一般不需要此结构

/*
 * 
 * 			Inode结构
 * 
 */
1.内核用inode结构在内部表示文件，因此它和file结构不同，后在表示打开的文件符
对单个文件，可能会有许多表示打开的文件妙舒服的file结构，但她们都指向单个inode结构

inode 结构中包含了大量有关文件的信息，作为常规，只有下面两个字段对编写驱动程序代码有用：

dev_t i_rdev:	对表示设备文件的inode结构，该字段包含了真正的设备编号

struct cdev *i_cdev:
struct cdev 是表示字符设备的内核的内部结构，当inode指向一个字符设备文件时，该字段包含了指向struct cdev结构的指针

i_rdev的类型在2.5开发系列版本中发生了变化，这破坏了大量驱动程序代码的兼容性，为了鼓励编写可移植性更强的代码，内核
开发者增加了两个新的宏，可以用 inode 中获得 .0主设备号和次设备号
unsigned int iminor (struct inode *inode);
unsigned int imajor (struct inode *inode);

为了防止因为类似的改变而出现问题，我们应该使用上述宏，而不是直接操作i_rdev;

static inline unsigned iminor(const struct inode *inode)
{
	return MINOR(inode->i_rdev);
}
MAJOR(dev_t dev); //提取主设备号的方法
MINOR(dev_t dev); //提取次设备号的方法
相反，如果需要将主设备号和次设备号转换成dev_t类型，则使用:
MKDEV(int major, int minor); //major为主设备号 minor为次设备号
static inline unsigned imajor(const struct inode *inode)
{
	return MAJOR(inode->i_rdev);
}

/**
 * 
 * 			字符设备的注册
 * 
 */
1.内核内部使用struct cdev 结构来表示字符设备，在内核调用设备的操作之前，必须分配并注册一个或者
多个上述结构，因此需要 #include<linux/cdev.h>

	struct cdev *my_cdev = cdev_alloc();
	my_cdev->ops	= &my_fops;
现在可以将cdev结构嵌入到自己的设备特定结构中， scull就是这样做的，这种情况下，我们需要用下面的
代码来初始化已分配到的结构：
void cdev_init （ struct cdev *cdev, struct file_operations *fops);
{	// 会将cdev里的ops成员与传进入的 fops成员进行帮定
   memset(cdev, 0, sizeof *cdev);
   INIT_LIST_HEAD(&cdev->list);
   kobject_init(&cdev->kobj, &ktype_cdev_default);
   cdev->ops = fops;
}
另外一个 struct cdev 的字段需要初始化，和file_operations 结构类似， struct cdev也有一个
所有字段，应被设置为 THIS_MODULE.
	
在 cdev 结构设置好之后，最后的步骤是通过下面的调用告诉内核该结构的信息：
	int cdev_add ( struct cdev *cdev, dev_t num, unsigned int count);
这里, dev是cdev结构， num是该设备对应的第一个设备编号，count是应该和该设备关联的设备编号的数量，
count经常取1， 
	注： add 设备时可能失败，

要从系统中移除一个字符设备,调用
	void cdev_del(struct cdev *dev);
	
	
struct cdev {
   struct kobject kobj;          // 每个 cdev 都是一个 kobject
   struct module *owner;       // 指向实现驱动的模块
   const struct file_operations *ops;   // 操纵这个字符设备文件的方法
   struct list_head list;       // 与 cdev 对应的字符设备文件的 inode->i_devices 的链表头
   dev_t dev;                   // 起始设备编号
   unsigned int count;       // 设备范围号大小
};

一个 cdev 一般它有两种定义初始化方式：静态的和动态的。
静态内存定义初始化：
struct cdev my_cdev;
cdev_init(&my_cdev, &fops);
my_cdev.owner = THIS_MODULE;

动态内存定义初始化：
struct cdev *my_cdev = cdev_alloc();
my_cdev->ops = &fops;
my_cdev->owner = THIS_MODULE;

/*
	* 设备驱动程序通过调用cdev_add把它所管理的设备对象的指针嵌入到一个类型为struct probe的节点之中，
	* 然后再把该节点加入到cdev_map所实现的哈希链表中。
	* 当设备驱动程序成功调用了cdev_add之后，就意味着一个字符设备对象已经加入到了系统，在需要的时候，
	* 系统就可以找到它。对用户态的程序而言，cdev_add调用之后，就已经可以通过文件系统的接口呼叫到我们的驱动程序。
*/
void cdev_init(struct cdev *cdev, const struct file_operations *fops)
{
   memset(cdev, 0, sizeof *cdev);
   INIT_LIST_HEAD(&cdev->list);
   kobject_init(&cdev->kobj, &ktype_cdev_default);
   cdev->ops = fops;
}

int cdev_add(struct cdev *p, dev_t dev, unsigned count)
{
   p->dev = dev;
   p->count = count;
   return kobj_map(cdev_map, dev, count, NULL, exact_match, exact_lock, p);
}

 //当一个字符设备驱动不再需要的时候（比如模块卸载），就可以用 cdev_del() 函数来释放 cdev 占用的内存。
void cdev_del(struct cdev *p)
{
   cdev_unmap(p->dev, p->count);
   kobject_put(&p->kobj);
}

注册一个 字符设备驱动程序的经典方式

