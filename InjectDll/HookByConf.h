#pragma once
#include "stdafx.h"
#include<Windows.h>
#include<string>
#include<map>
#include<vector>
using namespace std;

VOID hookByConf(HMODULE hInstance);
VOID unhoookByConf(HMODULE hInstance);