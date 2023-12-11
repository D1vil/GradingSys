#include"os.h"
#include"function.h"
#include<iomanip>
#include<time.h>
#include<string.h>
#include<stdio.h>
#include<iostream>

using namespace std;

void help() {
	cout.setf(ios::left); //���ö��뷽ʽΪleft 
	cout.width(30); //���ÿ�ȣ������ÿո���� 
	cout << "ls" << "Display the current directory listing" << endl;	//�г���ǰĿ¼�嵥(ls/ls -l)
	cout.width(30);
	cout << "cd" << "Enter the specific directory " << endl;		//ǰ��ָ��Ŀ¼(cd home)
	cout.width(30);
	cout << "gotoRoot" << "Return to the root directory " << endl;		//���ظ�Ŀ¼
	cout.width(30);
	cout << "mkdir" << "Create directory" << endl;					//����Ŀ¼
	cout.width(30);
	cout << "rm" << "Delete directory or file" << endl;					//ɾ��Ŀ¼���ļ�
	cout.width(30);
	cout << "touch" << "Create a blank file" << endl;			//�������ļ�
	cout.width(30);
	cout << "echo" << "Create a non-empty file" << endl;		//�����������ļ�
	cout.width(30);
	cout << "writefile" << "write a blank file" << endl;		//д���ļ�
	cout.width(30);
	cout << "addfile" << "continue to write file" << endl;		//��д�ļ�
	cout.width(30);
	cout << "chmod" << "Modify the access right" << endl;    //�޸��ļ�Ȩ��
	cout.width(30);
	//cat��chown

	cout << "useradd" << "Add user" << endl;		//�����û�
	cout.width(30);
	cout << "userdel" << "Delete user" << endl;		//ɾ���û�
	cout.width(30);

	cout << "logout" << "Logout the account" << endl;		//�˳��˺�
	cout.width(30);
	//usergrpadd,userfrpdel,�����޸ģ�


	cout << "snapshot" << "Back up the system" << endl;			//����ϵͳ
	cout.width(30);
	//����ϵͳ&�ָ�ϵͳ

	cout << "exit" << "Exit the system" << endl;		//�˳�ϵͳ
}

bool cd_func(int CurAddr, char* str) {	
	//cd����һ����·��or���·��
	//�����ֳ���ʧ�ָܻ���
	int pro_cur_dir_addr = Cur_Dir_Addr;
	char pro_cur_dir_name[310];
	strcpy(pro_cur_dir_name, Cur_Dir_Name);
	int flag = 1;
	
	//�鿴cd���ͣ�����·��or���·��
	if (strcmp(str, "/") == 0) {//ǰ����Ŀ¼
		gotoRoot();
		return true;
	}
	if (str[0] == '/') {	//����·��
		gotoRoot();
		str += 1;
	}
	while (strlen(str) != 0) {
		char name[sizeof(str)];
		int i = 0;
		memset(name, '\0', sizeof(name));
		while (((*str) != '/') && (strlen(str) != 0)) {
			name[i++] = str[0];
			str += 1;
		}
		if((*str) == '/') str += 1;
		if (strlen(name) != 0) {
			if (cd(Cur_Dir_Addr, name)==false) {
				flag = 0;
				break;
			}
		}
		else {
			break;
		}
	}

	//�ж��Ƿ�ɹ�
	if (flag == 0) {//ʧ�ܣ��ָ��ֳ�
		Cur_Dir_Addr = pro_cur_dir_addr;
		strcpy(Cur_Dir_Name, pro_cur_dir_name);
		return false;
	}
	return true;
}
bool mkdir_func(int CurAddr, char* str) {//������Ŀ¼�´���Ŀ¼
	//����,���,ֱ�Ӵ���
	char* p = strrchr(str, '/');
	if (p == NULL) {	//ֱ�Ӵ���
		if (mkdir(CurAddr, str))	return true;
		return false;
	}
	else {
		char name[File_Max_Size];
		memset(name, '\0', sizeof(name));
		p++;
		strcpy(name, p);
		*p = '\0';
		if (cd_func(CurAddr, str)) {
			if (mkdir(Cur_Dir_Addr, name))
				return true;
		}
		return false;
	}
}
bool mkfile_func(int CurAddr, char* str,char *buf) {//������Ŀ¼�´����ļ�
	//����,���,ֱ�Ӵ���
	char* p = strrchr(str, '/');
	if (p == NULL) {	//ֱ�Ӵ���
		if (mkfile(CurAddr, str,buf))	return true;
		return false;
	}
	else {
		char name[File_Max_Size];
		memset(name, '\0', sizeof(name));
		p++;
		strcpy(name, p);
		*p = '\0';
		if (cd_func(CurAddr, str)) {
			if (mkfile(Cur_Dir_Addr, name,buf))
				return true;
		}
		return false;
	}
}
bool rm_func(int CurAddr, char* str,char* s_type) {//������Ŀ¼��ɾ��
	//�ļ�����
	int type = -1;
	if (strcmp(s_type, "-rf") == 0) {
		type = 1;
	}
	else if (strcmp(s_type, "-f") == 0) {
		type = 0;
	}
	else {
		printf("�޷�ȷ���ļ�ɾ�����ͣ����������룡\n");
		return false;
	}

	//����,���,ֱ�Ӵ���
	char* p = strrchr(str, '/');
	if (p == NULL) {	//ֱ��ɾ��
		if (rm(CurAddr, str,type))	return true;
		return false;
	}
	else {
		char name[File_Max_Size];
		memset(name, '\0', sizeof(name));
		p++;
		strcpy(name, p);
		*p = '\0';
		if (cd_func(CurAddr, str)) {
			if (rm(Cur_Dir_Addr, name,type))
				return true;
		}
		return false;
	}
}

void cmd(char cmd_str[], int count) {
	char com1[100];
	char com2[100];
	char com3[100];
	char com4[100];
	sscanf(cmd_str, "%s", com1);
	//һ������com1��com2
	if (strcmp(com1, "help") == 0) {
		help();
	}
	else if (strcmp(com1, "ls") == 0) {
		sscanf(cmd_str, "%s%s", com1, com2);
		ls(com2);
	}
	else if (strcmp(com1, "cd") == 0) {
		sscanf(cmd_str, "%s%s", com1, com2);
		cd_func(Cur_Dir_Addr, com2);
	}
	else if (strcmp(com1, "gotoRoot") == 0) {
		gotoRoot();
	}
	else if (strcmp(com1, "mkdir") == 0) {	//cd����Ŀ¼--> mkdir
		sscanf(cmd_str, "%s%s", com1, com2);
		mkdir_func(Cur_Dir_Addr, com2);
	}
	else if (strcmp(com1, "rm") == 0) {
		sscanf(cmd_str, "%s%s%s", com1, com2,com3);
		rm_func(Cur_Dir_Addr,com3,com2);
	}
	else if (strcmp(com1, "touch") == 0) {
		sscanf(cmd_str, "%s%s", com1, com2);
		mkfile_func(Cur_Dir_Addr, com2, "");
	}
	else if (strcmp(com1, "echo") == 0) {
		sscanf(cmd_str, "%s%s%s%s", com1, com2,com3,com4);
		//ȥ������
		com2[-1] = '\0';
		mkfile_func(Cur_Dir_Addr, com4,com2+1);
	}
	//writefile&addfile��Ҫ��һ��
	//cat,chown
	//chmod

	else if (strcmp(com1, "useradd") == 0) {
		inUsername(com1);
		inPasswd(com2);
		ingroup(com3);
		useradd(com1, com2, com3);
	}
	else if (strcmp(com1, "userdel") == 0) {
		sscanf(cmd_str, "%s%s", com1, com2);
		userdel(com2);
	}
	else if (strcmp(com1, "logout") == 0) {
		logout();
	}
	//usergrpadd,userfrpdel,�����޸ģ�

	//����ϵͳ&�ָ�ϵͳ
	else if (strcmp(com1, "exit") == 0) {
		cout << "�˳��ɼ�����ϵͳ���ݰݣ�" << endl;
		exit(0);
	}

	return;
}