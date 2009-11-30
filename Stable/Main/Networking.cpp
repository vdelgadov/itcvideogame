#define _WINSOCKAPI_
#include <winsock2.h>
#include <stdio.h>
#include "Networking.h"
#include <conio.h>
#include "Params.h"*

#pragma comment(lib, "Ws2_32.lib")    // WinSock Library
//I'M A CLIENT.
#define SERVER_ADDRESS "10.201.38.239"
#define SERVER_PORT 17000
#define SERVER 1 //0 or 1
#define BUFFLEN 512


WSADATA Winsock;    // Stores information about Winsock
SOCKET Socket;    // The ID of the socket
sockaddr_in ServerAddress;    // The address to send data to
char SendBuffer[BUFFLEN];    // The buffer of data to send
char RecvBuffer[BUFFLEN];    // The buffer of data to receive
int SizeInt = sizeof(ServerAddress);    // The size of the server's address

//network server variables
//sockaddr_in ServerAddress;
sockaddr_in IncomingAddress;     // Contains the address of the sending client
sockaddr_in ClientAddress[MAXCLIENTS];    // Stores the client's addresses
char Buffer[BUFFLEN];
int AddressLen = sizeof(IncomingAddress);

//Cambie mensaje aqui como variable global para poder agarrarla y enviarla con el socket.
char mensaje[BUFFLEN];
Params* params;
DWORD WINAPI networking(LPVOID Param)
{
	params = (Params*)Param;
	if(SERVER == 0)
	{
		networkClient();
		return 1;
	}
	else
	{
		networkServer();
		return 1;
	}
	while(params->notQuit)
	{
		SendBuffer[0] = 'Q';
	    sendto(Socket, SendBuffer, 1, 0, (sockaddr*)&ServerAddress, sizeof(sockaddr));
		break;
	};

	return 0;
}
DWORD WINAPI RecvThread(LPVOID Whatever)
{
    while(true)
    {
        // Wait for messages to arrive
        recvfrom(Socket, RecvBuffer, BUFFLEN, 0, (sockaddr*)&ServerAddress, &SizeInt);
		printf(RecvBuffer);

        char msgTail[BUFFLEN];
		char msgStart[BUFFLEN];
		cout << "Recibido: " << RecvBuffer <<endl;
		for(int i =0; i < (strlen(RecvBuffer));i++)
		//for(int i =strlen(RecvBuffer)-1;i>-1;i--)
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
			//printf("Llego Move!\n");
			//printf(RecvBuffer);
			networkRecvMove(msgTail);
		}
		if(strcmp (msgStart,"Activate") == 0)
		{
			//printf("Llego activate!\n");
			//printf(RecvBuffer);
			networkActivateClient(msgTail);
		}
		if(strcmp (msgStart,"Deactivate") == 0)
		{
			//printf("Llego activate!\n");
			//printf(RecvBuffer);
			networkDeactivateClient(msgTail);
		}
		if(strcmp (msgStart,"MyObject") == 0)
		{
			std::istringstream sID(msgTail);
			float iID;
			sID >> iID;
			//params.myObject = o;
			params->myObject = params->scene->find(iID);
			
		}
		
	}

    return 0;
}
void networkRecvMove(char* RecvBuffer)
{
	char* objID = strtok(RecvBuffer,"@");
	char* x = strtok(NULL,"@");
	char* y = strtok(NULL,"@");
	char* z = strtok(NULL,"@");
	std::istringstream sID(objID);
	float iID;
	sID >> iID;
	std::istringstream sx(x);
	float iX;
	sx >> iX;
	std::istringstream sy(y);
	float iY;
	sy >> iY;
	std::istringstream sz(z);
	float iZ;
	sz >> iZ;
	cout << "moving: " << "id: " << iID << " x: " << iX << " y: " << iY << " z: " << iZ << endl;
	params->scene->find(iID)->setPosition(Vector3D(iX,iY,iZ));
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
	char mensaje[BUFFLEN];
	strcpy_s(mensaje, BUFFLEN, "Move@");
	strcat_s(mensaje, BUFFLEN, float_to_str(objectID).c_str());
	strcat_s(mensaje, BUFFLEN, "@");
	strcat_s(mensaje, BUFFLEN, float_to_str(x).c_str());
	strcat_s(mensaje, BUFFLEN, "@");
	strcat_s(mensaje, BUFFLEN, float_to_str(y).c_str());
	strcat_s(mensaje, BUFFLEN, "@");
	strcat_s(mensaje, BUFFLEN, float_to_str(z).c_str());
	//printf("Mover!!! %s\n",mensaje);
	
	sendto(Socket, mensaje, BUFFLEN, 0, (sockaddr*)&ServerAddress, sizeof(sockaddr));
	return 0;
}
void networkClient()
{
	cout << "client!" << endl;
	WSAStartup(MAKEWORD(2, 2), &Winsock);

    if(LOBYTE(Winsock.wVersion) != 2 || HIBYTE(Winsock.wVersion) != 2)    // Check version
    {
        WSACleanup();
        return;
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
	//no quitar!!!!
	while(true)
	{}
    // Send the Messages
    /*while(true)
    {
        gets(SendBuffer);
		SendBuffer[BUFFLEN-1] = '\0';
		//cout << "se envio: '" << SendBuffer << "'" << endl;
        sendto(Socket, SendBuffer, BUFFLEN, 0, (sockaddr*)&ServerAddress, sizeof(sockaddr));

        if(SendBuffer[0] == ' ')
            break;
    };*/

    WSACleanup();
	return;
}
void networkServer()
{
	cout << "server!" << endl;
    WSAStartup(MAKEWORD(2, 2), &Winsock);

    if(LOBYTE(Winsock.wVersion) != 2 || HIBYTE(Winsock.wVersion) != 2)
    {
        WSACleanup();
        return;
    }

    // Make the Socket
    Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // Input server information and bind it to the socket
    ZeroMemory(&ServerAddress, sizeof(ServerAddress));
    ServerAddress.sin_family = AF_INET;
      ServerAddress.sin_port = SERVER_PORT;
    bind(Socket, (sockaddr*)&ServerAddress, sizeof(ServerAddress));

    while(true)
    {
        if(recvfrom(Socket, Buffer, BUFFLEN, 0, (sockaddr*)&IncomingAddress, &AddressLen))
        {
			cout << "Packet received: "<<Buffer<<endl;
            // If the packet is a knock, add the client's address to ClientAddress
            if(Buffer[0] == 1)
            {
                for(int i = 0; i < MAXCLIENTS; i++)
                {
                    if(!ClientAddress[i].sin_family)
                    {
                        ClientAddress[i] = IncomingAddress;
						networkSendActiveClients(i); //send all active clients to new client
						params->scene->find(i+1)->isRendereable = true;
						params->scene->find(i+1)->boundingSphere = true;
						
                        break;
                    }
                }
                continue;
            }
			//----------------------------------
			/*      // Display the message and broadcast it to all active clients
            Buffer[BUFFLEN-1] = '\0'; // Always end the packet with this
			printf("Broadcasting: ");
            printf(Buffer);
            printf("\n");
			
            for(int i = 0; i < MAXCLIENTS; i++)
            {
                if(ClientAddress[i].sin_family)
                    sendto(Socket, Buffer, BUFFLEN, 0, (sockaddr*)&ClientAddress[i],
                           sizeof(sockaddr));
            }*/
			
			networkProcessPacket(Buffer);
			//----------------------------------
            
                  // If a client has quit, remove that client's address from ClientAddress
           if(Buffer[0] == 'Q')
            {
                for(int i = 0; i < MAXCLIENTS; i++)
                {
                   if(ClientAddress[i].sin_addr.s_addr == IncomingAddress.sin_addr.s_addr)
				   {
                        ZeroMemory(&ClientAddress[i], sizeof(sockaddr_in));
				   }
					//remove client!!!!!
					networkSendDectivatedClient(i);
                }
            }
        }
    }

    WSACleanup();

    return;
}
void networkQuit()
{
	SendBuffer[0] = 'Q';
    sendto(Socket, SendBuffer, 1, 0, (sockaddr*)&ServerAddress, sizeof(sockaddr));
}
void networkProcessPacket(char* RecvBuffer)
{
	char response[BUFFLEN];
	char msgTail[BUFFLEN];
	char msgStart[BUFFLEN];
	for(int i =0;i<strlen(RecvBuffer);i++)
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
		char* objID = strtok(msgTail,"@");
		char* x = strtok(NULL,"@");
		char* y = strtok(NULL,"@");
		char* z = strtok(NULL,"@");
		std::istringstream sID(objID);
		float iID;
		sID >> iID;
		std::istringstream sx(x);
		float iX;
		sx >> iX;
		std::istringstream sy(y);
		float iY;
		sy >> iY;
		std::istringstream sz(z);
		float iZ;
		sz >> iZ;
		params->scene->find(iID)->move(Vector3D(iX,iY,iZ));
		//generate string to transmite with new coordinates
		strcpy_s(response, BUFFLEN, "Move@");
		strcat_s(response, BUFFLEN, float_to_str(iID).c_str());
		strcat_s(response, BUFFLEN, "@");
		strcat_s(response, BUFFLEN, float_to_str(params->scene->find(iID)->getVehicle()->getPos().x).c_str());
		strcat_s(response, BUFFLEN, "@");
		strcat_s(response, BUFFLEN, float_to_str(params->scene->find(iID)->getVehicle()->getPos().y).c_str());
		strcat_s(response, BUFFLEN, "@");
		strcat_s(response, BUFFLEN, float_to_str(params->scene->find(iID)->getVehicle()->getPos().z).c_str());
	}
	for(int i = 0; i < MAXCLIENTS; i++)
    {
        if(ClientAddress[i].sin_family)
            sendto(Socket, response, BUFFLEN, 0, (sockaddr*)&ClientAddress[i],
                   sizeof(sockaddr));
    }
}
void main()
{
	DWORD ThreadIdNetwork;
	HANDLE ThreadHandleNetwork;
	char* params = NULL;
	ThreadHandleNetwork = CreateThread(NULL,0,networking,&params,0,&ThreadIdNetwork);
	while(true)
	{
	}
}
void networkSendActiveClients(int client) //function to send the clients that are active
{
	 for(int i = 0; i < MAXCLIENTS; i++)
     {
		 if(ClientAddress[i].sin_family)
		 {
			char message[BUFFLEN];
			char response[BUFFLEN];
			//activate client
			strcpy_s(message, BUFFLEN, "Activate@");
			strcat_s(message, BUFFLEN, float_to_str(client+1).c_str());
			sendto(Socket, message, BUFFLEN, 0, (sockaddr*)&ClientAddress[i],sizeof(sockaddr));
			//set client position
			strcpy_s(response, BUFFLEN, "Move@");
			strcat_s(response, BUFFLEN, float_to_str(client+1).c_str());
			strcat_s(response, BUFFLEN, "@");
			strcat_s(response, BUFFLEN, float_to_str(params->scene->find(client+1)->getVehicle()->getPos().x).c_str());
			strcat_s(response, BUFFLEN, "@");
			strcat_s(response, BUFFLEN, float_to_str(params->scene->find(client+1)->getVehicle()->getPos().y).c_str());
			strcat_s(response, BUFFLEN, "@");
			strcat_s(response, BUFFLEN, float_to_str(params->scene->find(client+1)->getVehicle()->getPos().z).c_str());
			sendto(Socket, response, BUFFLEN, 0, (sockaddr*)&ClientAddress[client],sizeof(sockaddr));
		 }
	 }
	 //set clients object
	char message[BUFFLEN];
	strcpy_s(message, BUFFLEN, "MyObject@");
	strcat_s(message, BUFFLEN, float_to_str(client+1).c_str());
	sendto(Socket, message, BUFFLEN, 0, (sockaddr*)&ClientAddress[client],sizeof(sockaddr));
}
void networkSendDectivatedClient(int client) //function to send the clients that are active
{
	 for(int i = 0; i < MAXCLIENTS; i++)
     {
		 if(ClientAddress[i].sin_family)
		 {
			char message[BUFFLEN];
			char response[BUFFLEN];
			//activate client
			strcpy_s(message, BUFFLEN, "Deactivate@");
			strcat_s(message, BUFFLEN, float_to_str(i+1).c_str());
			sendto(Socket, message, BUFFLEN, 0, (sockaddr*)&ClientAddress[i],sizeof(sockaddr));
			//set client position
			strcpy_s(response, BUFFLEN, "Move@");
			strcat_s(response, BUFFLEN, float_to_str(i+1).c_str());
			strcat_s(response, BUFFLEN, "@");
			strcat_s(response, BUFFLEN, float_to_str(0.0).c_str());
			strcat_s(response, BUFFLEN, "@");
			strcat_s(response, BUFFLEN, float_to_str(1.0).c_str());
			strcat_s(response, BUFFLEN, "@");
			strcat_s(response, BUFFLEN, float_to_str(0.0).c_str());
			sendto(Socket, response, BUFFLEN, 0, (sockaddr*)&ClientAddress[client],sizeof(sockaddr));
		 }
	 }
}
void networkActivateClient(char*  RecvBuffer) //function to receive and activate clients
{
	char* objID = strtok(RecvBuffer,"@");
	std::istringstream sID(objID);
	float iID;
	sID >> iID;
	
	cout << "activating: " << "id: " << iID  << endl;
	params->scene->find(iID)->isRendereable = true;
	params->scene->find(iID)->boundingSphere = true;
}
void networkDeactivateClient(char* RecvBuffer)
{
	char* objID = strtok(RecvBuffer,"@");
	std::istringstream sID(objID);
	float iID;
	sID >> iID;
	
	cout << "activating: " << "id: " << iID  << endl;
	params->scene->find(iID)->isRendereable = false;
	params->scene->find(iID)->boundingSphere = false;
}