#pragma once
#include"os.h"
#include<iostream>
#include<stdio.h>
#include<time.h>
#include<string.h>

#define BLOCK_SIZE 512	//һ�����С 512 Byte
#define INODE_SIZE 128  //һ��inode entry�Ĵ�С��128Byte
#define DirItem_Size 16 //һ���������װ16��DirItem
#define FILE_NAME_MAX_SIZE	28	//�ļ����28Byte
#define Operation_Num 20		//�����20�β���
#define Backup_Block_Num 43		//����һ��backup�ṹ��ռ��43��block

#define BACKUP_SYS_NAME "backup_sys.sys"	//����ϵͳ��

struct Backup {
	char inodeBitMap[1024];		//ռ2��blcok
	char blcokBitMap[10240];	//ռ20��block
	inode childinode;
	inode parinode;
	int childinodeAddr;
	int parinodeAddr;
	char childblocks[BLOCK_SIZE * 10];
	char parblocks[BLOCK_SIZE * 10];
};

extern FILE* bfw;							//�����ļ� д�ļ�ָ��
extern FILE* bfr;							//�����ļ� ���ļ�ָ��

extern const int Start_Addr;
extern const int Backup_Cur_Addr;			//�����ļ�ϵͳ��ǰ��ַ

extern const char backup_buf[500000];

void initial();
bool backup(int count,int parAddr,int childAddr);
bool recovery(int count);