#ifndef NETWORKING_H_
#define NETWORKING_H_

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


DWORD WINAPI networking(LPVOID Param);
int __cdecl enviarA(char * mensaje,char * destinatario);
bool networkMove(int objectID, float x, float y, float z, LPVOID param);

#endif NETWORKING_H_