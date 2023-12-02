#include"server.h"
#include"client.h"

void handleClient(Client& client)
{
    int client_sock = client.client_sock;

    while (1)
    {
        memset(client.buffer, 0, sizeof(client.buffer));
        int len = recv(client_sock, client.buffer, sizeof(client.buffer), 0);

        if (strcmp(client.buffer, "exit\n") == 0 || len <= 0)
            break;

        printf("Client %d send message: %s", client_sock, client.buffer);
        parseCommand(client.buffer);
        cmd(client.buffer);
        send(client_sock, client.buffer, strlen(client.buffer), 0);
        printf("Send message: %s\n", client.buffer);
    }

    close(client_sock);
}

char* parseCommand(char* buffer)
{
    return buffer;
}

int run_server()
{
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);//������Ӧsocket
    struct sockaddr_in server_sockaddr;//���汾�ص�ַ��Ϣ
    server_sockaddr.sin_family = AF_INET;//����ipv4
    server_sockaddr.sin_port = htons(MY_PORT);//ָ���˿�
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);//��ȡ�������յ�������Ӧ

    if (bind(server_sock, (struct sockaddr*)&server_sockaddr, sizeof(server_sockaddr)) == -1) {//�󶨱���ip��˿�
        perror("Bind Failure\n");
        printf("Error: %s\n", strerror(errno));//���������Ϣ
        return -1;
    }

    printf("Listen Port : %d\n", MY_PORT);
    if (listen(server_sock, MAX_QUEUE_NUM) == -1) {//���ü���״̬
        perror("Listen Error");
        return -1;
    }

    printf("Waiting for connection!\n");
    while (true)
    {
        Client client;
        client.client_sock = accept(server_sock, (struct sockaddr*)&client.client_addr, &client.length);
       if (client.client_sock == -1) {//����ʧ��
            perror("Connect Error");
            return -1;
        }
        printf("Connection Successful\n");
        // �����ӽ���������ͻ�������
        if (fork() == 0)
        {
            close(server_sock); // �ӽ��̹رշ���������
            handleClient(client); // ����ͻ�������
            exit(0); // �ӽ��̴�����Ϻ��˳�
        }
        else close(client.client_sock); // �����̹رտͻ������ӣ���������
    }
   
    close(server_sock);//�رշ�������Ӧsocket
    return 0;
}