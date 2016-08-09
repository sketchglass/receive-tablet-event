#include "addon.h"
#include <iostream>
#include <Windows.h>

LRESULT CALLBACK hookMessage(int code, WPARAM wParam, LPARAM lParam) {
  return 0;
}

void InterceptWindow(void *handle) {
  HWND hWnd = (HWND)handle;
}
void UninterceptWindow(void *handle) {

}
void Init() {
  SetWindowsHookEx(WH_GETMESSAGE, hookMessage, 0, 0);
}
