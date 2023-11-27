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
	int plen = 0;
	char c;
	fflush(stdin);	//��ջ�����
	printf("passwd:");
	while (c = getchar()) {
		if (c == '\r') {	//����س�������ȷ��
			passwd[plen] = '\0';
			fflush(stdin);	//�建����
			printf("\n");
			break;
		}
		else if (c == '\b') {	//�˸�ɾ��һ���ַ�
			if (plen != 0) {	//û��ɾ��ͷ
				plen--;
			}
		}
		else {	//�����ַ�
			passwd[plen++] = c;
		}
	}
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
