#ifndef NETWORKING_H_
#define NETWORKING_H_
//#define _WINSOCKAPI_
//#define _WINSOCK2API_
//#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>

#include "engine.h"

#include <stdlib.h>

#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <fstream>
#include <string>


DWORD WINAPI networking(LPVOID Param);
int __cdecl enviarA(char * mensaje,char * destinatario);

#endif NETWORKING_H_