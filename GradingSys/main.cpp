﻿#include <cstdio>
#include<cstdlib>
#include <iostream>
#include"os.h"
#include<unistd.h>
#include<limits>


const int Superblock_Start_Addr=0;     
const int InodeBitmap_Start_Addr = 1 * BLOCK_SIZE;	
const int BlockBitmap_Start_Addr = InodeBitmap_Start_Addr + 2 * BLOCK_SIZE;
const int Inode_Start_Addr = BlockBitmap_Start_Addr + 20 * BLOCK_SIZE;
const int Block_Start_Addr = Inode_Start_Addr + INODE_NUM / (BLOCK_SIZE / INODE_SIZE) * BLOCK_SIZE;

const int Disk_Size= Block_Start_Addr + BLOCK_NUM * BLOCK_SIZE;
const int File_Max_Size = 10 * BLOCK_SIZE;


int Root_Dir_Addr;							//根目录inode地址
int Cur_Dir_Addr;							//当前目录
char Cur_Dir_Name[310];						//当前目录名
char Cur_Host_Name[110];					//当前主机名
char Cur_User_Name[110];					//当前登陆用户名
char Cur_Group_Name[110];					//当前登陆用户组名
char Cur_User_Dir_Name[310];				//当前登陆用户目录名

int nextUID;								//下一个要分配的用户标识号
int nextGID;								//下一个要分配的用户组标识号

bool isLogin;								//是否有用户登陆

FILE* fw;									//虚拟磁盘文件 写文件指针
FILE* fr;									//虚拟磁盘文件 读文件指针
SuperBlock* superblock = new SuperBlock;	//超级块指针
bool inode_bitmap[INODE_NUM];				//inode位图
bool block_bitmap[BLOCK_NUM];				//磁盘块位图

char buffer[10000000] = { 0 };				//10M，缓存整个虚拟磁盘文件

using namespace std;


int main()
{
    printf("%s 向你问好!\n", "GradingSys");
    //###############打不开文件################
    if ((fr = fopen(GRADE_SYS_NAME, "rb")) == NULL) {        
        fw = fopen(GRADE_SYS_NAME, "wb");
        if (fw == NULL) {
            printf("虚拟磁盘文件打开失败！");
            return 0;
        }
        fr = fopen(GRADE_SYS_NAME, "rb");
        printf("虚拟磁盘文件打开成功！\n");
        //初始化变量
        nextUID = 0;
        nextGID = 0;
        isLogin = false;
        strcpy(Cur_User_Name, "root");
        strcpy(Cur_Group_Name, "root");
        //获取主机名
        memset(Cur_Host_Name, 0, sizeof(Cur_Host_Name));
        if (gethostname(Cur_Host_Name, sizeof(Cur_Host_Name)) != 0) {
            perror("Error getting hostname");
            return 1;
        }
        
        Root_Dir_Addr = Inode_Start_Addr;
        Cur_Dir_Addr = Root_Dir_Addr;
        strcpy(Cur_Dir_Name, "/");
        printf("文件系统正在格式化");
        
        //系统格式化
        if (!Format()) {
            printf("格式化失败");
            return 0;
        }
        printf("格式化完成");
        printf("按'Enter'键登录\n");
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        //Install

    }
    else {
        fread(buffer, Disk_Size, 1, fr);
        fw = fopen(GRADE_SYS_NAME, "wb");

        if (fw == NULL) {
            printf("磁盘文件打开失败！/n");
            return false;
        }

        fwrite(buffer, Disk_Size, 1, fw);
        nextUID = 0;
        nextGID = 0;
        isLogin = false;
        strcpy(Cur_User_Name, "root");
        strcpy(Cur_Group_Name, "root");

        //获取主机名
        memset(Cur_Host_Name, 0, sizeof(Cur_Host_Name));
        if (gethostname(Cur_Host_Name, sizeof(Cur_Host_Name)) != 0) {
            perror("Error getting hostname");
            return 1;
        }
        printf("%s\n", Cur_Host_Name);

        //获取根目录
        Root_Dir_Addr = Inode_Start_Addr;
        Cur_Dir_Addr = Root_Dir_Addr;
        strcpy(Cur_Dir_Name, "/");

        //Install
    }

    while (1) {
        if (isLogin) {
            char str[100];
            char* p;
            if ((p = strstr(Cur_Dir_Name, Cur_User_Dir_Name)) == NULL)	//没找到
                printf("[%s@%s %s]# ", Cur_Host_Name, Cur_User_Name, Cur_Dir_Name);
            else
                printf("[%s@%s ~%s]# ", Cur_Host_Name, Cur_User_Name, Cur_Dir_Name + strlen(Cur_User_Dir_Name));
            gets(str);
            //cmd(str);
            
        }
        else {
            printf("欢迎来到MingOS，请先登录\n");
            while (!login());	//登陆
            printf("登陆成功！\n");
            //system("pause");
            //system("cls");
        }
        fclose(fw);		//释放文件指针
        fclose(fr);		//释放文件指针

        return 0;
    }
    help();
    return 0;
}