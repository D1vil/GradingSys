#include"os.h"
#include<iomanip>
#include<time.h>
#include<string.h>
#include<stdio.h>

using namespace std;

void help() {
	
	cout.setf(ios::left); //���ö��뷽ʽΪleft 
	cout.width(30); //���ÿ�ȣ������ÿո���� 
	//cout << setiosflags(ios::left);
	cout << "ls" << "Display the current directory listing" << endl;	//�г���ǰĿ¼�嵥
	cout.width(30);
	cout << "cd" << "Enter the specific directory " << endl;		//ǰ��ָ��Ŀ¼
	cout.width(30);
	cout << "mkdir" << "Create directory" << endl;					//����Ŀ¼
	cout.width(30);
	cout << "rm" << "Delete the file or directory" << endl;			//ɾ���ļ���Ŀ¼ 
	cout.width(30);
	cout << "touch" << "Create new file" << endl;				//�������ļ�
	cout.width(30);
	cout << "read" << "Read the content of file" << endl;		//���ļ�
	cout.width(30);
	cout << "write" << "Write the file" << endl;			//д�ļ�
	cout.width(30);
	cout << "chmod" << "Modify the access right" << endl;		//�޸��ļ�Ȩ��
	cout.width(30);
	cout << "adduser" << "Add user" << endl;		//�����û�
	cout.width(30);
	cout << "deluser" << "Delete user" << endl;		//ɾ���û�
	cout.width(30);
	cout << "addusergrp" << "Add user group" << endl;		//�����û���
	cout.width(30);
	cout << "delusergrp" << "Delete user group" << endl;		//ɾ���û���
	cout.width(30);
	cout << "snapshot" << "Back up the system" << endl;			//����ϵͳ
	cout.width(30);
	cout << "recover" << "Recover the system" << endl;			//�ָ�ϵͳ
}

bool Format() {
	//��ʼ��:������,λͼ
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

	//inode��block�����ʱ����Ҫ����
	fflush(fw);//���������ݷ���fw��

	//������Ŀ¼
	int iaddr = ialloc();
	int baddr = balloc();
	if (iaddr == -1 || baddr==-1) {
		return false;
	}

	inode ino;
	ino.inode_id = iaddr;
	ino.inode_mode = MODE_DIR | DIR_DEF_PERMISSION;
	ino.inode_file_count = 1; //"."Ҳ��
	strcpy(ino.i_uname , "root");
	strcpy(ino.i_gname , "root");
	ino.inode_file_size = 1 * BLOCK_SIZE;
	time(&ino.inode_change_time);
	time(&ino.file_change_time);
	time(&ino.file_modified_time);
	// printf("%s", asctime(localtime(&time)));ת�ɺ��ʸ�ʽ
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
	
	//����Ŀ¼�������ļ�
	if(!mkdir(Cur_Dir_Addr, "home")) return false;
	cd(Cur_Dir_Addr, "home");
	if(!mkdir(Cur_Dir_Addr, "root"))	return false;
	
	gotoRoot();
	if(!mkdir(Cur_Dir_Addr, "etc")) return false;
	cd(Cur_Dir_Addr, "etc");

	char buf[1000] = { 0 };
	sprintf(buf, "root:%d:%d", nextUID, nextGID);//root:uid-0,gid-0
	if(!mkfile(Cur_Dir_Addr, "passwd", buf)) return false;

	int pmode = 0400;//owner:�ɶ�
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


bool mkdir(int PIAddr, char name[]) {	//Ŀ¼��������
	//������Cur_Dir_Addr��ϵͳ����ģ�Ӧ������ȷ��
	if (strlen(name) > FILE_NAME_MAX_SIZE) {
		printf("�ļ����Ƴ�����󳤶�\n");
		return false;
	}

	//���Ҹ�Ŀ¼�Ŀ�λ��
	int bpos = -1;
	int dpos = -1;
	int empty_b = -1;
	inode parino;
	fseek(fr, PIAddr, SEEK_SET);
	fread(&parino, sizeof(parino), 1, fr);

	for (int i = 0; i < 10; ++i) {
		int baddr = parino.i_dirBlock[i];
		if (baddr != -1) {//block�ѱ�ʹ�� 
			DirItem ditem[DirItem_Size];
			fseek(fr, baddr, SEEK_SET);
			fread(ditem, sizeof(BLOCK_SIZE), 1, fr);
			for (int j = 0; j < 16; ++j) {
				if (strcmp(ditem[j].itemName, "") == 0) {//δʹ�ù�����
					bpos = i;
					dpos = j;
				}
				if (strcmp(ditem[j].itemName, name)==0) {//�жϣ�����ͬ��Ŀ¼
					inode temp;
					fseek(fr, ditem[j].inodeAddr, SEEK_SET);
					fread(&temp, sizeof(inode), 1, fr);
					if (((temp.inode_mode >> 9) & 1) == 1) {//��Ŀ¼
						printf("��Ŀ¼���Ѱ���ͬ��Ŀ¼\n");
						return false;
					}
				}
			}
		}
	}
	fflush(fr);

	if (bpos == -1 || dpos == -1) {	//block����Ҫ�¿�
		for (int i = 0; i < 10; ++i) {
			if (parino.i_dirBlock[i] == -1) {
				empty_b = i;
			}
		}
		if (empty_b == -1) {
			printf("��Ŀ¼�������޷���Ӹ����ļ�");
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

	//������Ŀ¼��bpos,dpos
	int chiiaddr = ialloc();
	int chibaddr = balloc();

	inode chiino;
	chiino.inode_id = chiiaddr;
	chiino.inode_mode = MODE_DIR | DIR_DEF_PERMISSION;
	chiino.inode_file_count = 2; //"."��".."
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
bool mkfile(int PIAddr, char name[],char buf[]) {	//�ļ���������
	//������Cur_Dir_Addr��ϵͳ����ģ�Ӧ������ȷ��
	if (strlen(name) > FILE_NAME_MAX_SIZE) {
		printf("�ļ����Ƴ�����󳤶�\n");
		return false;
	}

	//���Ҹ�Ŀ¼�Ŀ�λ��
	int bpos = -1;
	int dpos = -1;
	int empty_b = -1;
	inode parino;
	fseek(fr, PIAddr, SEEK_SET);
	fread(&parino, sizeof(parino), 1, fr);

	for (int i = 0; i < 10; ++i) {
		int baddr = parino.i_dirBlock[i];
		if (baddr != -1) {//block�ѱ�ʹ�� 
			DirItem ditem[DirItem_Size];
			fseek(fr, baddr, SEEK_SET);
			fread(ditem, sizeof(BLOCK_SIZE), 1, fr);
			for (int j = 0; j < 16; ++j) {
				if (strcmp(ditem[j].itemName, "") == 0) {//δʹ�ù�����
					bpos = i;
					dpos = j;
				}
				if (strcmp(ditem[j].itemName, name)==0) {//�жϣ�����ͬ���ļ�
					inode temp;
					fseek(fr, ditem[j].inodeAddr, SEEK_SET);
					fread(&temp, sizeof(inode), 1, fr);
					if (((temp.inode_mode >> 9) & 1) == 0) {//���ļ�
						printf("��Ŀ¼���Ѱ���ͬ���ļ�\n");
						return false;
					}
				}
			}
		}
	}
	fflush(fr);

	if (bpos == -1 || dpos == -1) {	//block����Ҫ�¿�
		for (int i = 0; i < 10; ++i) {
			if (parino.i_dirBlock[i] == -1) {
				empty_b = i;
			}
		}
		if (empty_b == -1) {
			printf("��Ŀ¼�������޷���Ӹ����ļ�");
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

	//������Ŀ¼��bpos,dpos
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

	writefile(chiino, chiiaddr, buf);//��buf��Ϣд��

	parino.inode_file_count += 1;
	fseek(fw, PIAddr, SEEK_SET);
	fwrite(&parino, sizeof(parino), 1, fw);
	
	fflush(fw);

	return true;
}
void writefile(inode fileinode, int iaddr, char buf[]) { //�ļ�д�루��д��
	//ǰ�᣺�����ǰ���block˳��洢
	if ((fileinode.inode_file_size + strlen(buf)) > 10 * BLOCK_SIZE) {
		printf("�ļ��ڴ治�㣬�޷������������\n");
		return;
	}
	int start = fileinode.inode_file_size / BLOCK_SIZE ; //ʹ�õ��ڼ��飨����block[0])
	int offset = 0;
	int left = 0;
	for (int i = start; i < 10; ++i) {
		if(fileinode.i_dirBlock[i] != -1) {	//����ʹ�ÿ�(��ȫ��
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
			fwrite(buf + left + k * BLOCK_SIZE, sizeof(BLOCK_SIZE), 1, fw);//���һ��Ҫ�ǲ����أ�

			fileinode.i_dirBlock[i] = baddr;
		}
	}
	fileinode.inode_file_count += strlen(buf);
	fseek(fw, iaddr, SEEK_SET);
	fwrite(&fileinode, sizeof(inode), 1, fw);
}
void cd(int PIAddr, char name[]) {//�л�Ŀ¼��Ȩ�ޣ�
	inode pinode;
	fseek(fr, PIAddr, SEEK_SET);
	fread(&pinode, sizeof(inode), 1, fr);

	//�ж����
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
				if (strcmp(ditem[j].itemName, name) == 0) { //�ҵ�ͬ��
					if (strcmp(name, ".") == 0) {
						return;
					}
					if (strcmp(name, "..") == 0) {
						if (strcmp(Cur_Dir_Name, "/") ){
							return;
						}
						//char* p = strrchr(Cur_Dir_Addr, '/'); �ܲ��˰�
						char* p = Cur_Dir_Name+strlen(Cur_Dir_Name);
						while ((*p) != '/')p--;
						p = '\0'; //�����
					}
					inode chiino;
					fseek(fr, ditem[j].inodeAddr, SEEK_SET);
					fread(&chiino, sizeof(inode), 1, fr);
					if (((chiino.inode_mode >> role) & 1) == 1) {	//�Ƿ���ִ��Ȩ��
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

int ialloc() { //����inode�����˷���-1
	int iaddr = -1;
	for (int i = 0; i < INODE_NUM; i++) {
		if (inode_bitmap[i] == 0) {
			iaddr = i;
			inode_bitmap[i] = 1;
			break;
		}
	}
	if (iaddr == -1) {
		printf("û��inode�ռ�\n");
		return -1;
	}
	superblock->s_free_INODE_NUM -= 1;
	fseek(fw, Superblock_Start_Addr, SEEK_SET);
	fwrite(superblock, sizeof(superblock), 1, fw);
	fseek(fw, InodeBitmap_Start_Addr, SEEK_SET);
	fwrite(inode_bitmap, sizeof(inode_bitmap), 1, fw);
	return iaddr;
}
void ifree(int iaddr) {
	if ((iaddr % INODE_SIZE) != 0) {
		printf("��ǰinodeλ�ô���\n");
		return;
	}
	int index = (iaddr - InodeBitmap_Start_Addr) / INODE_SIZE;
	if (inode_bitmap[index] == 0) {
		printf("δʹ�õ�ǰinode�������ͷ�\n");
		return;
	}
	inode_bitmap[index] = 0;
	fseek(fw, InodeBitmap_Start_Addr, SEEK_SET);
	fwrite(inode_bitmap, sizeof(inode_bitmap), 1, fw);
	inode ino;
	fseek(fw, iaddr, SEEK_SET);
	fwrite(&ino, sizeof(INODE_SIZE), 1, fw);
	superblock->s_free_INODE_NUM -= 1;
	fseek(fw, Superblock_Start_Addr, SEEK_SET);
	fwrite(superblock, sizeof(superblock), 1, fw);
}
int balloc() { //����block�����˷���-1
	int baddr = -1;
	for (int i = 0; i < BLOCK_NUM; i++) {
		if (block_bitmap[i] == 0) {
			baddr = i;
			block_bitmap[i] = 1;
			break;
		}
	}
	if (baddr == -1) {
		printf("û��block�ռ�\n");
		return -1;
	}
	superblock->s_free_BLOCK_NUM -= 1;
	fseek(fw, Superblock_Start_Addr, SEEK_SET);
	fwrite(superblock, sizeof(superblock), 1, fw);
	fseek(fw, BlockBitmap_Start_Addr, SEEK_SET);
	fwrite(block_bitmap, sizeof(block_bitmap), 1, fw);
	return baddr;
}
void bfree(int baddr) {
	if ((baddr % BLOCK_SIZE) != 0) {
		printf("��ǰblockλ�ô���\n");
		return;
	}
	int index = (baddr - BlockBitmap_Start_Addr) / BLOCK_SIZE;
	if (block_bitmap[index] == 0) {
		printf("δʹ�õ�ǰblock�������ͷ�\n");
		return;
	}
	block_bitmap[index] = 0;
	fseek(fw, BlockBitmap_Start_Addr, SEEK_SET);
	fwrite(block_bitmap, sizeof(block_bitmap), 1, fw);
	superblock->s_free_BLOCK_NUM -= 1;
	fseek(fw, Superblock_Start_Addr, SEEK_SET);
	fwrite(superblock, sizeof(superblock), 1, fw);
}
bool Install() {
	return true;
}

void inUsername(char username[])	//�����û���
{
	printf("username:");
	scanf("%s", username);	//�û���
}
void inPasswd(char passwd[])	//��������
{
	printf("password:");
	scanf("%s", passwd);
}
bool login()	//��½����
{
	char username[100] = { 0 };
	char passwd[100] = { 0 };
	inUsername(username);	//�����û���
	inPasswd(passwd);		//�����û�����
	printf("����1");
	//if (strcmp(username,"root")==0&& strcmp(passwd, "root")==0) {	//�˶��û���������
	if(1){
		printf("����2");
		isLogin = true;
		return true;
	}
	else {
		printf("����3");
		isLogin = false;
		return false;
	}
}
bool logout() {	//�û�ע��
	gotoRoot();
	strcmp(Cur_User_Name, "");
	strcmp(Cur_Group_Name, "");
	strcmp(Cur_User_Dir_Name, "");
	isLogin = false;
	return true;
	//pause
}
bool useradd(char username[], char passwd[], char group[]) {	//�û�ע��
	//Ȩ���ж�
	if (strcmp(Cur_User_Name, "root") != 0) {
		printf("Ȩ�޲��㣬�޷�����û���\n");
		return false;
	}
	//�����ֳ���������Ϣ
	int pro_cur_dir_addr = Cur_Dir_Addr;
	char pro_cur_dir_name[310], pro_cur_user_name[110], pro_cur_group_name[110], pro_cur_user_dir_name[310];
	strcpy(pro_cur_dir_name, Cur_Dir_Name);
	strcpy(pro_cur_user_name, Cur_User_Name);
	strcpy(pro_cur_group_name, Cur_Group_Name);
	strcpy(pro_cur_user_dir_name, Cur_User_Dir_Name);
	
	strcpy(Cur_User_Name, username);
	strcpy(Cur_Group_Name, group);
	
	gotoRoot();
	cd(Cur_Dir_Addr, "home");
	// δ���������

}
void chmod(int PIAddr, char name[], int pmode) {//�޸��ļ�orĿ¼Ȩ�ޣ��ٶ��ļ���Ŀ¼Ҳ����������
	if (strlen(name) > FILENAME_MAX) {
		printf("�ļ����Ƴ�����󳤶�\n");
		return;
	}
	if (strcmp(name, ".") || strcmp(name, "..")) {
		printf("���ļ��޷��޸�Ȩ��\n");
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
			if (strcmp(ditem[j].itemName, name)) {//�ҵ�ͬ���ļ�
				inode chiino;
				fseek(fr, ditem[j].inodeAddr, SEEK_SET);
				fread(&chiino, sizeof(inode), 1, fr);
				//ֻ�д����ߺ͹���Ա���Ը���Ȩ��
				if ((strcmp(chiino.i_uname, Cur_Dir_Name) == 0) || strcmp(Cur_Dir_Name, "root")) {
					chiino.inode_mode = (chiino.inode_mode >> 9 << 9) | pmode;
					fseek(fr, ditem[j].inodeAddr, SEEK_SET);
					fwrite(&chiino, sizeof(inode), 1, fr);
				}
			}
		}
	}
}