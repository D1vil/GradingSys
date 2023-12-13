#include"os.h"
#include"teacher.h"
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
	cout << "Checkteach" << "Display the current directory listing" << endl;	//�г���ǰĿ¼�嵥,�鿴���ڿγ̣�ls)
	cout.width(30);
	cout << "assign" << "Post an assignment" << endl;  //������ҵ
	cout.width(30);
	cout << "checkAssign" << "Check the corresponding assignment submitted by student" << endl;  //�鿴ѧ������ҵ
	cout.width(30);
	cout << "score" << "Scoring assignment" << endl;  //����ҵ���

}





bool Assign(int CurAddr, char* AssignName, char* buf, char* s_type) {//�ڿγ�Ŀ¼���½���ҵĿ¼������ҵĿ¼���½���ҵ�ļ���д����ҵ���ݵ��ļ���
	mkdir_func(CurAddr, AssignName);
	touch_func(CurAddr, AssignName, buf);
	echo_func(CurAddr, AssignName, s_type, buf);
}

bool CheckAssign(int SAAddr, char* str, char* AssignName) {
	cd_func(SAAddr, str);
	//cat();
}

bool Score(int CurAddr, char* FileName, char* content, char* buf, char* s_type) { //ÿ����ҵĿ¼����ÿ��ѧ���ύ��ҵ��Ŀ¼���������ѧ������ҵ�ļ�����ʦ�½������ļ���д�����
	touch_func(CurAddr, FileName, buf);
	echo_func(CurAddr, content, s_type, buf);
}