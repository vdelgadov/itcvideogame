#define _WINSOCKAPI_
#include <winsock2.h>
#include <stdio.h>
#include "Networking.h"
#include "Params.h"

#pragma comment(lib, "Ws2_32.lib")    // WinSock Library
//I'M A CLIENT.
#define SERVER_ADDRESS "192.168.0.2"
#define SERVER_PORT 17000

WSADATA Winsock;    // Stores information about Winsock
SOCKET Socket;    // The ID of the socket
sockaddr_in ServerAddress;    // The address to send data to
char SendBuffer[256];    // The buffer of data to send
char RecvBuffer[256];    // The buffer of data to receive
int SizeInt = sizeof(ServerAddress);    // The size of the server's address

//Cambie mensaje aqui como variable global para poder agarrarla y enviarla con el socket.
char mensaje[512];

DWORD WINAPI networking(LPVOID Param)
{
	Params* params = (Params*)Param;
	WSAStartup(MAKEWORD(2, 2), &Winsock);

    if(LOBYTE(Winsock.wVersion) != 2 || HIBYTE(Winsock.wVersion) != 2)    // Check version
    {
        WSACleanup();
        return 1;
    }

    // Make the Socket
    Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // Input Server Information
    ZeroMemory(&ServerAddress, sizeof(ServerAddress));    // clear the struct
    ServerAddress.sin_family = AF_INET;    // set the address family
    ServerAddress.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);    // set the IP address
    ServerAddress.sin_port = SERVER_PORT;    // set the port

    // Send a knock to the server
    SendBuffer[0] = 1;
    sendto(Socket, SendBuffer, 1, 0, (sockaddr*)&ServerAddress, sizeof(sockaddr));

    // Start the receiver thread
    CreateThread(NULL, 0, RecvThread, NULL, 0, NULL);

    // Send the Messages
    while(true)
    {
        /*gets(SendBuffer);
        sendto(Socket, SendBuffer, 256, 0, (sockaddr*)&ServerAddress, sizeof(sockaddr));

        if(SendBuffer[0] == ' ')
            break;*/
    };

    WSACleanup();
	return 0;
}
DWORD WINAPI RecvThread(LPVOID Whatever)
{
    while(true)
    {
        // Wait for messages to arrive
        recvfrom(Socket, RecvBuffer, 256, 0, (sockaddr*)&ServerAddress, &SizeInt);

        char msgTail[512];
		char msgStart[512];
		for(int i =strlen(RecvBuffer)-1;i>-1;i--)
		{
			if(RecvBuffer[i]=='@')
			{
				strcpy_s(msgTail,&RecvBuffer[i+1]);
				RecvBuffer[i]='\0';
				break;
			}
		}
		strcpy_s(msgStart,RecvBuffer);
		if(strcmp (msgStart,"Move") == 0)
		{
			printf("Llego Move!\n");
			networkRecvMove(RecvBuffer);
		}
	}

    return 0;
}
void networkRecvMove(char* RecvBuffer)
{
	char msgTail[512];
	char msgStart[512];
	for(int i =strlen(RecvBuffer)-1;i>-1;i--)
	{
		if(RecvBuffer[i]=='@')
		{
			strcpy_s(msgTail,&RecvBuffer[i+1]);
			RecvBuffer[i]='\0';
			break;
		}
	}
	strcpy_s(msgStart,RecvBuffer);


	if(strcmp (msgStart,"Move") == 0){
		printf("Llego Move!\n");
	}
}

std::string float_to_str(float f)
{
	std::stringstream stream;
	stream << f;
	return stream.str();
}
bool networkMove(int objectID, float x, float y, float z, LPVOID Param)
{
	Params* params = (Params*)Param;
	char mensaje[512];
	strcpy_s(mensaje, 512, "Move@");
	strcat_s(mensaje, 512, float_to_str(objectID).c_str());
	strcat_s(mensaje, 512, "%");
	strcat_s(mensaje, 512, float_to_str(x).c_str());
	strcat_s(mensaje, 512, "%");
	strcat_s(mensaje, 512, float_to_str(y).c_str());
	strcat_s(mensaje, 512, "%");
	strcat_s(mensaje, 512, float_to_str(z).c_str());
	//printf("Mover!!! %s\n",mensaje);
	
	sendto(Socket, SendBuffer, 256, 0, (sockaddr*)&ServerAddress, sizeof(sockaddr));
	return 0;
}