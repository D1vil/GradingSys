#include<iostream>
#include<stdio.h>
#include<time.h>
#include<string>
#include<ctime>
#include"snapshot.h"
#include"os.h"

using namespace std;


bool fullBackup() {
	time_t cur_time;
	char backupSysName[256] = { 0 };
	time(&cur_time);
	strftime(backupSysName, sizeof(backupSysName), "FullBackupSys %Y.%m.%d %H-%M-%S.sys", localtime(&cur_time));

	if ((bfr = fopen(backupSysName, "rb")) == NULL) {
		bfw = fopen(backupSysName, "wb");
		if (bfw == NULL) {
			printf("�����ļ���ʧ�ܣ�");
			return false;
		}
		bfr = fopen(backupSysName, "rb");
		printf("�����ļ��򿪳ɹ�");
	}
	else {
		bfw = fopen(backupSysName, "rb+");
		if (bfw == NULL) {
			printf("�����ļ���ʧ��");
			return false;
		}
	}
	//�ѱ��ݵĿռ��ʼ��
	char backup_buf[10000000];
	memset(backup_buf, '\0', sizeof(backup_buf));
	fseek(bfw, Start_Addr, SEEK_SET);
	fwrite(backup_buf, sizeof(backup_buf), 1, bfw);

	try {
		char tmp_backup[10000000];
		fseek(fr, Start_Addr, SEEK_SET);
		fread(&tmp_backup, sizeof(tmp_backup), 1, fr);
		fseek(bfw, Start_Addr, SEEK_SET);
		fwrite(&tmp_backup, sizeof(tmp_backup), 1, bfw);
	}
	catch (exception e) {
		printf("%s\n", e.what());
		return false;
	}
	//���λͼ���
	char tmp_inodeBitmap[INODE_NUM];
	for (int i = 0; i < INODE_NUM; i++) {
		tmp_inodeBitmap[i] = 0;
	}
	fseek(fw, Modified_inodeBitmap_Start_Addr, SEEK_SET);
	fwrite(&tmp_inodeBitmap, sizeof(tmp_inodeBitmap), 1, fw);

	time_t curtime;
	time(&curtime);
	last_backup_time = curtime;
	return true;
}

//����ת��
bool incrementalBackup() {
	//�����µ�sys�ļ�
	time_t cur_time;
	char backupSysName[256] = { 0 };
	time(&cur_time);
	strftime(backupSysName, sizeof(backupSysName), "IncreBackupSys %Y.%m.%d %H-%M-%S.sys", localtime(&cur_time));

	if ((bfr = fopen(backupSysName, "rb")) == NULL) {
		bfw = fopen(backupSysName, "wb");
		if (bfw == NULL) {
			printf("�����ļ���ʧ�ܣ�");
			return false;;
		}
		bfr = fopen(backupSysName, "rb");
		printf("�����ļ��򿪳ɹ�");
	}
	else {
		bfw = fopen(backupSysName, "rb+");
		if (bfw == NULL) {
			printf("�����ļ���ʧ��");
			return false;
		}
	}

	char tmp_inodeBitmap[INODE_NUM];
	memset(tmp_inodeBitmap, '\0', sizeof(tmp_inodeBitmap));
	fseek(fr, Modified_inodeBitmap_Start_Addr, SEEK_SET);
	fread(&tmp_inodeBitmap, sizeof(tmp_inodeBitmap), 1, fr);

	//��ÿһ���޸Ĺ����ļ�&ȫ��Ŀ¼��λͼ�������
	for (int i = 0; i < INODE_NUM; i++) {
		inode tmp_inode;
		fseek(fr, Inode_Start_Addr, SEEK_SET);
		fread(&tmp_inode, sizeof(inode), 1, fr);
		if (tmp_inode.file_modified_time > last_backup_time) {
			tmp_inodeBitmap[i] = 1;
		}
		else {
			continue;
		}
	}

	fseek(fw, Modified_inodeBitmap_Start_Addr, SEEK_SET);
	fwrite(&tmp_inodeBitmap, sizeof(modified_inode_bitmap), 1, fw);
	
	//�ڱ����ļ����ȼ�¼modified inode bitmap
	fseek(bfw, Start_Addr, SEEK_SET);
	fwrite(&tmp_inodeBitmap, sizeof(modified_inode_bitmap), 1, bfw);

	//ȥÿһ�����޸ĵ�inode���ұ���Ӧ�޸ĵ�ֱ�ӿ�
	int Cur_Addr=Backup_Block_Start_Addr;
	for (int i = 0; i < INODE_NUM; i++) {
		inode tmp_inode;
		if (modified_inode_bitmap[i] == 1) {
			char tmp[128];
			memset(tmp, '\0', sizeof(tmp));
			//���޸Ĺ���inode
			fseek(fr, Inode_Start_Addr, SEEK_SET);
			fread(&tmp_inode, sizeof(inode), 1, fr);
			//���޸Ĺ���inodeд����������ϵͳ
			fseek(bfw, Cur_Addr, SEEK_SET);
			fwrite(&tmp_inode, sizeof(inode), 1, bfw);
			Cur_Addr += sizeof(inode);
			//��inodeֱ�ӿ�
			int tmp_i_dirBlock[10];
			for (int j = 0; j < 10; j++) {
				if (tmp_inode.i_dirBlock[j] != -1) {
					//��ֱ�ӿ�
					char tmp_block[512];
					int block_addr = tmp_inode.i_dirBlock[j];
					fseek(fr, block_addr, SEEK_SET);
					fread(&tmp_block, sizeof(tmp_block), 1, fr);
					fseek(bfw, Cur_Addr, SEEK_SET);
					fwrite(&tmp_block, sizeof(tmp_block), 1, bfw);
					Cur_Addr += sizeof(tmp_block);;
				}
				else {
					continue;
				}
			}

		}
	}
	time_t curtime;
	time(&curtime);
	last_backup_time = curtime;
	return true;
}

bool recovery() {

}