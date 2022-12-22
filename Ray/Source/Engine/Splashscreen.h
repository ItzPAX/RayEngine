#pragma once
#include "Platform/Win32/w32Window.h"

namespace SplashScreen {

	VOID RAY_API Open();
	VOID RAY_API Close();
	VOID RAY_API AddMessage(CONST WCHAR* message);
}

class RAY_API SplashWindow : public Win32::Window {

public:
	SplashWindow();
	~SplashWindow();

	virtual	LRESULT	MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;

private:
	WCHAR m_OutputMessage[MAX_NAME_STRING];
};