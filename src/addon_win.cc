#include "addon.h"
#include <iostream>
#include <assert.h>
#include <Windows.h>
#include <Commctrl.h>
#pragma comment(lib, "comctl32.lib")

LRESULT CALLBACK handleSubclassMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	std::cout << "message " << std::hex << uMsg << std::endl;
	return DefSubclassProc(hwnd, uMsg, wParam, lParam);
}

void InterceptWindow(void *handle)
{
	auto hwnd = (HWND)handle;
	SetWindowSubclass(hwnd, handleSubclassMessage, 0, 0);
}

void UninterceptWindow(void *handle)
{

}

void Init()
{

}
