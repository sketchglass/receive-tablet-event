#include "addon.h"
#include <iostream>
#include <Commctrl.h>
#pragma comment(lib, "comctl32.lib")

LRESULT CALLBACK hookMessage(int code, WPARAM wParam, LPARAM lParam) {
  return 0;
}

void InterceptWindow(void *handle, const FunctionPersistent &callback) {
  HWND hWnd = (HWND)handle;
}
void UninterceptWindow(void *handle) {

}
void Init() {
  SetWindowsHookEx(WH_GETMESSAGE, hookMessage, 0, 0);
}
