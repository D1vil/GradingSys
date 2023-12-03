#include"os.h"
#include<iomanip>
#include<time.h>
#include<string.h>
#include<stdio.h>

//using namespace std;

void help() {
	
	//cout.setf(ios::left); //���ö��뷽ʽΪleft 
	//cout.width(30); //���ÿ�ȣ������ÿո���� 
	////cout << setiosflags(ios::left);
	//cout << "ls" << "Display the current directory listing" << endl;	//�г���ǰĿ¼�嵥
	//cout.width(30);
	//cout << "cd" << "Enter the specific directory " << endl;		//ǰ��ָ��Ŀ¼
	//cout.width(30);
	//cout << "mkdir" << "Create directory" << endl;					//����Ŀ¼
	//cout.width(30);
	//cout << "rm" << "Delete the file or directory" << endl;			//ɾ���ļ���Ŀ¼ 
	//cout.width(30);
	//cout << "touch" << "Create new file" << endl;				//�������ļ�
	//cout.width(30);
	//cout << "read" << "Read the content of file" << endl;		//���ļ�
	//cout.width(30);
	//cout << "write" << "Write the file" << endl;			//д�ļ�
	//cout.width(30);
	//cout << "chmod" << "Modify the access right" << endl;		//�޸��ļ�Ȩ��
	//cout.width(30);
	//cout << "adduser" << "Add user" << endl;		//�����û�
	//cout.width(30);
	//cout << "deluser" << "Delete user" << endl;		//ɾ���û�
	//cout.width(30);
	//cout << "addusergrp" << "Add user group" << endl;		//�����û���
	//cout.width(30);
	//cout << "delusergrp" << "Delete user group" << endl;		//ɾ���û���
	//cout.width(30);
	//cout << "snapshot" << "Back up the system" << endl;			//����ϵͳ
	//cout.width(30);
	//cout << "recover" << "Recover the system" << endl;			//�ָ�ϵͳ
}

//****���ຯ��****
bool Format() { //ok
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
	superblock->s_BlockBitmap_Start_Addr = BlockBitmap_Start_Addr;
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
	memset(ino.i_dirBlock, -1, sizeof(ino.i_dirBlock));
	ino.i_dirBlock[0] = baddr;
	fseek(fw, iaddr, SEEK_SET);
	fwrite(&ino, sizeof(inode), 1, fw);

	DirItem ditem[DirItem_Size];
	for (int i = 0; i < DirItem_Size; ++i){
		ditem[i].inodeAddr = -1;
		strcpy(ditem[i].itemName, "");
	}
	ditem[0].inodeAddr = iaddr;
	strcpy(ditem[0].itemName , ".");
	fseek(fw, baddr, SEEK_SET);
	fwrite(ditem, sizeof(ditem), 1, fw);

	fflush(fw);
	//����Ŀ¼�������ļ�
	mkdir(Cur_Dir_Addr, "home");
	cd(Cur_Dir_Addr, "home");
	mkdir(Cur_Dir_Addr, "root");
	
	gotoRoot();
	mkdir(Cur_Dir_Addr, "etc");
	cd(Cur_Dir_Addr, "etc");

	char buf[1000] = { 0 };
	sprintf(buf, "root:%d:%d\n", nextUID, nextGID);//root:uid-0,gid-0
	mkfile(Cur_Dir_Addr, "passwd", buf);

	int pmode = 0400;//owner:�ɶ�
	sprintf(buf, "root:root\n");
	mkfile(Cur_Dir_Addr, "shadow", buf);
	chmod(Cur_Dir_Addr, "shadow", pmode,0);

	sprintf(buf, "root:%d:root\n", ROOT);
	sprintf(buf + strlen(buf), "teacher:%d:\n", TEACHER);
	sprintf(buf + strlen(buf), "student:%d:\n", STUDENT);
	mkfile(Cur_Dir_Addr, "group", buf);
	
	gotoRoot();
	return true;
}
bool Install() {	//��װ�ļ�ϵͳ ok
	fseek(fr, Superblock_Start_Addr, SEEK_SET);
	fread(superblock, sizeof(superblock), 1, fr);

	fseek(fr, InodeBitmap_Start_Addr, SEEK_SET);
	fread(inode_bitmap, sizeof(inode_bitmap), 1, fr);

	fseek(fr, BlockBitmap_Start_Addr, SEEK_SET);
	fread(block_bitmap, sizeof(block_bitmap), 1, fr);

	fflush(fr);
	return true;
}
bool mkdir(int PIAddr, char name[]) {	//Ŀ¼��������(��Ŀ¼Ȩ��:д)(ok
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

	//�ж����
	int role = 0;	//other 0
	if (strcmp(Cur_Group_Name, parino.i_gname) == 0) {
		role = 3;	//group 3
	}
	if (strcmp(Cur_User_Name, parino.i_uname) == 0) {
		role = 6;
	}
	if (((parino.inode_mode >> role >> 1) & 1 == 0) ||( strcmp(Cur_User_Name, "root" )!= 0)){
		printf("Ȩ�޲��㣬�޷��½�Ŀ¼\n");
		return false;
	}

	for (int i = 0; i < 10; ++i) {
		int baddr = parino.i_dirBlock[i];
		if (baddr != -1) {//block�ѱ�ʹ�� 
			DirItem ditem[DirItem_Size];
			fseek(fr, baddr, SEEK_SET);
			fread(ditem, sizeof(ditem), 1, fr);
			for (int j = 0; j < DirItem_Size; ++j) {
				if (ditem[j].inodeAddr==-1) {//δʹ�ù�����
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
	//���п鵹��洢

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
		fseek(fw, Cur_Dir_Addr, SEEK_SET);
		fwrite(&parino, sizeof(parino), 1, fw);
		fflush(fw);

		DirItem ditem[DirItem_Size];
		for (int i = 0; i < DirItem_Size; ++i) {
			ditem[i].inodeAddr = -1;
			strcpy(ditem[i].itemName, "");
		}
		fseek(fw, baddr, SEEK_SET);
		fwrite(ditem, sizeof(ditem), 1, fr);

		bpos = empty_b;
		dpos = 0;
	}

	//������Ŀ¼��bpos,dpos
	int chiiaddr = ialloc();
	int chibaddr = balloc();

	//���ڵ�inode��block����
	parino.inode_file_count += 1;
	fseek(fw, PIAddr, SEEK_SET);
	fwrite(&parino, sizeof(parino), 1, fw);

	DirItem paritem[DirItem_Size];
	fseek(fr, parino.i_dirBlock[bpos], SEEK_SET);
	fread(paritem, sizeof(paritem), 1, fr);
	fflush(fr);
	strcpy(paritem[dpos].itemName, name);
	paritem[dpos].inodeAddr = chiiaddr;
	fseek(fw, parino.i_dirBlock[bpos], SEEK_SET);
	fwrite(paritem, sizeof(paritem), 1, fw);

	//�ӽڵ����
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
	memset(chiino.i_dirBlock, -1, sizeof(chiino.i_dirBlock));
	chiino.i_dirBlock[0] = chibaddr;
	fseek(fw, chiiaddr, SEEK_SET);
	fwrite(&chiino, sizeof(inode), 1, fw);

	DirItem chiitem[DirItem_Size];
	for (int i = 0; i < DirItem_Size; ++i) {
		chiitem[i].inodeAddr = -1;
		strcpy(chiitem[i].itemName, "");
	}
	chiitem[0].inodeAddr = chiiaddr;
	strcpy(chiitem[0].itemName, ".");
	chiitem[1].inodeAddr = Cur_Dir_Addr;
	strcpy(chiitem[1].itemName, "..");
	fseek(fw, chibaddr, SEEK_SET);
	fwrite(chiitem, sizeof(chiitem), 1, fw);

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

	//�ж����
	int role = 0;	//other 0
	if (strcmp(Cur_Group_Name, parino.i_gname) == 0) {
		role = 3;	//group 3
	}
	if (strcmp(Cur_User_Name, parino.i_uname) == 0) {
		role = 6;
	}
	if (((parino.inode_mode >> role >> 1) & 1 == 0) || (strcmp(Cur_User_Name, "root") != 0)) {
		printf("Ȩ�޲��㣬�޷��½�Ŀ¼\n");
		return false;
	}
	
	for (int i = 0; i < 10; ++i) {
		int baddr = parino.i_dirBlock[i];
		if (baddr != -1) {//block�ѱ�ʹ�� 
			DirItem ditem[DirItem_Size];
			fseek(fr, baddr, SEEK_SET);
			fread(ditem, sizeof(ditem), 1, fr);
			for (int j = 0; j < DirItem_Size; ++j) {
				if (ditem[j].inodeAddr == -1) {//δʹ�ù�����
					bpos = i;
					dpos = j;
				}
				if (strcmp(ditem[j].itemName, name) == 0) {//�жϣ�����ͬ��Ŀ¼
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
		fseek(fw, Cur_Dir_Addr, SEEK_SET);
		fwrite(&parino, sizeof(parino), 1, fw);
		fflush(fw);

		DirItem ditem[DirItem_Size];
		for (int i = 0; i < DirItem_Size; ++i) {
			ditem[i].inodeAddr = -1;
			strcpy(ditem[i].itemName, "");
		}
		fseek(fw, baddr, SEEK_SET);
		fwrite(ditem, sizeof(ditem), 1, fr);

		bpos = empty_b;
		dpos = 0;
	}

	//������Ŀ¼��bpos,dpos
	int chiiaddr = ialloc();
	int chibaddr = balloc();

	//���ڵ�inode��block����
	parino.inode_file_count += 1;
	fseek(fw, PIAddr, SEEK_SET);
	fwrite(&parino, sizeof(parino), 1, fw);

	DirItem paritem[DirItem_Size];
	fseek(fr, parino.i_dirBlock[bpos], SEEK_SET);
	fread(paritem, sizeof(paritem), 1, fr);
	fflush(fr);
	strcpy(paritem[dpos].itemName, name);
	paritem[dpos].inodeAddr = chiiaddr;
	fseek(fw, parino.i_dirBlock[bpos], SEEK_SET);
	fwrite(paritem, sizeof(paritem), 1, fw);

	inode chiino;
	chiino.inode_id = chiiaddr;
	chiino.inode_mode = MODE_FILE | FILE_DEF_PERMISSION;
	chiino.inode_file_count = 1; 
	strcpy(chiino.i_uname, Cur_User_Name);
	strcpy(chiino.i_gname, Cur_Group_Name);
	time(&chiino.inode_change_time);
	time(&chiino.file_change_time);
	time(&chiino.file_modified_time);
	memset(chiino.i_dirBlock, -1, sizeof(chiino.i_dirBlock));
	chiino.i_dirBlock[0] = chibaddr;
	chiino.inode_file_size =0;
	fseek(fw, chiiaddr, SEEK_SET);
	fwrite(&chiino, sizeof(inode), 1, fw);

	char temp_file[BLOCK_SIZE];
	memset(temp_file, '\0', BLOCK_SIZE);
	fseek(fw, chibaddr, SEEK_SET);
	fwrite(temp_file, BLOCK_SIZE, 1, fw);
	writefile(chiino, chiiaddr, buf);//��buf��Ϣд��
	
	fflush(fw);
	return true;
}
bool rmdir(int CHIAddr, char name[]) {//ɾ����ǰĿ¼
	if (strlen(name) > FILE_NAME_MAX_SIZE) {
		printf("�ļ����Ƴ�����󳤶�\n");
		return false;
	}
	if ((strcmp(name, ".") == 0) || strcmp(name, "..") == 0) {
		printf("�ļ��������޷�ɾ��\n");
		return false;
	}

	//�ж�Ȩ��
	inode ino;
	fseek(fr, CHIAddr, SEEK_SET);
	fread(&ino, sizeof(inode), 1, fr);

	int mode = 0;//other
	if (strcmp(Cur_Group_Name, ino.i_gname) == 0) {//group
		mode = 3;
	}
	if (strcmp(Cur_User_Name, ino.i_uname) == 0) {//owner
		mode = 6;
	}
	if (((ino.inode_mode >> mode >> 1) & 1 == 0) || (strcmp(Cur_User_Name, "root") != 0)) {//�Ƿ��д��2
		printf("û��Ȩ��ɾ�����ļ���\n");
		return false;
	}

	//ɾ�������ļ���
	for (int i = 0; i < 10; ++i) {
		DirItem ditem[DirItem_Size];
		if (ino.i_dirBlock[i] != -1) {//��ʹ�ù�
			fseek(fr, ino.i_dirBlock[i], SEEK_SET);
			fread(ditem, sizeof(BLOCK_SIZE), 1, fr);
			for (int j = 0; j < DirItem_Size; ++j) {
				inode chiino;
				if (strcmp(ditem[j].itemName, ".") == 0 || strcmp(ditem[j].itemName, "..") == 0) {
					continue;
				}
				if (strlen(ditem[j].itemName) != 0) {
					fseek(fr, ditem[j].inodeAddr, SEEK_SET);
					fread(&chiino, sizeof(inode), 1, fr);
					if ((chiino.inode_mode >> 9) & 1 == 1) {	//Ŀ¼
						rmdir(ditem[j].inodeAddr, ditem[j].itemName);
					}
					else {										//�ļ�
						rmfile(ditem[j].inodeAddr, ditem[j].itemName);
					}
				}
				ditem[j].inodeAddr = -1;
				strcpy(ditem[j].itemName, "");
			}
			//block����&inode��������
			bfree(ino.i_dirBlock[i]);
			ino.inode_file_count -= 16;
			ino.inode_file_size -= BLOCK_SIZE;
			ino.i_dirBlock[i] = -1;
		}
	}

	ifree(CHIAddr);
	return true;
}
bool rmfile(int CHIAddr, char name[]) {	//ɾ����ǰ�ļ�
	if (strlen(name) > FILE_NAME_MAX_SIZE) {
		printf("�ļ����Ƴ�����󳤶�\n");
		return false;
	}

	//�ж�Ȩ��
	inode ino;
	fseek(fr, CHIAddr, SEEK_SET);
	fread(&ino, sizeof(inode), 1, fr);

	int mode = 0;//other
	if (strcmp(Cur_Group_Name, ino.i_gname) == 0) {//group
		mode = 3;
	}
	if (strcmp(Cur_User_Name, ino.i_uname) == 0) {//owner
		mode = 6;
	}
	if (((ino.inode_mode >> mode >> 1) & 1 == 0) || (strcmp(Cur_User_Name, "root") != 0)) {//�Ƿ��д��2
		printf("û��Ȩ��ɾ�����ļ�\n");
		return false;
	}

	//ɾ��������
	for (int i = 0; i < 10; ++i) {
		if (ino.i_dirBlock[i] != -1) {//��ʹ�ù�
			//block����&inode��������
			bfree(ino.i_dirBlock[i]);
			ino.i_dirBlock[i] = -1;
		}
	}
	ino.inode_file_count = 0;
	ino.inode_file_size =0;
	ifree(CHIAddr);
	return true;
}

bool writefile(inode fileinode, int iaddr, char buf[]) { //�ļ�д�루��д��ok
	//ǰ�᣺�����ǰ���block˳��洢
	if ((fileinode.inode_file_size + strlen(buf)) > 10 * BLOCK_SIZE) {
		printf("�ļ��ڴ治�㣬�޷������������\n");
		return false;
	}

	fileinode.inode_file_size += strlen(buf);
	fseek(fw, iaddr, SEEK_SET);
	fwrite(&fileinode, sizeof(inode), 1, fw);

	//д���ļ�
	int start = fileinode.inode_file_size / BLOCK_SIZE; //ʹ�õ��ڼ��飨����block[0])
	char temp[BLOCK_SIZE];
	for (int i = start; i < 10; ++i) {
		if (fileinode.i_dirBlock[i] != -1) {	//����ʹ�ÿ�(��ȫ��
			fseek(fr, fileinode.i_dirBlock[i], SEEK_SET);
			fread(temp, BLOCK_SIZE, 1, fr);
			fflush(fr);
			int offset = BLOCK_SIZE - strlen(temp);
			strncat(temp, buf, offset);
			fseek(fw, fileinode.i_dirBlock[i], SEEK_SET);
			fwrite(temp, BLOCK_SIZE, 1, fw);
			fflush(fw);
			if (strlen(buf) > offset) {
				buf = buf + offset;
			}
			else {
				break;
			}
			
		}
		else {
			int baddr = balloc();
			if (baddr == -1) return false;
			fileinode.i_dirBlock[i] = baddr;
			fseek(fw, fileinode.i_dirBlock[i], SEEK_SET);
			fwrite(buf, BLOCK_SIZE, 1, fw);
			fflush(fw);
			if (strlen(buf) > BLOCK_SIZE) {	//����û�����
				buf += BLOCK_SIZE;
			}
			else {
				break;
			}
		}
	}

	fflush(fw);
	return true;
}
void cd(int PIAddr, char name[]) {//�л�Ŀ¼(ok
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
			fread(ditem, sizeof(ditem), 1, fr);
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
						*p = '\0'; //�����
						Cur_Dir_Addr = PIAddr;
					}
					inode chiino;
					fseek(fr, ditem[j].inodeAddr, SEEK_SET);
					fread(&chiino, sizeof(inode), 1, fr);
					if (((chiino.inode_mode >> role) & 1) == 1) {	//�Ƿ���ִ��Ȩ��
						if (strcmp(Cur_Dir_Name, "/") != 0) {
							strcat(Cur_Dir_Name, "/");
						}
						strcat(Cur_Dir_Name, name);
						Cur_Dir_Addr = ditem[j].inodeAddr;
					}
				}
			}
		}
	}
	fflush(fr);
	return;
}
void gotoRoot() { //ok
	Cur_Dir_Addr= Root_Dir_Addr;
	strcpy(Cur_Dir_Name , "/");
}
void ls() {//��ʾ��ǰĿ¼�����ļ�
	inode ino;
	fseek(fr, Cur_Dir_Addr, SEEK_SET);
	fread(&ino, sizeof(inode), 1, fr);
	
	//�鿴Ȩ��
	int mode = 0;//other
	if (strcmp(Cur_Group_Name, ino.i_gname) == 0) {//group
		mode = 3;
	}
	if (strcmp(Cur_User_Name, ino.i_uname) == 0) {//owner
		mode = 6;
	}
	if (((ino.inode_mode >> mode >> 2) & 1 == 0) || (strcmp(Cur_User_Name, "root") != 0)) {//�Ƿ�ɶ���4
		printf("û��Ȩ�޲鿴���ļ���\n");
		return;
	}

	for (int i = 0; i < 10; ++i) {
		DirItem ditem[DirItem_Size];
		if (ino.i_dirBlock[i] != -1) {//��ʹ�ù�
			fseek(fr, ino.i_dirBlock[i], SEEK_SET);
			fread(ditem, sizeof(BLOCK_SIZE), 1, fr);
			for (int j = 0; j < DirItem_Size; ++j) {
				if (strlen(ditem[j].itemName) != 0) {
					printf("%s  ", ditem[i].itemName);
				}
			}
		}
	}
	return;
}

//****���ߺ���****
int ialloc() { //����inode�����˷���-1 ok
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
	iaddr =Inode_Start_Addr + iaddr * INODE_SIZE;
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
int balloc() { //����block�����˷���-1 ok
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
	baddr = Block_Start_Addr + baddr * BLOCK_SIZE;
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

//****�û�&�û��麯��****
void inUsername(char* username)	//�����û���
{
	printf("username:\n");
	scanf("%s", username);	//�û���
}
void inPasswd(char *passwd)	//��������
{
	printf("password:\n");
	scanf("%s", passwd);
}
bool login()	//��½����
{
	char username[100] = { 0 };
	char passwd[100] = { 0 };
	inUsername(username);	//�����û���
	inPasswd(passwd);		//�����û�����
	if(check(username,passwd)){
		isLogin = true;
		return true;
	}
	else {
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
	
	//�����û�Ŀ¼
	gotoRoot();
	cd(Cur_Dir_Addr, "home");
	mkdir(Cur_Dir_Addr, username);
	//�ָ��ֳ�
	strcpy(Cur_User_Name, pro_cur_user_name);
	strcpy(Cur_User_Dir_Name, pro_cur_user_dir_name);
	strcpy(Cur_Group_Name, pro_cur_group_name);

	//��ȡetc���ļ�
	inode etcino,shadowino,passwdino,groupino;
	int shadowiddr, passwdiddr,groupiddr;
	gotoRoot();
	cd(Cur_Dir_Addr, "etc");
	fseek(fr, Cur_Dir_Addr, SEEK_SET);
	fread(&etcino, sizeof(inode), 1, fr);
	for (int i = 0; i < 10; ++i) {
		DirItem ditem[DirItem_Size];
		int baddr = etcino.i_dirBlock[i];
		if (baddr != -1) {
			fseek(fr, baddr, SEEK_SET);
			fread(&ditem, BLOCK_SIZE, 1, fr);
			for (int j = 0; j < DirItem_Size; ++j) {
				if (strcmp(ditem[j].itemName, "passwd") == 0) {
					passwdiddr = ditem[j].inodeAddr;
					fseek(fr, passwdiddr, SEEK_SET);
					fread(&passwdino, sizeof(inode), 1, fr);
				}
				if (strcmp(ditem[j].itemName, "shadow") == 0) {	//���ж��Ƿ�Ϊ�ļ���
					shadowiddr = ditem[j].inodeAddr;
					fseek(fr, shadowiddr, SEEK_SET);
					fread(&shadowino, sizeof(inode), 1, fr);
				}
				if (strcmp(ditem[j].itemName, "group") == 0) {
					groupiddr = ditem[j].inodeAddr;
					fseek(fr,groupiddr, SEEK_SET);
					fread(&groupino, sizeof(inode), 1, fr);
				}
			}
		}
	}

	//��ȡ���ļ����ݲ��޸����ļ�
	char buf[BLOCK_SIZE * 10]; //1char:1B
	char temp[BLOCK_SIZE];
	int g = -1;
	if (strcmp(group, "root")==0) {
		g = 0;
	}
	else if (strcmp(group, "teacher")==0) {
		g = 1;
	}
	else if (strcmp(group, "student")==0) {
		g = 2;
	}
	else {
		printf("�û������ȷ������������");
		return false;
	}

	//passwd
	memset(buf, '\0', sizeof(temp));
	for (int i = 0; i < 10; ++i) {
		if (passwdino.i_dirBlock[i] != -1) {
			memset(temp, '\0', sizeof(temp));
			fseek(fr, passwdino.i_dirBlock[i], SEEK_SET);
			fread(&temp, BLOCK_SIZE, 1, fr);//��֪���ܷ�ɹ�
			strcpy(buf, temp);
		}
	}
	//buf[strlen(buf)] = '\0'; (strcat���ܻ��Զ���ӣ���
	if (strstr(buf, username)!= NULL) {
		printf("���û����Ѵ���\n");
		return false;
	}
	sprintf(buf + strlen(buf), "%s:%d:%d\n", username, nextUID++, g);
	passwdino.inode_file_size = strlen(buf);
	writefile(passwdino, passwdiddr, buf);

	//shadow
	memset(buf, '\0', sizeof(temp));
	for (int i = 0; i < 10; ++i) {
		if (shadowino.i_dirBlock[i] != -1) {
			memset(temp, '\0', sizeof(temp));
			fseek(fr, shadowino.i_dirBlock[i], SEEK_SET);
			fread(&temp, BLOCK_SIZE, 1, fr);//��֪���ܷ�ɹ�
			strcat(buf, temp);
		}
	}
	sprintf(buf + strlen(buf), "%s:%s\n", username, passwd);
	shadowino.inode_file_size = strlen(buf);
	writefile(shadowino, shadowiddr, buf);

	//group(root:0:XX,XX)
	memset(buf, '\0', sizeof(temp));
	for (int i = 0; i < 10; ++i) {
		if (groupino.i_dirBlock[i] != -1) {
			memset(temp, '\0', sizeof(temp));
			fseek(fr, groupino.i_dirBlock[i], SEEK_SET);
			fread(&temp, BLOCK_SIZE, 1, fr);//��֪���ܷ�ɹ�
			strcat(buf, temp);
		}
	}
	//ƴ��״����
	if (g == 0) {	//root
		char* p = strstr(buf, "teacher");
		char temp[strlen(p)];
		strncpy(temp, p, strlen(p));
		*p = '0';
		if (buf[strlen(buf) - 2] == ':') {
			sprintf(buf + strlen(buf) - 1, "%s\n", username);
		}
		else {
			sprintf(buf + strlen(buf) - 1, ",%s\n", username);
		}
		strcat(buf, temp);
		groupino.inode_file_size=strlen(buf);
		writefile(groupino, groupiddr, buf);
	}
	else if (g == 1) {//teacher
		char* p = strstr(buf, "student");
		char temp[strlen(p)];
		strncpy(temp, p, strlen(p));
		*p = '0';
		if (buf[strlen(buf) - 2] == ':') {
			sprintf(buf + strlen(buf) - 1, "%s\n", username);
		}
		else {
			sprintf(buf + strlen(buf) - 1, ",%s\n", username);
		}
		strcat(buf, temp);
		groupino.inode_file_size = strlen(buf);
		writefile(groupino, groupiddr, buf);
	}
	else {//student
		if (buf[strlen(buf) - 2] == ':') {
			sprintf(buf + strlen(buf) - 1, "%s\n", username);
		}
		else {
			sprintf(buf + strlen(buf) - 1, ",%s\n", username);
		}
		groupino.inode_file_size = strlen(buf);
		writefile(groupino, groupiddr, buf);
	}

	Cur_Dir_Addr = pro_cur_dir_addr;
	strcpy(Cur_Dir_Name, pro_cur_dir_name);
	return true;
}
bool userdel(char username[]) {	//�û�ɾ��
	if (strcmp(Cur_Dir_Name, "root") != 0) {
		printf("Ȩ�޲��㣬�޷�ɾ���û�\n");
		return false;
	}
	if (strcmp(username, "root") == 0) {
		printf("�޷�ɾ������Ա\n");
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
	strcpy(Cur_Group_Name, "");

	//��ȡetc���ļ�
	inode etcino, shadowino, passwdino, groupino;
	int shadowiddr, passwdiddr, groupiddr;
	gotoRoot();
	cd(Cur_Dir_Addr, "etc");
	fseek(fr, Cur_Dir_Addr, SEEK_SET);
	fread(&etcino, sizeof(inode), 1, fr);
	for (int i = 0; i < 10; ++i) {
		DirItem ditem[DirItem_Size];
		int baddr = etcino.i_dirBlock[i];
		if (baddr != -1) {
			fseek(fr, baddr, SEEK_SET);
			fread(&ditem, BLOCK_SIZE, 1, fr);
			for (int j = 0; j < DirItem_Size; ++j) {
				if (strcmp(ditem[j].itemName, "passwd") == 0) {
					passwdiddr = ditem[j].inodeAddr;
					fseek(fr, passwdiddr, SEEK_SET);
					fread(&passwdino, sizeof(inode), 1, fr);
				}
				if (strcmp(ditem[j].itemName, "shadow") == 0) {	//���ж��Ƿ�Ϊ�ļ���
					shadowiddr = ditem[j].inodeAddr;
					fseek(fr, shadowiddr, SEEK_SET);
					fread(&shadowino, sizeof(inode), 1, fr);
				}
				if (strcmp(ditem[j].itemName, "group") == 0) {
					groupiddr = ditem[j].inodeAddr;
					fseek(fr, groupiddr, SEEK_SET);
					fread(&groupino, sizeof(inode), 1, fr);
				}
			}
		}
	}

	//��ȡ���ļ����ݲ��޸����ļ�
	char buf[BLOCK_SIZE * 10]; //1char:1B
	char temp[BLOCK_SIZE];

	//passwd
	for (int i = 0; i < 10; ++i) {
		if (passwdino.i_dirBlock[i] != -1) {
			memset(temp, '\0', sizeof(temp));
			fseek(fr, passwdino.i_dirBlock[i], SEEK_SET);
			fread(&temp, BLOCK_SIZE, 1, fr);//��֪���ܷ�ɹ�
			strcat(buf, temp);
		}
	}
	//buf[strlen(buf)] = '\0'; (strcat���ܻ��Զ���ӣ���
	char* p = strstr(buf, username);
	if (strstr(buf, username) == NULL) {
		printf("���û��������ڣ��޷�ɾ��\n");
		return false;
	}
	*p = '\0';
	while ((*p) != '\n') {
		p++;
	}
	p++;
	strcat(buf, p);
	passwdino.inode_file_size = strlen(buf);
	writefile(passwdino, passwdiddr, buf);

	//shadow
	memset(buf, '\0', sizeof(temp));
	for (int i = 0; i < 10; ++i) {
		if (shadowino.i_dirBlock[i] != -1) {
			memset(temp, '\0', sizeof(temp));
			fseek(fr, shadowino.i_dirBlock[i], SEEK_SET);
			fread(&temp, BLOCK_SIZE, 1, fr);//��֪���ܷ�ɹ�
			strcat(buf, temp);
		}
	}
	p = strstr(buf, username);
	*p = '\0';
	while ((*p) != '\n') {
		p++;
	}
	p++;
	strcat(buf, p);
	shadowino.inode_file_size = strlen(buf);
	writefile(shadowino, shadowiddr, buf);

	//group(root:0:XX,XX)
	memset(buf, '\0', sizeof(temp));
	for (int i = 0; i < 10; ++i) {
		if (groupino.i_dirBlock[i] != -1) {
			memset(temp, '\0', sizeof(temp));
			fseek(fr, groupino.i_dirBlock[i], SEEK_SET);
			fread(&temp, BLOCK_SIZE, 1, fr);//��֪���ܷ�ɹ�
			strcat(buf, temp);
		}
	}
	p = strstr(buf, username);
	if ((*(p - 1)) == ':') {	//��һ��������ո�Ͷ��Ŷ�Ҫȥ��
		*p = '/0';
		while ((*p) != '\n'||(*p)!=',') { 
			p++;
		}
		p++;
	}
	else {	//���ǵ�һ����ǰ�涺��Ҫȥ��
		p = p - 1;
		*p = '\0';
		while ((*p) != '\n' || (*p) != ',') {
			p++;
		}
	}
	strcat(buf, p);
	groupino.inode_file_size = strlen(buf);
	writefile(groupino, groupiddr, buf);

	Cur_Dir_Addr = pro_cur_dir_addr;
	strcpy(Cur_Dir_Name, pro_cur_dir_name);

	//ɾ�����û���Ӧ�ļ��У�����
	return true;
}
bool check(char username[], char passwd[]) {//������ݵ�¼&���� ok
	//��ȡ���ļ�
	inode etcino, shadowino, passwdino, groupino;
	int shadowiddr, passwdiddr, groupiddr;
	gotoRoot();
	cd(Cur_Dir_Addr, "etc");
	fseek(fr, Cur_Dir_Addr, SEEK_SET);
	fread(&etcino, sizeof(inode), 1, fr);
	for (int i = 0; i < 10; ++i) {
		DirItem ditem[DirItem_Size];
		int baddr = etcino.i_dirBlock[i];
		if (baddr != -1) {
			fseek(fr, baddr, SEEK_SET);
			fread(&ditem, BLOCK_SIZE, 1, fr);
			for (int j = 0; j < DirItem_Size; ++j) {
				if (strcmp(ditem[j].itemName, "passwd") == 0) {
					passwdiddr = ditem[j].inodeAddr;
					fseek(fr, passwdiddr, SEEK_SET);
					fread(&passwdino, sizeof(inode), 1, fr);
				}
				if (strcmp(ditem[j].itemName, "shadow") == 0) {	//���ж��Ƿ�Ϊ�ļ���
					shadowiddr = ditem[j].inodeAddr;
					fseek(fr, shadowiddr, SEEK_SET);
					fread(&shadowino, sizeof(inode), 1, fr);
				}
				if (strcmp(ditem[j].itemName, "group") == 0) {
					groupiddr = ditem[j].inodeAddr;
					fseek(fr, groupiddr, SEEK_SET);
					fread(&groupino, sizeof(inode), 1, fr);
				}
			}
		}
	}
	fflush(fr);
	//��ȡ���ļ����ݲ��޸����ļ�
	char buf[BLOCK_SIZE * 10]; //1char:1B
	char temp[BLOCK_SIZE];
	char checkpw[100];
	char group[10];


	//shadow
	memset(buf, '\0', sizeof(temp));
	for (int i = 0; i < 10; ++i) {
		if (shadowino.i_dirBlock[i] != -1) {
			memset(temp, '\0', sizeof(temp));
			fseek(fr, shadowino.i_dirBlock[i], SEEK_SET);
			fread(&temp, BLOCK_SIZE, 1, fr);//��֪���ܷ�ɹ�
			strcpy(buf, temp);
		}
	}
	char* p = strstr(buf, username);
	if (p == NULL) {
		printf("���û������ڡ��봴���û������µ�½.\n");
		return false;
	}
	while ((*p) != ':') {
		p++;
	}
	p++;
	int i = 0;
	while ((*p) != '\n') {
		checkpw[i++] = (*p);
		p++;
	}
	if (strcmp(checkpw, passwd) != 0) {
		printf("���벻��ȷ�������³��ԣ�\n");
		return false;
	}

	//passwd
	memset(buf, '\0', sizeof(temp));
	for (int i = 0; i < 10; ++i) {
		if (passwdino.i_dirBlock[i] != -1) {
			memset(temp, '\0', sizeof(temp));
			fseek(fr, passwdino.i_dirBlock[i], SEEK_SET);
			fread(&temp, BLOCK_SIZE, 1, fr);//��֪���ܷ�ɹ�
			strcpy(buf, temp);
		}
	}
	p = strstr(buf, username);
	int flag;
	i=flag = 0;
	
	memset(group, '\0', strlen(group));
	while ((*p) != '\n') {
		if (flag == 2) {
			group[i++] = (*p);
		}
		if ((*p) == ':') {
			flag++;
		}
		p++;
	}

	//�ɹ���¼�������
	if(strcmp(group,"0")==0){
		strcpy(Cur_Group_Name, "root");
	}
	else if (strcmp(group, "1") == 0) {
		strcpy(Cur_Group_Name, "teacher");
	}
	else if (strcmp(group, "2") == 0) {
		strcpy(Cur_Group_Name, "student");
	}
	strcpy(Cur_User_Name,username);
	sprintf(Cur_User_Dir_Name, "/home/%s", username);
	gotoRoot();
	cd(Cur_Dir_Addr, "home");
	cd(Cur_Dir_Addr, username);

	return true;
}
bool chmod(int PIAddr, char name[], int pmode,int type) {//�޸��ļ�orĿ¼Ȩ�ޣ��ٶ��ļ���Ŀ¼Ҳ����������
	if (strlen(name) > FILENAME_MAX) {
		printf("�ļ����Ƴ�����󳤶�\n");
		return false;
	}
	if (strcmp(name, ".") ==0|| strcmp(name, "..")==0) {
		printf("���ļ��޷��޸�Ȩ��\n");
		return false;
	}
	inode ino;
	fseek(fr, PIAddr, SEEK_SET);
	fread(&ino, sizeof(inode), 1, fr);
	fflush(fr);
	for (int i = 0; i < 10; ++i) {
		DirItem ditem[DirItem_Size];
		fseek(fr, ino.i_dirBlock[i], SEEK_SET);
		fread(ditem, sizeof(ditem), 1, fr);
		for (int j = 0; j < DirItem_Size; ++j) {
			if (strcmp(ditem[j].itemName, name)==0) {//�ҵ�ͬ���ļ�
				inode chiino;
				fseek(fr, ditem[j].inodeAddr, SEEK_SET);
				fread(&chiino, sizeof(inode), 1, fr);
				fflush(fr);
				//1:Ŀ¼ 0���ļ�
				if (((chiino.inode_mode >> 9) & 1 )!= type) {	//δ�ҵ�ͬһ�����ļ�
					continue;
				}
				//ֻ�д����ߺ͹���Ա���Ը���Ȩ��
				if ((strcmp(chiino.i_uname, Cur_User_Name) == 0) || strcmp(Cur_User_Name, "root")==0) {
					chiino.inode_mode = (chiino.inode_mode >> 9 << 9) | pmode;
					fseek(fr, ditem[j].inodeAddr, SEEK_SET);
					fwrite(&chiino, sizeof(inode), 1, fr);
					fflush(fw);
					return true;
				}
			}
		}
	}
	printf("û���ҵ����ļ����޷��޸�Ȩ��\n");
	return false;
}

