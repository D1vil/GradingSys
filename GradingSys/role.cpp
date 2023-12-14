#include<iostream>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include<fstream>
#include"function.h"
#include"os.h"
#include"role.h"
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
	Relation relations[100];
	int i = 0;
	char pwd[30];
	while (getline(fin, line)) {
		Relation rela;
		sscanf(line.c_str(), "%s %s %s", rela.student, rela.lesson, rela.teacher);
		relations[i++] = rela;
	}

	//������������
	//��������Ĭ��������ͬ��
	for (int j = 0; j < i; j++) {
		//�����û������ļ���
		strcpy(pwd, relations[j].student);
		useradd(relations[j].student, pwd, "student");
		strcpy(pwd, relations[j].teacher);
		useradd(relations[j].teacher, pwd ,"teacher");
		//�����ļ����´�����Ӧ�γ��ļ���
		char dir_path[100];
		sprintf(dir_path, "/home/%s/%s", relations[j].teacher, relations[j].lesson);
		mkdir_func(Cur_Dir_Addr, dir_path);
		sprintf(dir_path, "/home/%s/%s", relations[j].student, relations[j].lesson);
		mkdir_func(Cur_Dir_Addr, dir_path);
	}

	//�ָ��ֳ�
	Cur_Dir_Addr = pro_cur_dir_addr;
	strcpy(Cur_Dir_Name, pro_cur_dir_name);
	printf("����������˻�\n");
	return true;
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
		cout << "�޷�����������" << endl;
		Cur_Dir_Addr = pro_cur_dir_addr;
		strcpy(Cur_Dir_Name, pro_cur_dir_name);
		return false;
	}
	while (getline(fin, line)) {
		strcat(buf, line.c_str());
	}

	//��file���Ƶ�����OS��
	char* p = strstr(filename, ".");
	p = '\0';
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
	char* p = strstr(hwname, ".");
	p = '\0';
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
		//�������ʦ�����������ſ�
		if ((strcmp(relations[j].teacher, Cur_User_Name) == 0) && (strcmp(relations[j].lesson, lesson) == 0)) {
			//����ѧ���ļ��У��鿴ѧ���ļ�
			char hw_path[310], s_buf[310];
			double score = 0;
			memset(hw_path, '\0', sizeof(hw_path));
			memset(s_buf, '\0', sizeof(s_buf));

			sprintf(hw_path, "/home/%s/%s", relations[j].student, relations[j].lesson);
			if (cd_func(Cur_Dir_Addr, hw_path)) {
				cat(Cur_Dir_Addr, hwname);//���ѧ������ҵ�ļ�����
				printf("Please mark this assignment: ");//��ʦ����ѧ����ҵ���( uname:score)
				scanf("%.2d", score);
			}
			else {
				printf("%s doesn't hand out the homework!\n", relations[j].student);
			}

			sprintf(s_buf, "%s:%.2d", relations[j].student, score);
			strcat(buf, s_buf);
		}
	}

	//���ɼ������ĵ�
	echo_func(Cur_Dir_Addr, score_path, ">", buf);

	//�ָ��ֳ�
	Cur_Dir_Addr = pro_cur_dir_addr;
	strcpy(Cur_Dir_Name, pro_cur_dir_name);
	return true;
}

