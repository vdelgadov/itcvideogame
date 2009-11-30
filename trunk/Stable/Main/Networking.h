#ifndef NETWORKING_H
#define NETWORKING_H

//#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
//#define _WINSOCKAPI_
//dd#define _WINSOCK2API_

#include "Engine.h"


#include <stdlib.h>

#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <fstream>
#include <string>
#include <sstream>
#define MAXCLIENTS 8


DWORD WINAPI networking(LPVOID Param);
void constantRun();
DWORD WINAPI RecvThread(LPVOID Whatever);    // The RecvThread() prototype
bool networkMove(int objectID, float x, float y, float z, LPVOID param);
void networkRecvMove(char* RecvBuffer);
void networkClient();
void networkServer();
void networkProcessPacket(char* RecvBuffer);
void networkQuit();
void networkSendActiveClients(); //function to send the clients that are active
void networkActivateClient(char* RecvBuffer); //function to receive and activate clients
void networkDeactivateClient(char* RecvBuffer); //function to receive and activate clients
void networkSendDectivatedClient(int client);

#endif NETWORKING_H_