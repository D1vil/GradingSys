#pragma once
#include<iostream>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include"os.h"



//��ʾ����
void help();

//���ܺ���
bool ViewAssign(int TAAddr, char* AssignName); //�鿴��ҵ
bool Submit(int CurAddr, char* AssignName, char* content, char* s_type, char* buf);   //�ύ��ҵ
bool ViewScore(); //�鿴��ҵ����


