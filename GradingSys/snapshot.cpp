#include<iostream>
#include<stdio.h>
#include<time.h>
#include<string>
#include<ctime>
#include"snapshot.h"
#include"os.h"

using namespace std;

void Initial() {
	time_t cur_time;
	char backupSysName[256] = { 0 };
	time(&cur_time);
	strftime(backupSysName, sizeof(backupSysName), "%Y.%m.%d %H-%M-%S backup.sys", localtime(&cur_time));
	
	if ((bfr = fopen(backupSysName, "rb")) == NULL) {
		bfw = fopen(backupSysName, "wb");
		if (bfw == NULL) {
			printf("�����ļ���ʧ�ܣ�");
			return;
		}
		bfr = fopen(backupSysName, "rb");
		printf("�����ļ��򿪳ɹ�");
	}
	else {
		bfw = fopen(backupSysName, "rb+");
		if (bfw == NULL) {
			printf("�����ļ���ʧ��");
			return;
		}
	}
	//�ѱ��ݵĿռ��ʼ��
	char backup_buf[10000000];
	memset(backup_buf, '\0', sizeof(backup_buf));
	fseek(bfw, Start_Addr, SEEK_SET);
	fwrite(backup_buf, sizeof(backup_buf), 1, bfw);
	return;
}

bool fullBackup() {
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
	return true;
}

//����ת��
bool incrementalBackup() {
	//��ÿһ���޸Ĺ����ļ�&ȫ��Ŀ¼��λͼ�������

	//�ҵ�Ҫ��ת����inode

	//
}