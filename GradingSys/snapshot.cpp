#include"snapshot.h"
#include"os.h"
#include<iostream>
#include<stdio.h>
#include<time.h>
#include<vector>
#include<string>
#include<fstream>
#include<ctime>
#include<dirent.h>

using namespace std;

bool demo() {
	time_t cur_time;
	time(&cur_time);
	//string bSysName;
	char backupSysName[256];
	memset(backupSysName, '\0', sizeof(backupSysName));
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

	char backup_buf[10000000];
	memset(backup_buf, '\0', sizeof(backup_buf));

	fseek(fr, 0, SEEK_SET);
	fread(&backup_buf, sizeof(backup_buf), 1, fr);
	fseek(bfw, 0, SEEK_SET);
	fwrite(&backup_buf, sizeof(backup_buf), 1, bfw);
	return true;
}

bool fullBackup() {
	time_t cur_time;
	time(&cur_time);
	//string bSysName;
	char backupSysName[256];
	memset(backupSysName, '\0', sizeof(backupSysName));
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
	
	fseek(fr, 0, SEEK_SET);
	fread(&backup_buf, sizeof(backup_buf), 1, fr);
	fseek(bfw, 0, SEEK_SET);
	fwrite(&backup_buf, sizeof(backup_buf), 1, bfw);
	

	//���λͼ���
	char tmp_inodeBitmap[INODE_NUM];
	memset(tmp_inodeBitmap, 0, sizeof(tmp_inodeBitmap));
	for (int i = 0; i < INODE_NUM; i++) {
		tmp_inodeBitmap[i] = 0;
	}
	fseek(fw, Modified_inodeBitmap_Start_Addr, SEEK_SET);
	fwrite(&tmp_inodeBitmap, sizeof(tmp_inodeBitmap), 1, fw);

	time_t nowtime;
	time(&nowtime);
	last_backup_time = nowtime;
	return true;
}

//����ת��
bool incrementalBackup() {
	//�����µ�sys�ļ�
	time_t cur_time;
	char backupSysName[256] = { 0 };
	time(&cur_time);
	strftime(backupSysName, sizeof(backupSysName), "IncreBackupSys %Y.%m.%d %H-%M-%S.sys", localtime(&cur_time));
	string str(backupSysName);
	printf("%s", backupSysName);
	if ((bfr = fopen(backupSysName, "rb")) == NULL) {
		bfw = fopen(backupSysName, "wb");
		if (bfw == NULL) {
			printf("BackupSys File error��");
			return false;;
		}
		bfr = fopen(backupSysName, "rb");
		printf("BackupSys File successful open");
	}
	else {
		bfw = fopen(backupSysName, "rb+");
		if (bfw == NULL) {
			printf("BackupSys File error��");
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
	fseek(bfw, Backup_Start_Addr, SEEK_SET);
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
					memset(tmp_block, '\0', sizeof(tmp_block));
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
	//��ȡ��ǰ�ļ����µ������ļ�
	DIR* dir;
	struct dirent* ent;
	//�ָ�ȫ������
	if ((dir = opendir("./")) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			if (strcmp(ent->d_name, "Full") == 0) {
				//���ļ�
				if ((bfr = fopen(ent->d_name, "rb")) == NULL) {
					bfw = fopen(ent->d_name, "wb");
					if (bfw == NULL) {
						printf("�����ļ���ʧ�ܣ�");
						return false;;
					}
					bfr = fopen(ent->d_name, "rb");
					printf("�����ļ��򿪳ɹ�");
				}
				else {
					bfw = fopen(ent->d_name, "rb+");
					if (bfw == NULL) {
						printf("�����ļ���ʧ��");
						return false;
					}
				}
				char buffer[10000000];
				memset(buffer, '\0', sizeof(buffer));
				fseek(bfr, Backup_Start_Addr, SEEK_SET);
				fread(&buffer, sizeof(buffer), 1, bfr);
				fseek(fw, Superblock_Start_Addr, SEEK_SET);
				fwrite(&buffer, sizeof(buffer), 1, fw);
				break;
			}

		}
	}
	//����������ת��
	//ͳ���ж��ٸ�����ת��
	vector<string> files;
	int incre_backup_count = 0;
	if ((dir = opendir("./")) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			if (strcmp(ent->d_name, "Incre") == 0) {
				incre_backup_count++;
				files.push_back(ent->d_name);
			}
		}
	}
	//���޸�ʱ����絽�����򣨴�С����
	for (int m = 0; m < files.size() - 1; ++m) {
		for (int n = 0; n < files.size() - 1; ++n) {
			if (strcmp(files[n].c_str(), files[n + 1].c_str()) > 0) {
				string tmp = files[n];
				files[n] = files[n + 1];
				files[n + 1] = tmp;
			}
		}
	}

	//���絽��˳�������ת�����ļ�
	for (int i = 0; i < incre_backup_count; i++) {
		//�򿪱����ļ�
		if ((bfr = fopen(files[i].c_str(), "rb")) == NULL) {
			bfw = fopen(files[i].c_str(), "wb");
			if (bfw == NULL) {
				printf("�����ļ���ʧ�ܣ�");
				return false;;
			}
			bfr = fopen(files[i].c_str(), "rb");
			printf("�����ļ��򿪳ɹ�");
		}
		else {
			bfw = fopen(files[i].c_str(), "rb+");
			if (bfw == NULL) {
				printf("�����ļ���ʧ��");
				return false;
			}
		}

		int Cur_Addr = 0;
		//ȡ�����ļ��е�inodeλͼ
		char tmp_bitmap[1024];
		memset(tmp_bitmap, '\0', sizeof(tmp_bitmap));
		fseek(bfr, Cur_Addr, SEEK_SET);
		fread(&tmp_bitmap, sizeof(tmp_bitmap), 1, bfr);
		Cur_Addr += sizeof(tmp_bitmap);
		for (int k = 0; k < INODE_NUM; k++) {
			if (tmp_bitmap[k] == 1) {
				inode tmp;
				//��inode�����������ļ�ϵͳ�ж���
				fseek(bfr, Cur_Addr, sizeof(inode));
				fread(&tmp, sizeof(inode), 1, bfr);
				Cur_Addr += sizeof(inode);
				//��inodeд��ԭ�ļ�ϵͳ
				fseek(fw, Inode_Start_Addr + k * sizeof(inode), SEEK_SET);
				fwrite(&tmp, sizeof(inode), 1, fw);
				//����ֱ�ӿ�
				for (int i = 0; i < 10; i++) {
					if (tmp.i_dirBlock[i] != -1) {
						//�ѱ����ļ��е�ֱ�ӿ����
						char tmp_block[512];
						memset(tmp_block, '\0', sizeof(tmp_block));
						fseek(bfr, Cur_Addr, SEEK_SET);
						fread(&tmp_block, sizeof(tmp_block), 1, bfr);
						Cur_Addr += sizeof(tmp_block);
						//д��Դ�ļ���Ӧ���ַ
						fseek(fw, tmp.i_dirBlock[i], SEEK_SET);
						fwrite(&tmp_block, sizeof(tmp_block), 1, fw);
					}
				}
			}
		}
	}
	return true;
}