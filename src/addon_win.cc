#include "addon.h"
#include <iostream>
#include <assert.h>
#include <Windows.h>
#include <Commctrl.h>
#pragma comment(lib, "comctl32.lib")

void logError()
{
	std::cerr << "Error: " << std::hex << GetLastError() << std::endl;
}

LRESULT CALLBACK handleSubclassMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	auto root = (HWND)dwRefData;
	switch (uMsg) {
	case WM_POINTERDOWN:
	case WM_POINTERUPDATE:
	case WM_POINTERUP:
	{
		auto pointerId = GET_POINTERID_WPARAM(wParam);
		POINTER_INFO pointerInfo;
		if (!GetPointerInfo(pointerId, &pointerInfo))
		{
			logError();
			break;
		}
		if (pointerInfo.pointerType != PT_PEN)
		{
			break;
		}
		POINTER_PEN_INFO pointerPenInfo;
		if (!GetPointerPenInfo(pointerId, &pointerPenInfo))
		{
			logError();
			break;
		}

		const char *eventType;
		switch (uMsg)
		{
		case WM_POINTERDOWN:
			eventType = "down";
			break;
		case WM_POINTERUPDATE:
			eventType = "move";
			break;
		case WM_POINTERUP:
			eventType = "up";
			break;
		}

		HDC hdc = GetDC(pointerInfo.hwndTarget);
		double widthPx = GetDeviceCaps(hdc, HORZRES);
		double widthMm = GetDeviceCaps(hdc, HORZSIZE);
		double himetricToPx = widthPx / (widthMm * 100.0);
		double x = pointerInfo.ptHimetricLocationRaw.x * himetricToPx;
		double y = pointerInfo.ptHimetricLocationRaw.y * himetricToPx;

		const char *penType;
		if (pointerPenInfo.penFlags & PEN_FLAG_ERASER) {
			penType = "eraser";
		}
		else {
			penType = "pen";
		}
		EmitTabletEvent(
			root, eventType,
			x, y, pointerPenInfo.pressure / 1024.0,
			penType, pointerInfo.pointerId
		);
		return 0;
	}
	default:
		break;
	}
	return DefSubclassProc(hwnd, uMsg, wParam, lParam);
}

void InterceptWindow(void *handle)
{
	auto hwnd = (HWND)handle;
	auto child = FindWindowEx(hwnd, NULL, TEXT("Chrome_RenderWidgetHostHWND"), NULL);
	SetWindowSubclass(child, handleSubclassMessage, 0, (DWORD_PTR)hwnd);
}

void UninterceptWindow(void *handle)
{

}

void Init()
{

}
