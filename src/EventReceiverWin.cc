#include "EventReceiver.h"
#include <iostream>
#include <assert.h>
#include <Windows.h>
#include <Commctrl.h>
#pragma comment(lib, "comctl32.lib")

static void logError()
{
	std::cerr << "Error: " << std::hex << GetLastError() << std::endl;
}

class EventReceiverWin : public EventReceiver {
public:
	EventReceiverWin(HWND hwnd, std::unique_ptr<EventDelegate> &&delegate) :
		EventReceiver(std::move(delegate)),
		m_hwnd(hwnd)
	{
	}

	~EventReceiverWin()
	{
		RemoveWindowSubclass(m_hwnd, handleSubclassMessage, 0);
	}

	void OnReload() override
	{
		auto child = FindWindowEx(m_hwnd, NULL, TEXT("Chrome_RenderWidgetHostHWND"), NULL);
		SetWindowSubclass(child, handleSubclassMessage, 0, (DWORD_PTR)this);
	}

	bool HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *result)
	{
		switch (uMsg) {
		case WM_POINTERDOWN:
		case WM_POINTERUPDATE:
		case WM_POINTERUP:
		{
			auto pointerId = GET_POINTERID_WPARAM(wParam);
			POINTER_INFO pointerInfo;
			if (!GetPointerInfo(pointerId, &pointerInfo)) {
				logError();
				break;
			}
			if (pointerInfo.pointerType != PT_PEN) {
				break;
			}
			POINTER_PEN_INFO pointerPenInfo;
			if (!GetPointerPenInfo(pointerId, &pointerPenInfo)) {
				logError();
				break;
			}

			const char *eventType;
			switch (uMsg) {
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

			int button;
			if (pointerInfo.pointerFlags & POINTER_FLAG_FIRSTBUTTON) {
				button = 0;
			}
			else if (pointerInfo.pointerFlags & POINTER_FLAG_SECONDBUTTON) {
				button = 2;
			}
			else if (pointerInfo.pointerFlags & POINTER_FLAG_THIRDBUTTON) {
				button = 1;
			}
			else if (pointerInfo.pointerFlags & POINTER_FLAG_FOURTHBUTTON) {
				button = 3;
			}
			else if (pointerInfo.pointerFlags & POINTER_FLAG_FIFTHBUTTON) {
				button = 4;
			}

			RECT himetricRect, displayRect;
			GetPointerDeviceRects(pointerInfo.sourceDevice, &himetricRect, &displayRect);

			double deviceX = ((double)pointerInfo.ptHimetricLocation.x - himetricRect.left) / (himetricRect.right - himetricRect.left);
			double deviceY = ((double)pointerInfo.ptHimetricLocation.y - himetricRect.top) / (himetricRect.bottom - himetricRect.top);
			double globalX = displayRect.left + deviceX * (displayRect.right - displayRect.left);
			double globalY = displayRect.top + deviceY * (displayRect.bottom - displayRect.top);

			POINT origin{ 0, 0 };
			MapWindowPoints(pointerInfo.hwndTarget, NULL, &origin, 1);

			double localX = globalX - origin.x;
			double localY = globalY - origin.y;

			const char *penType;
			if (pointerPenInfo.penFlags & PEN_FLAG_ERASER) {
				penType = "eraser";
			} else {
				penType = "pen";
			}

			HDC hdc = GetDC(pointerInfo.hwndTarget);
			double dpiScale = GetDeviceCaps(hdc, LOGPIXELSX) / 96.0;
			ReleaseDC(pointerInfo.hwndTarget, hdc);

			*result = 0;
			return Delegate()->OnTabletEvent(
				eventType,
				button,
				localX / dpiScale, localY / dpiScale,
				pointerPenInfo.pressure / 1024.0,
				penType, pointerInfo.pointerId
			);
		}
		default:
			break;
		}
		return false;
	}

private:
	static LRESULT CALLBACK handleSubclassMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
	{
		auto receiver = (EventReceiverWin *)dwRefData;
		LRESULT result;
		if (receiver->HandleMessage(hwnd, uMsg, wParam, lParam, &result)) {
			return result;
		}
		else {
			return DefSubclassProc(hwnd, uMsg, wParam, lParam);
		}
	}

	HWND m_hwnd;
};

void InitEventReceiver()
{
}

std::unique_ptr<EventReceiver> CreateEventReceiver(void *handle, std::unique_ptr<EventDelegate> &&delegate)
{
	return std::unique_ptr<EventReceiver>(
		new EventReceiverWin((HWND)handle, std::move(delegate))
	);
}
