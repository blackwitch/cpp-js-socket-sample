#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <tchar.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define BUF_SIZE 1024               
struct cPacket_Test {
	int		point;
	char	strLength32[32];
};

int main(int argc, char* argv[])           
{
	WSADATA wsaData;                         
	SOCKET server_sock;         
	SOCKADDR_IN server_addr;  
	int port = 28000;

	if( WSAStartup( MAKEWORD(2, 2), &wsaData) !=0 )  {
		printf("WSAStartup error!");
		exit(1);
	}

	server_sock = socket(PF_INET, SOCK_STREAM, 0);   
	if(server_sock == INVALID_SOCKET){
		printf("socket error!");
		exit(1);
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port);
	
	if( bind(server_sock, (SOCKADDR*) &server_addr, sizeof(server_addr)) == SOCKET_ERROR){
		printf("bind error!");
		exit(1);
	}
	
	if( listen(server_sock, 5) == SOCKET_ERROR){
		printf("listen Error!");
		exit(1);
	}
	
	SOCKADDR_IN client_addr;
	int clientAddrSize = sizeof(client_addr); 
	while(1)
	{
		SOCKET client_sock = accept(server_sock, (SOCKADDR*) &client_addr, &clientAddrSize);  
		if(client_sock == -1){
			printf("accept Error!");
			exit(1);
		}
		printf("Accept client \n"); 

		int length = 0;
		char message[BUF_SIZE] ={0,};
		char data[BUF_SIZE] = {0,};
		while((length = recv(client_sock, message, BUF_SIZE, 0)) !=0)
		{   
			if (length == -1)
			{
				printf("Disconnected\n"); 
				break;
			}
			memset(data, 0, BUF_SIZE);
			printf("data:%s  recv_len = %d \n", message, length);

			cPacket_Test resp = {99,"[Go Home!!]"};
			memcpy(data,&resp, sizeof(cPacket_Test));

			send(client_sock, data, sizeof(cPacket_Test), 0);
		}
		closesocket(client_sock);
	}

	closesocket(server_sock);
	WSACleanup();
	return 0;
}