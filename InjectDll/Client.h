#pragma once

#include"stdafx.h"
#include <iostream>
#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include <winsock2.h>
#define BUFSIZE 512
#pragma comment(lib, "ws2_32.lib")

void connectServer();