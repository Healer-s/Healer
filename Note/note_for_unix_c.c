文件测试参数
运算符
-d		测试文件是否为目录类型
-e		测试文件是否存在
-f		判断是否为一般文件
-r		测试当前用户是否有权限读取
-w		测试当前用户是否有权限写入
-x		测试当前用户是否有权限执行
整数比较运算符
-eq		是否等于
-ne		是否不等于
-gt		是否大于
-lt		是否小于
-le		是否等于或小于
-ge		是否大于或等于

字符串比较运算符
=		比较字符串内容是否相同
!=		比较字符串内容是否不同
-z		判断字符串内容是否为空

ping的参数：
ping -c 3  -i	0.2	-w	3	&> /dev/null
-c		来固定ping尝试的次数
-i		定义每个数据包的发送间隔
-w		定义等待超时时间

read： 用来读取用户输入信息的命令。
-p 		：向用户显示一段提示信息。

控制语句
if [];then
elif [];then
else 
fi

循坏语句
for  变量名 in  取值列表
do
	命令语句
done 


echo "$PASSWD" | passwd --stdin $UNAME
将PASSWD的值设置成$UNAME用户的密码
--stdin重定向为标准输入

/************************************
				C/C++
************************************/

C++：
Myclass A , B;
Myclass C = B;  // 此处是浅拷贝，不会通过重载操作符来进行赋值，只会调用 Myclass(const Myclass &x)进行浅拷贝
/*
 * Myclass C;
 * C=B;  // 此处会调用重载操作符函数来进行的赋值，是属于深拷贝， 
 * 深拷贝：
 * Myclass &operator=(const Myclass &x)
 * {
 * 	 this.成员变量 =  x.成员变量
 *    ......
 * }
 * */
 
  注 ： 在C/C++中, 局部变量按照存储形式可分为三种 auto, static, register
register 修饰的变量可能不放在内存中，所以不能使用 取地址符（&）；
只有局部自动变量和形式参数可以作为 寄存器变量 ，其它（如全局变量）不行。
局部静态变量不能定义为寄存器变量。不能写成：register static int a, b, c;

static 用法小结 
(1)局部静态变量
	只有第一次调用的时候进行初始化，第二次调用不再初始化。
	主要是延长变量的生命周期，函数执行完，变量还在，直到main函数结束
	注意事项： 切记在多线程调用有带有 static 修饰的局部变量时，要注意变量的值会被其中一个线程覆盖
(2)外部静态变量/函数
	在C中static有了第二种含义：用来表示不能被其它文件访问的全局变量和函数。, 但为了限制全局变量/函数的作用域, 函数或变量前加static使得函数成为静态函数。
	只能被当前包含的文件进行访问该函数该变量，哪怕其他文件里有一样的函数名，也只访问当前文件的函数
	主要作用，放置外部文件进行 extern 引用当前文件中的 函数/变量
(3)静态数据成员/成员函数 (C++特有)
	表示属于一个类而不是属于此类的任何特定对象的变量和函数. （ 变量不是任何实例对象的一部分，而是该类的一部分，改它所有的实例都会被改变 ）
	static成员函数调用时, 不需要 this 指针，而成员函数变量都需要 this 指针	

注：	在设计类的多线程操作时, 由于POSIX库下的线程函数pthread_create()要求是全局的, 普通成员函数无法直接做为线程函数,
	可以考虑用Static成员函数做线程函数.
/*
 * 
 *    const 关键字使用详解
 * 
 * 
 * 
 */

1. 被const修饰过的变量不能被修改，故此具有常量之称
const  int MAX = 100； // 之后不能再次被赋值，赋值就会提示语法错误

const可以修饰函数的返回值，参数以及函数的定义体，被const修饰会受到强制的保护，能防止意外的修改，从而提高函数的健壮性。
1. 用 const 修饰过的形参不能被重新赋值，只能读取。
2. 用 const 修饰过的返回值以及变量都只能做右值，不能做左值做赋值操作
3. 用 const 修饰的函数定义体的函数能被 const 或者非 const 对象调用，但是 const 对象只能调用被 const 修饰过定义体的函数。

例子：
const int n=5;
int const n=5;
以上两项意思时一致的， n 的值不能被改变



二、常量指针与指针常量

常量指针是指针指向的内容是常量，可以有一下两种定义方式。

const int * n;
int const * n;

需要注意的是一下两点：

1、常量指针说的是不能通过这个指针改变变量的值，但是还是可以通过其他的引用来改变变量的值的。

int a=5;
const int* n=&a;
a=6;

2、常量指针指向的值不能改变，但是这并不是意味着指针本身不能改变，常量指针可以指向其他的地址。

int a=5;
int b=6;
const int* n=&a;
n=&b;
总结： 常量指针，不能已指针的形式去改指针变量指向的值进行更改
// =======================================================
指针常量是指指针本身是个常量，不能在指向其他的地址，写法如下：

int *const n;

需要注意的是，指针常量指向的地址不能改变，但是地址中保存的数值是可以改变的，可以通过其他指向改地址的指针来修改。

int a=5;
int *p=&a;
int* const n=&a;
*p=8;

区分常量指针和指针常量的关键就在于星号的位置，我们以星号为分界线，如果const在星号的左边，则为常量指针，如果const在星号的右边则为指针常量。如果我们将星号读作‘指针’，将const读作‘常量’的话，内容正好符合。int const * n；是常量指针，int *const n；是指针常量。

指向常量的常指针

是以上两种的结合，指针指向的位置不能改变并且也不能通过这个指针改变变量的值，但是依然可以通过其他的普通指针改变变量的值。

const int* const p;

// =============================================

四、修饰函数的返回值

如果给以“指针传递”方式的函数返回值加 const 修饰，那么函数返回值（即指针）的内容不能被修改，该  [返回值只能被赋给加const 修饰的同类型指针] 
例：
	const char * GetString(void);
	char *str = GetString();  // Error
	const char *str = GetString(); // No Error

五、修饰全局变量

全局变量的作用域是整个文件，我们应该尽量避免使用全局变量，以为一旦有一个函数改变了全局变量的值，它也会影响到其他引用这个变量的函数，
导致除了bug后很难发现，如果一定要用全局变量，我们应该尽量的使用const修饰符进行修饰，这样方式不必要的以为修改，使用的方法与局部变量是相同的。

作用及功能：
1. 便于进行类型检查
	const常量有数据类型，用于修饰形参 void func(const in i){...}
可以对调用该函数的传入的值进行类型检查， 不匹配时候会报错。修饰形参也可以保证传入的值不会被修改

2. 在 C++ 里可以多重载一个 函数体为const修饰的重载函数
3. 可以节省控件，避免不必要的内存分配。 #define 定义的常量会在每一次调用的时候进行内存分配，
而 const 修饰的只分配一次内存控件。
4. 提高了效率
编译器通常不为普通的const常量分配存储控件，而是将它们保存在符号表中，这使得它成为一个编译期间的常量，
没有了存储与读内存的操作。使得它的效率也很高



/*
 * 		 typedef 取别名
 */ 
1. 定义一种类型的别名，而不只是简单的宏替换。可以用作同时声明指针型的多个对象。
例： 
	typedef char* PCHAR; // 一般用大写
	PCHAR pa, pb; // 可行，同时声明了两个指向字符变量的指针
2. 用在旧的C的代码中（具体多旧没有查），帮助struct。以前的代码中，声明struct新对象时，必须要带上struct，即形式为： struct 结构名 对象名
typedef struct tagPOINT  
{  
    int x;  
    int y;  
}POINT;  
	POINT p1; // 这样就比原来的方式少写了一个struct，比较省事，尤其在大量使用的时候 
3. 用typedef来定义与平台无关的类型。
typedef double REAL; 

4. 为复杂的声明定义一个新的简单的别名。
原声明：int *(*a[5])(int, char*);

变量名为a，直接用一个新别名pFun替换a就可以了：

typedef int *(*pFun)(int, char*); 

原声明的最简化版：

pFun a[5];


2. 原声明：void (*b[10]) (void (*)());
变量名为b，先替换右边部分括号里的，pFunParam为别名一：

typedef void (*pFunParam)();

再替换左边的变量b，pFunx为别名二：

typedef void (*pFunx)(pFunParam);

原声明的最简化版：

pFunx b[10];


3. 原声明：doube(*)() (*e)[9]; 
变量名为e，先替换左边部分，pFuny为别名一：

typedef double(*pFuny)();

再替换右边的变量e，pFunParamy为别名二

typedef pFuny (*pFunParamy)[9];

原声明的最简化版：

pFunParamy e;


记住2个模式：
　　type (*)(....)函数指针 
  type (*)[]数组指针
  
2.两大陷阱
陷阱一：

记住，typedef是定义了一种类型的新别名，不同于宏，它不是简单的字符串替换。比如：
先定义：

typedef char* PSTR;

然后：

int mystrcmp(const PSTR, const PSTR);

const PSTR实际上相当于const char*吗？不是的，它实际上相当于char* const。原因在于const给予了整个指针本身以常量性，也就是形成了常量指针char* const。

简单来说，记住当const和typedef一起出现时，typedef不会是简单的字符串替换就行。
陷阱二：

typedef在语法上是一个存储类的关键字（如auto、extern、mutable、static、register等一样），虽然它并不真正影响对象的存储特性，如：

typedef static int INT2; //不可行

编译将失败，会提示“指定了一个以上的存储类”。

/*
 * 
 * 	typedef 与 #define的区别
 * 
 */
1. #define是预处理指令，在编译预处理时进行简单的替换，不作正确性检查，不关含义是否正确照样带入，只有在编译已被展开的源程序时才会发现可能的错误并报错。
typedef char *pStr1;  
#define pStr2 char *;  
pStr1 s1, s2;    ---> char * s1, char * s2;
pStr2 s3, s4;    ---> char * s3, s4;
注：在上述的变量定义中，s1、s2、s3都被定义为char *，而s4则定义成了char，不是我们所预期的指针变量，根本原因就在于#define只是简单的字符串替换而typedef则是为一个类型起新名字。


【强调】
const char* p1 = "abcd";
char *const p2 = "adbc";
p1++;  // 不会报错
p2++;  // 此处报错，const修饰了p2　p2的值不能修改


强大的typedef
typedef int (*PF) (const char *, const char *);
| 　　	| 		 |
v  		v  		 v
PF Register(PF pf);
| 　转　	| 	换	 |
v  		v  		 v
int (*Register (int (*pf)(const char *, const char *))) (const char *, const char *); 
注：　此项有用于　【Linux函数】Signal ()函数　中使用　



typedef register int FAST_COUNTER; // 错误

编译通不过。问题出在你不能在声明中有多个存储类关键字。因为符号 typedef 已经占据了存储类关键字的位置，在 typedef 声明中不能用 register（或任何其它存储类关键字）


/*
 * 		volatile 关键字使用
 * 
 */
 volatile提醒编译器它后面所定义的变量随时都有可能改变，因此编译后的程序每次需要存储或读取这个变量的时候，都会直接从变量地址中读取数据。
volatile　主要用于与寄存器的操作，多线程操作时，多个线程会导致某个变量会随时发生改变的时候，需要使用volatile进行修饰。

1、中断服务程序中修改的供其它程序检测的变量需要加volatile；
2、多任务环境下各任务间共享的标志应该加volatile；
3、存储器映射的硬件寄存器通常也要加volatile说明，因为每次对它的读写都可能由不同意义；

注：
1.中可以通过关中断来实现；
2.中可以禁止任务调度;
3.中则只能依靠硬件的良好设计了;


二、volatile 的含义
	volatile总是与优化有关，编译器有一种技术叫做【数据流分析】，分析程序中的变量在哪里赋值、在哪里使用、在哪里失效，分析结果可以用于常量合并，
常量传播等优化，进一步可以死代码消除。但有时这些优化不是程序所需要的，这时可以用volatile关键字禁止做这些优化，volatile的字面含义是易变的
作用：
1.	不会在两个操作之间把volatile变量缓存在寄存器中。在多任务、中断、甚至setjmp环境下，
变量可能被其他的程序改变，编译器自己无法知道，volatile就是告诉编译器这种情况。
2.　　不做常亮合并，常量传播等优化，所以像下面的代码： 
volatile int i = 1;
if (i > 0) ...
if的条件不会当作无条件真
3. 对volatile变量的读写不会被优化掉。如果你对一个变量赋值但后面没用到，编译器常常可以省略那个赋值操作，
然而对Memory Mapped IO的处理是不能这样优化的。 
