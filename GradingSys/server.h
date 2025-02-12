#pragma once
#include<stdio.h>
#include<string.h>
#include<string>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<cstring>
#include<cerrno>
#include<vector>
#include<cstdlib>

#define MY_PORT 6591
#define BUF_SIZE 1024
#define MAX_QUEUE_NUM 5

struct Client
{
    int client_sock;
    int ptr = -1;
    bool islogin;
    int Cur_Dir_Addr;                           //当前目录
    char Cur_Dir_Name[310];                     //当前目录名
    char Cur_User_Name[110];                    //当前登陆用户名
    char Cur_Group_Name[110];                   //当前登陆用户组名
    char Cur_User_Dir_Name[310];                //当前登陆用户目录名		

    char buffer[BUF_SIZE];
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);
};

void Welcome(Client&);
void handleClient(Client&);
int Initialize();
bool ever_logging();