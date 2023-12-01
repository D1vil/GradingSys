#include"os.h"
#include<iomanip>
#include<time.h>
#include<string.h>
#include<stdio.h>

using namespace std;

void help() {
	
	cout.setf(ios::left); //设置对齐方式为left 
	cout.width(30); //设置宽度，不足用空格填充 
	//cout << setiosflags(ios::left);
	cout << "ls" << "Display the current directory listing" << endl;	//列出当前目录清单
	cout.width(30);
	cout << "cd" << "Enter the specific directory " << endl;		//前往指定目录
	cout.width(30);
	cout << "mkdir" << "Create directory" << endl;					//创建目录
	cout.width(30);
	cout << "rm" << "Delete the file or directory" << endl;			//删除文件和目录 
	cout.width(30);
	cout << "touch" << "Create new file" << endl;				//创建新文件
	cout.width(30);
	cout << "read" << "Read the content of file" << endl;		//读文件
	cout.width(30);
	cout << "write" << "Write the file" << endl;			//写文件
	cout.width(30);
	cout << "chmod" << "Modify the access right" << endl;		//修改文件权限
	cout.width(30);
	cout << "adduser" << "Add user" << endl;		//新增用户
	cout.width(30);
	cout << "deluser" << "Delete user" << endl;		//删除用户
	cout.width(30);
	cout << "addusergrp" << "Add user group" << endl;		//新增用户组
	cout.width(30);
	cout << "delusergrp" << "Delete user group" << endl;		//删除用户组
	cout.width(30);
	cout << "snapshot" << "Back up the system" << endl;			//备份系统
	cout.width(30);
	cout << "recover" << "Recover the system" << endl;			//恢复系统
}

bool Format() {
	//初始化:超级块,位图
	superblock->s_INODE_NUM = INODE_NUM;
	superblock->s_BLOCK_NUM = BLOCK_NUM;
	superblock->s_free_INODE_NUM = INODE_NUM;
	superblock->s_free_BLOCK_NUM = BLOCK_NUM;
	superblock->s_BLOCK_SIZE = BLOCK_SIZE;
	superblock->s_INODE_SIZE = INODE_SIZE;
	superblock->s_SUPERBLOCK_SIZE = sizeof(SuperBlock);
	superblock->s_Superblock_Start_Addr = Superblock_Start_Addr;
	superblock->s_InodeBitmap_Start_Addr = InodeBitmap_Start_Addr;
	superblock->s_Inode_Start_Addr = Inode_Start_Addr;
	superblock->s_Block_Start_Addr = Block_Start_Addr;
	fseek(fw, Superblock_Start_Addr, SEEK_SET);
	fwrite(superblock, sizeof(SuperBlock), 1, fw);

	memset(inode_bitmap, 0, sizeof(inode_bitmap));
	fseek(fw, InodeBitmap_Start_Addr, SEEK_SET);
	fwrite(inode_bitmap, sizeof(inode_bitmap), 1, fw);

	memset(block_bitmap, 0, sizeof(block_bitmap));
	fseek(fw, BlockBitmap_Start_Addr, SEEK_SET);
	fwrite(block_bitmap, sizeof(block_bitmap), 1, fw);

	//inode和block板块暂时不需要内容
	fflush(fw);//将上面内容放入fw中

	//创建根目录
	int iaddr = ialloc();
	int baddr = balloc();
	if (iaddr == -1 || baddr==-1) {
		return false;
	}

	inode ino;
	ino.inode_id = iaddr;
	ino.inode_mode = MODE_DIR | DIR_DEF_PERMISSION;
	ino.inode_file_count = 1; //"."也算
	strcpy(ino.i_uname , "root");
	strcpy(ino.i_gname , "root");
	ino.inode_file_size = 1 * BLOCK_SIZE;
	time(&ino.inode_change_time);
	time(&ino.file_change_time);
	time(&ino.file_modified_time);
	// printf("%s", asctime(localtime(&time)));转成合适格式
	memset(ino.i_dirBlock, -1, 10);
	ino.i_dirBlock[0] = baddr;
	fseek(fw, iaddr, SEEK_SET);
	fwrite(&ino, sizeof(inode), 1, fw);

	DirItem ditem[DirItem_Size];
	ditem[0].inodeAddr = iaddr;
	strcpy(ditem[0].itemName , ".");
	fseek(fw, baddr, SEEK_SET);
	fwrite(ditem, sizeof(ditem), 1, fw);

	fflush(fw);
	
	//创建目录及配置文件
	if(!mkdir(Cur_Dir_Addr, "home")) return false;
	cd(Cur_Dir_Addr, "home");
	if(!mkdir(Cur_Dir_Addr, "root"))	return false;
	
	gotoRoot();
	if(!mkdir(Cur_Dir_Addr, "etc")) return false;
	cd(Cur_Dir_Addr, "etc");

	char buf[1000] = { 0 };
	sprintf(buf, "root:%d:%d", nextUID, nextGID);//root:uid-0,gid-0
	if(!mkfile(Cur_Dir_Addr, "passwd", buf)) return false;

	int pmode = 0400;//owner:可读
	sprintf(buf, "root:root");
	if(!mkfile(Cur_Dir_Addr, "shadow", buf)) return false;
	chmod(Cur_Dir_Addr, "shadow", pmode);

	sprintf(buf, "root:%d:root\n", ROOT);
	sprintf(buf + strlen(buf), "teacher:%d:\n", TEACHER);
	sprintf(buf + strlen(buf), "student:%d:\n", STUDENT);
	if(!mkfile(Cur_Dir_Addr, "group", buf)) return false;
	
	gotoRoot();
	return true;
}


bool mkdir(int PIAddr, char name[]) {	//目录创建函数
	//理论上Cur_Dir_Addr是系统分配的，应该是正确的
	if (strlen(name) > FILE_NAME_MAX_SIZE) {
		printf("文件名称超过最大长度\n");
		return false;
	}

	//查找父目录的空位置
	int bpos = -1;
	int dpos = -1;
	int empty_b = -1;
	inode parino;
	fseek(fr, PIAddr, SEEK_SET);
	fread(&parino, sizeof(parino), 1, fr);

	for (int i = 0; i < 10; ++i) {
		int baddr = parino.i_dirBlock[i];
		if (baddr != -1) {//block已被使用 
			DirItem ditem[DirItem_Size];
			fseek(fr, baddr, SEEK_SET);
			fread(ditem, sizeof(BLOCK_SIZE), 1, fr);
			for (int j = 0; j < 16; ++j) {
				if (strcmp(ditem[j].itemName, "") == 0) {//未使用过的项
					bpos = i;
					dpos = j;
				}
				if (strcmp(ditem[j].itemName, name)==0) {//判断：存在同名目录
					inode temp;
					fseek(fr, ditem[j].inodeAddr, SEEK_SET);
					fread(&temp, sizeof(inode), 1, fr);
					if (((temp.inode_mode >> 9) & 1) == 1) {//是目录
						printf("该目录下已包含同名目录\n");
						return false;
					}
				}
			}
		}
	}
	fflush(fr);

	if (bpos == -1 || dpos == -1) {	//block不足要新开
		for (int i = 0; i < 10; ++i) {
			if (parino.i_dirBlock[i] == -1) {
				empty_b = i;
			}
		}
		if (empty_b == -1) {
			printf("该目录已满，无法添加更多文件");
			return false;
		}
		int baddr = balloc();
		if (baddr == -1)	return false;

		parino.i_dirBlock[empty_b] = baddr;
		parino.inode_file_size += BLOCK_SIZE;
		//fseek(fw, Cur_Dir_Addr, SEEK_SET);
		//fwrite(&parino, sizeof(parino), 1, fw);

		DirItem ditem[DirItem_Size];
		fseek(fw, baddr, SEEK_SET);
		fwrite(ditem, sizeof(BLOCK_SIZE), 1, fr);
		bpos = empty_b;
		dpos = 0;
	}

	//创建子目录：bpos,dpos
	int chiiaddr = ialloc();
	int chibaddr = balloc();

	inode chiino;
	chiino.inode_id = chiiaddr;
	chiino.inode_mode = MODE_DIR | DIR_DEF_PERMISSION;
	chiino.inode_file_count = 2; //"."和".."
	strcpy(chiino.i_uname, Cur_User_Name);
	strcpy(chiino.i_gname, Cur_Group_Name);
	chiino.inode_file_size = 1 * BLOCK_SIZE;
	time(&chiino.inode_change_time);
	time(&chiino.file_change_time);
	time(&chiino.file_modified_time);
	memset(chiino.i_dirBlock, -1, 10);
	chiino.i_dirBlock[0] = chibaddr;
	fseek(fw, chiiaddr, SEEK_SET);
	fwrite(&chiino, sizeof(inode), 1, fw);

	DirItem ditem[DirItem_Size];
	ditem[0].inodeAddr = chiiaddr;
	strcpy(ditem[0].itemName, ".");
	ditem[1].inodeAddr = Cur_Dir_Addr;
	strcpy(ditem[1].itemName, "..");
	fseek(fw, chibaddr, SEEK_SET);
	fwrite(ditem, sizeof(ditem), 1, fw);

	parino.inode_file_count += 1;
	fseek(fw, PIAddr, SEEK_SET);
	fwrite(&parino, sizeof(parino), 1, fw);

	fflush(fw);

	return true;
}
bool mkfile(int PIAddr, char name[],char buf[]) {	//文件创建函数
	//理论上Cur_Dir_Addr是系统分配的，应该是正确的
	if (strlen(name) > FILE_NAME_MAX_SIZE) {
		printf("文件名称超过最大长度\n");
		return false;
	}

	//查找父目录的空位置
	int bpos = -1;
	int dpos = -1;
	int empty_b = -1;
	inode parino;
	fseek(fr, PIAddr, SEEK_SET);
	fread(&parino, sizeof(parino), 1, fr);

	for (int i = 0; i < 10; ++i) {
		int baddr = parino.i_dirBlock[i];
		if (baddr != -1) {//block已被使用 
			DirItem ditem[DirItem_Size];
			fseek(fr, baddr, SEEK_SET);
			fread(ditem, sizeof(BLOCK_SIZE), 1, fr);
			for (int j = 0; j < 16; ++j) {
				if (strcmp(ditem[j].itemName, "") == 0) {//未使用过的项
					bpos = i;
					dpos = j;
				}
				if (strcmp(ditem[j].itemName, name)==0) {//判断：存在同名文件
					inode temp;
					fseek(fr, ditem[j].inodeAddr, SEEK_SET);
					fread(&temp, sizeof(inode), 1, fr);
					if (((temp.inode_mode >> 9) & 1) == 0) {//是文件
						printf("该目录下已包含同名文件\n");
						return false;
					}
				}
			}
		}
	}
	fflush(fr);

	if (bpos == -1 || dpos == -1) {	//block不足要新开
		for (int i = 0; i < 10; ++i) {
			if (parino.i_dirBlock[i] == -1) {
				empty_b = i;
			}
		}
		if (empty_b == -1) {
			printf("该目录已满，无法添加更多文件");
			return false;
		}
		int baddr = balloc();
		if (baddr == -1)	return false;

		parino.i_dirBlock[empty_b] = baddr;
		parino.inode_file_size += BLOCK_SIZE;
		//fseek(fw, Cur_Dir_Addr, SEEK_SET);
		//fwrite(&parino, sizeof(parino), 1, fw);

		DirItem ditem[DirItem_Size];
		fseek(fw, baddr, SEEK_SET);
		fwrite(ditem, sizeof(BLOCK_SIZE), 1, fr);
		bpos = empty_b;
		dpos = 0;
	}

	//创建子目录：bpos,dpos
	int chiiaddr = ialloc();
	int chibaddr = balloc();
	inode chiino;
	chiino.inode_id = chiiaddr;
	chiino.inode_mode = MODE_FILE | FILE_DEF_PERMISSION;
	chiino.inode_file_count = 1; 
	strcpy(chiino.i_uname, Cur_User_Name);
	strcpy(chiino.i_gname, Cur_Group_Name);
	time(&chiino.inode_change_time);
	time(&chiino.file_change_time);
	time(&chiino.file_modified_time);
	memset(chiino.i_dirBlock, -1, 10);
	chiino.i_dirBlock[0] = chibaddr;
	chiino.inode_file_size =0;
	fseek(fw, chiiaddr, SEEK_SET);
	fwrite(&chiino, sizeof(inode), 1, fw);

	writefile(chiino, chiiaddr, buf);//将buf信息写入

	parino.inode_file_count += 1;
	fseek(fw, PIAddr, SEEK_SET);
	fwrite(&parino, sizeof(parino), 1, fw);
	
	fflush(fw);

	return true;
}
void writefile(inode fileinode, int iaddr, char buf[]) { //文件写入（续写）
	//前提：假设是按照block顺序存储
	if ((fileinode.inode_file_size + strlen(buf)) > 10 * BLOCK_SIZE) {
		printf("文件内存不足，无法继续添加内容\n");
		return;
	}
	int start = fileinode.inode_file_size / BLOCK_SIZE ; //使用到第几块（考虑block[0])
	int offset = 0;
	int left = 0;
	for (int i = start; i < 10; ++i) {
		if(fileinode.i_dirBlock[i] != -1) {	//正在使用块(补全）
			offset = fileinode.i_dirBlock[i] + fileinode.inode_file_size % BLOCK_SIZE;
			left = BLOCK_SIZE - offset;
			fseek(fw, offset, SEEK_SET);
			fwrite(buf, left, 1, fw);
		}
		else {
			int baddr = balloc();
			int k = i - start-1;
			if (baddr == -1)return;
			fseek(fw, baddr, SEEK_SET);
			fwrite(buf + left + k * BLOCK_SIZE, sizeof(BLOCK_SIZE), 1, fw);//最后一个要是不够呢？

			fileinode.i_dirBlock[i] = baddr;
		}
	}
	fileinode.inode_file_count += strlen(buf);
	fseek(fw, iaddr, SEEK_SET);
	fwrite(&fileinode, sizeof(inode), 1, fw);
}
void cd(int PIAddr, char name[]) {//切换目录（权限）
	inode pinode;
	fseek(fr, PIAddr, SEEK_SET);
	fread(&pinode, sizeof(inode), 1, fr);

	//判断身份
	int role = 0;	//other 0
	if (strcmp(Cur_Group_Name, pinode.i_gname) == 0) {
		role = 3;	//group 3
	}
	if (strcmp(Cur_User_Name, pinode.i_uname) == 0) {
		role = 6;
	}

	for (int i = 0; i < 10; ++i) {
		if (pinode.i_dirBlock[i] != -1) {
			DirItem ditem[DirItem_Size];
			fseek(fr, pinode.i_dirBlock[i], SEEK_SET);
			fread(ditem, sizeof(BLOCK_SIZE), 1, fr);
			for (int j = 0; j < DirItem_Size; ++j) {
				if (strcmp(ditem[j].itemName, name) == 0) { //找到同名
					if (strcmp(name, ".") == 0) {
						return;
					}
					if (strcmp(name, "..") == 0) {
						if (strcmp(Cur_Dir_Name, "/") ){
							return;
						}
						//char* p = strrchr(Cur_Dir_Addr, '/'); 跑不了啊
						char* p = Cur_Dir_Name+strlen(Cur_Dir_Name);
						while ((*p) != '/')p--;
						p = '\0'; //打断它
					}
					inode chiino;
					fseek(fr, ditem[j].inodeAddr, SEEK_SET);
					fread(&chiino, sizeof(inode), 1, fr);
					if (((chiino.inode_mode >> role) & 1) == 1) {	//是否有执行权限
						strcat(Cur_Dir_Name, "/");
						strcat(Cur_Dir_Name, name);
					}
				}
			}
		}
	}
	return;
}
void gotoRoot() {
	Cur_Dir_Addr= Root_Dir_Addr;
	strcpy(Cur_Dir_Name , "/");
}

int ialloc() { //分配inode，满了返回-1
	int iaddr = -1;
	for (int i = 0; i < INODE_NUM; i++) {
		if (inode_bitmap[i] == 0) {
			iaddr = i;
			inode_bitmap[i] = 1;
			break;
		}
	}
	if (iaddr == -1) {
		printf("没有inode空间\n");
		return -1;
	}
	superblock->s_free_INODE_NUM -= 1;
	fseek(fw, Superblock_Start_Addr, SEEK_SET);
	fwrite(superblock, sizeof(superblock), 1, fw);
	fseek(fw, InodeBitmap_Start_Addr, SEEK_SET);
	fwrite(inode_bitmap, sizeof(inode_bitmap), 1, fw);
	return iaddr;
}
int ifree(int iaddr) {
	if ((iaddr % INODE_SIZE) != 0) {

	}
}
int balloc() { //分配block，满了返回-1
	int baddr = -1;
	for (int i = 0; i < BLOCK_NUM; i++) {
		if (block_bitmap[i] == 0) {
			baddr = i;
			block_bitmap[i] = 1;
			break;
		}
	}
	if (baddr == -1) {
		printf("没有block空间\n");
		return -1;
	}
	superblock->s_free_BLOCK_NUM -= 1;
	fseek(fw, Superblock_Start_Addr, SEEK_SET);
	fwrite(superblock, sizeof(superblock), 1, fw);
	fseek(fw, BlockBitmap_Start_Addr, SEEK_SET);
	fwrite(block_bitmap, sizeof(block_bitmap), 1, fw);
	return baddr;
}
bool Install() {
	return true;
}

void inUsername(char username[])	//输入用户名
{
	printf("username:");
	scanf("%s", username);	//用户名
}
void inPasswd(char passwd[])	//输入密码
{
	printf("password:");
	scanf("%s", passwd);
}
bool login()	//登陆界面
{
	char username[100] = { 0 };
	char passwd[100] = { 0 };
	inUsername(username);	//输入用户名
	inPasswd(passwd);		//输入用户密码
	printf("到这1");
	//if (strcmp(username,"root")==0&& strcmp(passwd, "root")==0) {	//核对用户名和密码
	if(1){
		printf("到这2");
		isLogin = true;
		return true;
	}
	else {
		printf("到这3");
		isLogin = false;
		return false;
	}
}
void chmod(int PIAddr, char name[], int pmode) {//修改文件or目录权限（假定文件和目录也不能重名）
	if (strlen(name) > FILENAME_MAX) {
		printf("文件名称超过最大长度\n");
		return;
	}
	if (strcmp(name, ".") || strcmp(name, "..")) {
		printf("该文件无法修改权限\n");
		return;
	}
	inode ino;
	fseek(fr, PIAddr, SEEK_SET);
	fread(&ino, sizeof(inode), 1, fr);
	for (int i = 0; i < 10; ++i) {
		DirItem ditem[DirItem_Size];
		fseek(fr, ino.i_dirBlock[i], SEEK_SET);
		fread(ditem, sizeof(BLOCK_SIZE), 1, fr);
		for (int j = 0; j < DirItem_Size; ++j) {
			if (strcmp(ditem[j].itemName, name)) {//找到同名文件
				inode chiino;
				fseek(fr, ditem[j].inodeAddr, SEEK_SET);
				fread(&chiino, sizeof(inode), 1, fr);
				//只有创建者和管理员可以更改权限
				if ((strcmp(chiino.i_uname, Cur_Dir_Name) == 0) || strcmp(Cur_Dir_Name, "root")) {
					chiino.inode_mode = (chiino.inode_mode >> 9 << 9) | pmode;
					fseek(fr, ditem[j].inodeAddr, SEEK_SET);
					fwrite(&chiino, sizeof(inode), 1, fr);
				}
			}
		}
	}
}