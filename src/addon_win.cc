#include "addon.h"
#include <iostream>
#include <Commctrl.h>
#pragma comment(lib, "comctl32.lib")

LRESULT CALLBACK interceptEvent(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
  std::cout << "intercepting " << uMsg << std::endl;
  return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

void InterceptWindow(void *handle, const FunctionPersistent &callback) {
  HWND hWnd = (HWND)handle;
  SetWindowSubclass(hWnd, interceptEvent, 0, 0);
}
void UninterceptWindow(void *handle) {

}
void Init() {
}
