#include"os.h"
#include<iomanip>

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
	return true;
}

void inUsername(char username[])	//�����û���
{
	printf("username:");
	scanf("%s", username);	//�û���
}

void inPasswd(char passwd[])	//��������
{
	printf("passwd:");
	scanf("%s", passwd);
}

bool login()	//��½
{
	char username[100] = { 0 };
	char passwd[100] = { 0 };
	inUsername(username);	//�����û���
	inPasswd(passwd);		//�����û�����
	if (strcmp(username,"root")==0&& strcmp(passwd, "root")==0) {	//�˶��û���������
		isLogin = true;
		return true;
	}
	else {
		isLogin = false;
		return false;
	}
}

bool mkdir(int parinodeAddr, char name[]) {

}

void cmd(char cmd[]) {
	char com1[100];
	char com2[100];
	char com3[100];
	sscanf(cmd,"%s", com1);
	if (strcmp(com1, "ls") == 0) {
		ls(Cur_Dir_Addr);
	}
	else if (strcmp(com1, "mkdir") == 0) {
		cout << "in mkdir" << endl;
		sscanf(cmd, "%s%s", com1, com2);
		cout << com2 << endl;
		mkdir(Cur_Dir_Addr, com2);
	}
	return;
}

void ls(int parinodeAddr) {
	return;
}