#pragma once

#include <windows.h>

#if defined (WAP_EXPORT)
#define WAP_DLL __declspec(dllexport)
#else
#define WAP_DLL __declspec(dllexport)
#endif

LRESULT CALLBACK MouseHookProc(int code, WPARAM wparam, LPARAM lparam);

WAP_DLL bool setHooks(HWND hooker, HWND target);
WAP_DLL bool removeHooks();