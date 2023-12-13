#include"os.h"
#include"teacher.h"
#include<iomanip>
#include<time.h>
#include<string.h>
#include<stdio.h>
#include<iostream>

using namespace std;

void help() {
	cout.setf(ios::left); //���ö��뷽ʽΪleft 
	cout.width(30); //���ÿ�ȣ������ÿո���� 
	cout << "ls" << "Display the current directory listing" << endl;	//�г���ǰĿ¼�嵥
	cout.width(30);
	cout << "cd" << "Enter the specific directory" << endl;		//ǰ��ָ��Ŀ¼
	cout.width(30);
	cout << "assign" << "Post an assignment" << endl;  //������ҵ
	cout.width(30);
	cout << "checkAssign" << "Check the corresponding assignment submitted by student" << endl;  //�鿴ѧ������ҵ
	cout.width(30);
	cout << "score" << "Scoring assignment" << endl;  //����ҵ���

}





bool Assign(inode fileinode, int PIAddr, char AssignName[], char buf[]) {//�ڿγ�Ŀ¼���½���ҵĿ¼������ҵĿ¼���½���ҵ�ļ���д����ҵ���ݵ��ļ���
	mkdir(PIAddr, AssignName);
	mkfile(PIAddr + / AssignName, AssignName, buf);
	writefile(fileinode, PIAddr + / AssignName, buf);

}

bool Score(inode fileinode, int PIAddr, char Score[], char buf[]) { //ÿ����ҵĿ¼����ÿ��ѧ���ύ��ҵ��Ŀ¼���������ѧ������ҵ�ļ�����ʦ�½������ļ���д�����
	mkfile(PIAddr, Score, buf);
	writefile(fileinode, PIAddr, buf);
}