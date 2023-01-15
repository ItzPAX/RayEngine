#pragma once

class UI;
typedef std::shared_ptr<UI> UIPtr;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);