#pragma once
#include<iostream>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include"os.h"


//��ʾ����
void help();


//���ܺ���
bool CheckAssign();
bool Assign(inode fileinode, int PIAddr, char AssignName[], char buf[]);        //������ҵ
bool Score(inode fileinode, int PIAddr, char Score[], char buf[]);         //���