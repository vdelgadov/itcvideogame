#include "Networking.h"
#include "Params.h"

#define DEFAULT_BUFLEN 512
char  default_port[3] = "50";
char ip_address[21] = "10.40.49.182";

DWORD WINAPI networking(LPVOID Param)
{
	Params* params = (Params*)Param;
	while (params->notQuit) 
	{

		WSADATA wsaData; 
		SOCKET ListenSocket = INVALID_SOCKET,
			   ClientSocket = INVALID_SOCKET;
		struct addrinfo *result = NULL,
						hints;
		char recvbuf[DEFAULT_BUFLEN];
		int iResult;
		int recvbuflen = DEFAULT_BUFLEN;
	    
		
		// Initialize Winsock
		iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
		if (iResult != 0) {
			printf("No se pudo inicializar el servidor WinSock problem\n");
			return 1;
		}
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = 0;//IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		while(true)
		{
		// Resolve the server address and port
		iResult = getaddrinfo(NULL, default_port, &hints, &result);
		if ( iResult != 0 ) {
			printf("No se pudo inicializar el servidor Server problem\n");
			WSACleanup();
			return 1;
		}

		// Create a SOCKET for connecting to server
		ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (ListenSocket == INVALID_SOCKET) {
			printf("No se pudo inicializar el servidor Socket creation problem\n");
			freeaddrinfo(result);
			WSACleanup();
			return 1;
		}

		// Setup the TCP listening socket
		iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			printf("No se pudo inicializar el servidor\n");
			freeaddrinfo(result);
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}

		freeaddrinfo(result);
		
		iResult = listen(ListenSocket, SOMAXCONN);
		if (iResult == SOCKET_ERROR) {
			printf("No se pudo inicializar el servidor\n");
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}

		// Accept a client socket
		ClientSocket = accept(ListenSocket, NULL, NULL);
		if (ClientSocket == INVALID_SOCKET) {
			printf("No se pudo inicializar el servidor\n");
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}

		// No longer need server socket
		closesocket(ListenSocket);
		ZeroMemory(recvbuf, DEFAULT_BUFLEN);
		// Receive until the peer shuts down the connection
		do {

			iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
			if (iResult > 0) {
	//--------------------------------------------------------------------------------------
				//printf("recibido: %s\n",recvbuf);
				char resto[512];
				char cacho[512];
				for(int i =strlen(recvbuf)-1;i>-1;i--)
				{
					if(recvbuf[i]=='@')
					{
						strcpy_s(resto,&recvbuf[i+1]);
						recvbuf[i]='\0';
						break;
					}
				}
				strcpy_s(cacho,recvbuf);
				

				if(strcmp (cacho,"Coneccion") == 0){
					printf("Se inicio conneccion\n");
					send(ClientSocket,"Coneccion",DEFAULT_BUFLEN,0);
				}

				else if(strcmp (cacho,"Move") == 0)
				{
					char* objID = strtok(resto,"%");
					char* x = strtok(NULL,"%");
					char* y = strtok(NULL,"%");
					char* z = strtok(NULL,"%");
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
					params->scene->find(iID)->move(iX,iY,iZ,0.0,0.0,0.0,1.0);

					//send(ClientSocket,a.c_str(),strlen(a.c_str()),0);
		
					/*char comando[512]  = "listar.bat ";
					string a = "Ver archivos@";
					string lista = getfile("listado.txt ");
					a += lista;
					//printf("%s",a.c_str());
					send(ClientSocket,a.c_str(),strlen(a.c_str()),0);*/
				}
				
				
	//-----------------------------------------------------------------------------------
				//printf("Mensaje recibido: %s de %d y length %d\n", recvbuf, iResult, recvbuflen);
				//send( ClientSocket, "cosa", iResult, 0 );

			}
			else if (iResult == 0);
			else  {
				printf("recv failed: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}

		} while (iResult > 0);
		
		// shutdown the connection since we're done
		iResult = shutdown(ClientSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			printf("shutdown failed: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}
		
		}
		// cleanup
		closesocket(ClientSocket);
		WSACleanup();
		return 0;
	}
	return 0;
}
int __cdecl enviarA(char * mensaje,char * destinatario) 
{
	int actionResult = 0;
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    char *sendbuf = mensaje;
    char recvbuf[2048];
    int iResult;
    int recvbuflen = 2048;
    

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(destinatario, default_port, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("Error at socket(): %ld\n", WSAGetLastError());
            freeaddrinfo(result);
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    // Send an initial buffer
    iResult = send( ConnectSocket, sendbuf, (int)strlen(sendbuf), 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }
//iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
	if (iResult > 0) {
//----------------------------------------------
	//lo recibimos de respuesta
	char * cacho;
	cacho = strtok (recvbuf,"@");

	if(strcmp(cacho,"Exito") == 0)
	{
		printf("\nAcciones realizadas con exito\n");
		actionResult = 0;
	}
	else if(strcmp (cacho,"Move") == 0)
	{
		printf("No se pudieron realizar las acciones requeridas");
		actionResult = 1;
	}
	else if(strcmp (cacho,"Fallo") == 0)
	{
		printf("No se pudieron realizar las acciones requeridas");
		actionResult = 1;
	}
//--------------------------------------
	}	
    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();
	return actionResult;
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
	printf("%s",mensaje);
	if(enviarA(mensaje,ip_address) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
	return 0;
}


