#include<iostream>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include<fstream>
#include"function.h"
#include"os.h"

bool add_users(int role,char *filename) {
	FILE* userfr;
	if ((userfr = fopen(filename, "rb")) == NULL) {
		printf("�����ļ�����ʧ�ܣ�\n");
		return false;
	}
	istream 
}