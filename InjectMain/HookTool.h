#pragma once
#include "stdafx.h"
#include <Windows.h>
BOOL hook(_In_ LPCWSTR dllName, _In_ LPCSTR funcName, _In_ PROC pfnNew, _Out_ PBYTE pOrgBytes);
BOOL unHook(_In_ LPCWSTR dllName, _In_ LPCSTR funcName, _In_ PROC pfnNew, _In_ PBYTE pOrgBytes);
