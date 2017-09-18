#pragma once
#include<Windows.h>

SHORT WINAPI myGetAsyncKeyState(
	_In_ int vKey
);

BOOL WINAPI myDestroyWindow(
	_In_ HWND hWnd
);