#include<iostream>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include<fstream>
#include"function.h"
#include"os.h"
#include"root.h"
using namespace std;
bool add_users(char * namelist) { //root������������ʦ��ѧ���û�
	//��֤���
	if (strcmp(Cur_Group_Name, "root") != 0) {
		printf("������Ա��������ɾ�û�\n");
		return false;
	}
	//������Ϣ
	int pro_cur_dir_addr = Cur_Dir_Addr;
	char pro_cur_dir_name[310];
	memset(pro_cur_dir_name, '\0', sizeof(pro_cur_dir_name));
	strcpy(pro_cur_dir_name, Cur_Dir_Name);

	//��������
	ifstream fin(namelist);
	if (!fin.is_open()) {
		cout << "�޷�������" << endl;
		Cur_Dir_Addr = pro_cur_dir_addr;
		strcpy(Cur_Dir_Name, pro_cur_dir_name);
		return false;
	}
	string line;
	Relation relations[1000];
	int i = 0;
	char pwd[30];
	while (getline(fin, line)) {
		Relation rela;
		sscanf(line.c_str(), "%s:%s:%s", rela.student, rela.lesson, rela.teacher);
		relations[i++] = rela;
	}

	//������������
	//��������Ĭ��������ͬ��
	for (int j = 0; j < i; j++) {
		//�����û������ļ���
		strcpy(pwd, relations[j].student);
		useradd(relations[j].student, "student", pwd);
		strcpy(pwd, relations[j].teacher);
		useradd(relations[j].teacher, "teacher", pwd);
		//�����ļ����´�����Ӧ�γ��ļ���
		char dir_path[100];
		sprintf(dir_path, "/home/%s/%s", relations[j].teacher, relations[j].lesson);
		mkdir_func(Cur_Dir_Addr, dir_path);
		sprintf(dir_path, "/home/%s/%s", relations[j].student, relations[j].lesson);
		mkdir_func(Cur_Dir_Addr, dir_path);
	}
}

bool publish_task(char* lesson,char* filename) {//��ʦ������������ҵ����
	//��֤���
	if (strcmp(Cur_Group_Name, "teacher") != 0) {
		printf("ֻ����ʦ���Է�����ҵ!\n");
		return false;
	}

	//������Ϣ
	int pro_cur_dir_addr = Cur_Dir_Addr;
	char pro_cur_dir_name[310];
	memset(pro_cur_dir_name, '\0', sizeof(pro_cur_dir_name));
	strcpy(pro_cur_dir_name, Cur_Dir_Name);

	//��ȡfile��Ϣ
	char buf[BLOCK_SIZE * 10];
	string line;
	memset(buf, '\0', sizeof(buf));
	ifstream fin(filename);
	if (!fin.is_open()) {
		cout << "�޷�������" << endl;
		Cur_Dir_Addr = pro_cur_dir_addr;
		strcpy(Cur_Dir_Name, pro_cur_dir_name);
		return false;
	}
	while (getline(fin, line)) {
		strcat(buf, line.c_str());
	}

	//��file���Ƶ�����OS��
	char dir_path[100];
	sprintf(dir_path, "/home/%s/%s/%s_description", Cur_User_Name, lesson, filename);
	echo_func(Cur_Dir_Addr, dir_path, ">", buf); //�½�task�ļ�������

	return true;
}

bool judge_hw(char* namelist, char* lesson, char* hwname) {//��ʦ�����۱�����ҵ
	//��֤���
	if (strcmp(Cur_Group_Name, "teacher") != 0) {
		printf("ֻ����ʦ���Ե�����ҵ!\n");
		return false;
	}

	//������Ϣ
	int pro_cur_dir_addr = Cur_Dir_Addr;
	char pro_cur_dir_name[310];
	memset(pro_cur_dir_name, '\0', sizeof(pro_cur_dir_name));
	strcpy(pro_cur_dir_name, Cur_Dir_Name);

	//�½�������ҵ�����ĵ�( sname : mark)
	char score_path[310];
	sprintf(score_path, "/home/%s/%s/%s_score", Cur_Group_Name, lesson, hwname);
	touch_func(Cur_Dir_Addr, score_path, "");

	//��������
	ifstream fin(namelist);
	if (!fin.is_open()) {
		cout << "�޷�������" << endl;
		Cur_Dir_Addr = pro_cur_dir_addr;
		strcpy(Cur_Dir_Name, pro_cur_dir_name);
		return false;
	}
	string line;
	Relation relations[1000];
	int i = 0;
	char pwd[30];
	while (getline(fin, line)) {
		Relation rela;
		sscanf(line.c_str(), "%s:%s:%s", rela.student, rela.lesson, rela.teacher);
		relations[i++] = rela;
	}

	//�鿴��ҵ�����
	char buf[BLOCK_SIZE * 10];
	memset(buf, '\0', sizeof(buf));
	for (int j = 0; j < i; ++j) {
		//ֻ�����ſκ��Ǹ���ʦ bug
		// 
		//����ѧ���ļ��У��鿴ѧ���ļ�
		char hw_path[310];
		sprintf(hw_path, "/home/%s/%s", relations[j].student, relations[j].lesson);
		cd_func(Cur_Dir_Addr, hw_path);
		cat(Cur_Dir_Addr, hwname);//���ѧ������ҵ�ļ�����

		//��ʦ����ѧ����ҵ���( uname:score)
		double score = 0;
		char s_buf[30];
		printf("Please mark this assignment: ");
		scanf("%.2d", score);
		sprintf(s_buf, "%s:%.2d", relations[j].student, score);
		strcat(buf, s_buf);
	}

	//���ɼ������ĵ�
	echo_func(Cur_Dir_Addr, score_path, ">", buf);
	return true;
}

