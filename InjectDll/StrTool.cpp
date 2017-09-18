#include "stdafx.h"
#include "StrTool.h"
#include <stdlib.h>
WCHAR * charToWchar(const char *s) {
	int w_nlen = MultiByteToWideChar(CP_ACP, 0, s, -1, NULL, 0);
	WCHAR *ret;
	ret = (WCHAR*)malloc(sizeof(WCHAR)*w_nlen);
	memset(ret, 0, sizeof(ret));
	MultiByteToWideChar(CP_ACP, 0, s, -1, ret, w_nlen);
	return ret;
}

char * WchartoChar(const wchar_t *pwstr)

{
	int len = WideCharToMultiByte(CP_ACP,0,pwstr,-1,NULL,0,NULL,NULL);
	char * ret;
	ret = (char*)malloc(sizeof(char)*len);
	memset(ret, 0, sizeof(ret));
	WideCharToMultiByte(CP_ACP, 0, pwstr, -1, ret, len, NULL, NULL);
	return ret;
}
