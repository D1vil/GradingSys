#pragma once
#include<iostream>
#include<stdio.h>
#include<time.h>
#include<string.h>


#define BLOCK_SIZE 512	//һ�����С 512 Byte
#define INODE_SIZE 128  //һ��inode entry�Ĵ�С��128Byte
#define DirItem_Size 16 //һ���������װ16��DirItem
#define FILE_NAME_MAX_SIZE	32	//�ļ����32Byte

#define BLOCK_NUM 10240		//10240��block
#define INODE_NUM 1024	//һ��inode���Դ�10��block->Ҫ��1024��inode����Ϣ

#define MODE_DIR 01000	//Ŀ¼��ʶ���˽��ƣ�: 001 000 000 000
#define MODE_FILE 00000	//�ļ���ʶ���˽��ƣ�: 000 000 000 000

#define OWNER_R	4<<6						//���û���Ȩ��
#define OWNER_W	2<<6						//���û�дȨ��
#define OWNER_X	1<<6						//���û�ִ��Ȩ��
#define GROUP_R	4<<3						//���û���Ȩ��
#define GROUP_W	2<<3						//���û�дȨ��
#define GROUP_X	1<<3						//���û�ִ��Ȩ��
#define OTHERS_R	4						//�����û���Ȩ��
#define OTHERS_W	2						//�����û�дȨ��
#define OTHERS_X	1						//�����û�ִ��Ȩ��
#define FILE_DEF_PERMISSION 0664			//�ļ�Ĭ��Ȩ�� owner,group:��д other:�� 
#define DIR_DEF_PERMISSION	0755			//Ŀ¼Ĭ��Ȩ�� owner����дִ�� group,other:��

#define ROOT 0	  //����Ա
#define TEACHER 1 //��ʦ
#define STUDENT 2 //ѧ��

#define GRADE_SYS_NAME "grading_sys.sys"	//�ļ�ϵͳ��

struct SuperBlock {
	unsigned short s_INODE_NUM;				//inode�ڵ�������� 65535
	unsigned int s_BLOCK_NUM;				//���̿��������� 4294967294

	unsigned short s_free_INODE_NUM;		//����inode�ڵ���
	unsigned int s_free_BLOCK_NUM;			//���д��̿���

	unsigned short s_BLOCK_SIZE;			//���̿��С
	unsigned short s_INODE_SIZE;			//inode��С
	unsigned short s_SUPERBLOCK_SIZE;		//�������С
	
	//���̷ֲ�
	int s_Superblock_Start_Addr;
	int s_InodeBitmap_Start_Addr;
	int s_BlockBitmap_Start_Addr;
	int s_Inode_Start_Addr;
	int s_Block_Start_Addr;
};

struct inode {
	unsigned short inode_id;					//inode��ʶ����ţ�
	unsigned short inode_mode;					//��ȡȨ��:r--��ȡ��w--д��x--ִ��
	unsigned short inode_file_count;				//�ļ������ж����ļ�
	//unsigned short i_uid;					//�ļ������û�id
	//unsigned short i_gid;					//�ļ������û���id
	char i_uname[20];						//�ļ������û�
	char i_gname[20];						//�ļ������û���
	unsigned int inode_file_size;					//�ļ���С�Ƕ���Byte���ļ���Byte Ŀ¼��block��
	time_t  inode_change_time;						//inode��һ�α䶯��ʱ��
	time_t  file_change_time;						//�ļ�������һ�α䶯��ʱ��
	time_t  file_modified_time;						//�ļ���һ���޸ĵ�ʱ��
	int i_dirBlock[10];						//10��ֱ�ӿ飺�ܹ��ܴ洢�Ĵ�С��10*512B = 5120B = 5KB
};

//�ļ�Ŀ¼
struct DirItem {							//һ��item��32Byte��һ��block����װ16��item
	char itemName[FILE_NAME_MAX_SIZE];			//Ŀ¼�����ļ���
	int inodeAddr;							//Ŀ¼���Ӧ��inode�ڵ��ַ
};

extern SuperBlock* superblock;
extern const int Superblock_Start_Addr;		//������ƫ�Ƶ�ַ,ռһ��block
extern const int InodeBitmap_Start_Addr;		//inodeλͼ ƫ�Ƶ�ַ��ռ�������̿飬�����1024��inode��״̬
extern const int BlockBitmap_Start_Addr;		//blockλͼ ƫ�Ƶ�ַ��ռ��ʮ�����̿飬����� 10240 �����̿飨5120KB����״̬
extern const int Inode_Start_Addr;			//inode�ڵ��� ƫ�Ƶ�ַ��ռ INODE_NUM/(BLOCK_SIZE/INODE_SIZE) �����̿�
extern const int Block_Start_Addr;			//block������ ƫ�Ƶ�ַ ��ռ INODE_NUM �����̿�
extern const int File_Max_Size;				//�����ļ�����С
extern const int Disk_Size;					//��������ļ���С


//ȫ�ֱ�������
extern char Cur_Host_Name[110];				//��ǰ������
extern int Root_Dir_Addr;					//��Ŀ¼inode��ַ
extern int Cur_Dir_Addr;					//��ǰĿ¼
extern char Cur_Dir_Name[310];				//��ǰĿ¼��
extern char Cur_User_Name[110];				//��ǰ��½�û���
extern char Cur_Group_Name[110];			//��ǰ��½�û�����
extern char Cur_User_Dir_Name[310];			//��ǰ��½�û�Ŀ¼��

extern int nextUID;							//��һ��Ҫ������û���ʶ��
extern int nextGID;							//��һ��Ҫ������û����ʶ��

extern bool isLogin;						//�Ƿ����û���½

extern FILE* fw;							//��������ļ� д�ļ�ָ��
extern FILE* fr;							//��������ļ� ���ļ�ָ��
extern SuperBlock* superblock;				//������ָ��
extern bool inode_bitmap[INODE_NUM];		//inodeλͼ
extern bool block_bitmap[BLOCK_NUM];		//���̿�λͼ

extern char buffer[10000000];				//10M������������������ļ�

//��������&��ʾ����
void help();

//���ຯ��
bool Format();								//�ļ�ϵͳ��ʽ��
bool Install();								//��װ�ļ�ϵͳ
bool mkdir(int PIAddr, char name[]);
bool rmdir(int CHIAddr, char name[]);
bool mkfile(int PIAddr, char name[], char buf[]);
bool rmfile(int CHIAddr, char name[]);
void writefile(inode fileinode, int iaddr, char buf[]);
void cd(int PIAddr, char name[]);
void gotoRoot();
void ls();

//���ߺ���
int ialloc();
void ifree(int iaddr);
int balloc();
void bfree(int baddr);

//�û�&�û��麯��
void inUsername(char* username);								//�����û���
void inPasswd(char* passwd);
bool login();
bool logout();
bool useradd(char username[], char passwd[], char group[]);
bool userdel(char username[]);
bool check(char username[], char passwd[]);
void chmod(int PIAddr, char name[], int pmode);