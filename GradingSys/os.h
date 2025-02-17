#pragma once
#include<iostream>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include"server.h"
#include"function.h"


#define BLOCK_SIZE 512	//一个块大小 512 Byte
#define INODE_SIZE 128  //一个inode entry的大小是128Byte
#define DirItem_Size 16 //一个块最多能装16个DirItem
#define FILE_NAME_MAX_SIZE	28	//文件名最长28Byte

#define BLOCK_NUM 10240		//10240个block
#define INODE_NUM 1024	//一个inode可以存10个block->要用1024个inode存信息

#define MODE_DIR 01000	//目录标识（八进制）: 001 000 000 000
#define MODE_FILE 00000	//文件标识（八进制）: 000 000 000 000

#define OWNER_R	4<<6						//本用户读权限
#define OWNER_W	2<<6						//本用户写权限
#define OWNER_X	1<<6						//本用户执行权限
#define GROUP_R	4<<3						//组用户读权限
#define GROUP_W	2<<3						//组用户写权限
#define GROUP_X	1<<3						//组用户执行权限
#define OTHERS_R	4						//其它用户读权限
#define OTHERS_W	2						//其它用户写权限
#define OTHERS_X	1						//其它用户执行权限
#define FILE_DEF_PERMISSION 0664			//文件默认权限 owner,group:读写 other:读 
#define DIR_DEF_PERMISSION	0755			//目录默认权限 owner：全部 group,other:读执行

#define GRADE_SYS_NAME "grading_sys.sys"	//文件系统名
#define STUDENT_COURSE_LIST "Student_Course_Selection.txt" //学生选课名单（name:lesson:teacher)


struct SuperBlock {
	unsigned short s_INODE_NUM;				//inode节点数，最多 65535
	unsigned int s_BLOCK_NUM;				//磁盘块块数，最多 4294967294

	unsigned short s_free_INODE_NUM;		//空闲inode节点数
	unsigned int s_free_BLOCK_NUM;			//空闲磁盘块数

	unsigned short s_BLOCK_SIZE;			//磁盘块大小
	unsigned short s_INODE_SIZE;			//inode大小
	unsigned short s_SUPERBLOCK_SIZE;		//超级块大小
	
	//磁盘分布
	int s_Superblock_Start_Addr;
	int s_InodeBitmap_Start_Addr;
	int s_BlockBitmap_Start_Addr;
	int s_Inode_Start_Addr;
	int s_Block_Start_Addr;
};

struct inode {		//不要动此处变量，刚好128B
	unsigned short inode_id;					//inode标识（编号）
	unsigned short inode_mode;					//存取权限:r--读取，w--写，x--执行
	unsigned short inode_file_count;				//文件夹里有多少文件
	//unsigned short i_uid;					//文件所属用户id
	//unsigned short i_gid;					//文件所属用户组id
	char i_uname[18];						//文件所属用户
	char i_gname[18];						//文件所属用户组
	unsigned int inode_file_size;					//文件大小是多少Byte（文件：Byte 目录：block）
	time_t  inode_change_time;						//inode上一次变动的时间
	time_t  dir_change_time;						//文件内容上一次变动的时间(针对dir)
	time_t  file_modified_time;						//文件上一次修改的时间(针对file)
	int i_dirBlock[10];						//10个直接块：总共能存储的大小是10*512B = 5120B = 5KB
	int i_indirect_1;						//一级间接块
	int i_indirect_2;						//二级间接块
	bool snapshot;							//ture:是挂载的； false：非挂载
};

//文件目录
struct DirItem {							//一个item是32Byte，一个block可以装16个item
	char itemName[FILE_NAME_MAX_SIZE];			//目录或者文件名
	int inodeAddr;							//目录项对应的inode节点地址
};

extern SuperBlock* superblock;
extern const int Superblock_Start_Addr;		//超级块偏移地址,占一个block
extern const int InodeBitmap_Start_Addr;		//inode位图 偏移地址，占两个磁盘块，最多监控1024个inode的状态
extern const int BlockBitmap_Start_Addr;		//block位图 偏移地址，占二十个磁盘块，最多监控 10240 个磁盘块（5120KB）的状态
extern const int Inode_Start_Addr;			//inode节点区 偏移地址，占 INODE_NUM/(BLOCK_SIZE/INODE_SIZE) 个磁盘块
extern const int Block_Start_Addr;			//block数据区 偏移地址 ，占 INODE_NUM 个磁盘块
extern const int Modified_inodeBitmap_Start_Addr;		//逻辑转储
extern const int File_Max_Size;				//单个文件最大大小
extern const int Disk_Size;					//虚拟磁盘文件大小


//全局变量声明
extern char Cur_Host_Name[110];				//当前主机名
extern int Root_Dir_Addr;					//根目录inode地址

extern int nextUID;							//下一个要分配的用户标识号
extern int nextGID;							//下一个要分配的用户组标识号

extern bool isLogin;						//是否有用户登陆

extern FILE* fw;							//虚拟磁盘文件 写文件指针
extern FILE* fr;							//虚拟磁盘文件 读文件指针
extern SuperBlock* superblock;				//超级块指针
extern bool inode_bitmap[INODE_NUM];		//inode位图
extern bool block_bitmap[BLOCK_NUM];		//磁盘块位图
extern bool modified_inode_bitmap[INODE_NUM];

extern char buffer[10000000];				//10M，缓存整个虚拟磁盘文件
extern Client sys;							//系统初始化用对象
extern std::vector<Client> allClients;      //整个系统登录的用户




//大类函数
bool Format();								//文件系统格式化
bool Install();								//安装文件系统

bool mkdir(Client&, int PIAddr, char name[]);
bool mkfile(Client&, int PIAddr, char name[], char buf[]);
bool rm(Client&, int PIAddr, char name[],int type);
bool addfile(Client&, inode fileinode, int CHIaddr, char buf[]);
bool writefile(inode fileinode, int CHIaddr, char buf[]);
bool echo(Client&, int PIAddr, char name[], int type, char* buf);
bool chmod(Client&, int PIAddr, char name[], char* pmode);
bool cd(Client&, int PIAddr, char str[]);
void gotoRoot(Client&);
void ls(Client&, char str[]);

bool cat(Client&, int PIAddr, char name[]);
bool chown(Client&, int PIAddr, char* pmode, char name[], char group[]);

//工具函数
int ialloc();
void ifree(int iaddr);
int balloc();
void bfree(int baddr);
void safeFseek(FILE* file, long offset, int origin);
size_t safeFwrite(const void* ptr, size_t size, size_t count, FILE* file);
bool recursive_rmdir(Client&, int CHIAddr, char name[]);
bool recursive_rmfile(Client&, int CHIAddr, char name[]);

//用户&用户组函数
void inUsername(Client&, char* username);							//输入用户名
void inPasswd(Client&, char* passwd);
void ingroup(Client&, char* group);
bool login(Client&);
bool logout(Client&);
bool useradd(Client&, char username[], char passwd[], char group[]);
bool userdel(Client&, char username[]);
bool check(Client&, char username[], char passwd[]);	//账号&密码
bool groupadd(Client&, char* group);
bool groupdel(Client&, char* group);
bool check(Client&, char username[], char passwd[]);	//账号&密码
bool passwd(Client&, char username[], char pwd[]);
char* is_group(Client&, char* group,char *gid);//判断是否在组内
//bool check_group(char name[], char s_group[]);//账号&组别
bool ever_logging();												//判断系统是否还有用户处于登陆状态
//bool groupadd(char gname[]);
//bool groupdel(char gname[]);
//bool passwd();


