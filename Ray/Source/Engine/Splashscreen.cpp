#include "Ray.h"
#include "Splashscreen.h"

#include "Platform/Win32/w32Win32Utils.h"

namespace SplashScreen {

	#define WM_OUTPUTMESSAGE (WM_USER + 0x0001)
	SplashWindow* m_SplashWindow;

	VOID Open()
	{
		if (m_SplashWindow != nullptr)
			return;
		m_SplashWindow = new SplashWindow();
	}

	VOID Close()
	{
		CloseWindow(m_SplashWindow->Handle());
		DestroyWindow(m_SplashWindow->Handle());
	}

	VOID AddMessage(const WCHAR* message)
	{
		Logger::PrintLog(message);
		PostMessage(m_SplashWindow->Handle(), WM_OUTPUTMESSAGE, (WPARAM)message, 0);
	}
}

SplashWindow::SplashWindow()
	: Win32::Window(L"SplashScreen", NULL, Win32::WindowType::POPUP)
{
	wcscpy_s(m_OutputMessage, L"SplashScreen starting...");
	Win32::Window::RegisterNewClass();
	Size(500, 600);
	Win32::Window::Initialize();
}

SplashWindow::~SplashWindow()
{
}

LRESULT SplashWindow::MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		HBITMAP hbitmap = 0;
		HDC hdc, hmemdc = 0;
		PAINTSTRUCT ps;

		hdc = BeginPaint(hwnd, &ps);

		Win32::Utils::AddBitmap(PerGameSettings::SplashURL(), hdc);

		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(255, 255, 255));

		if (Engine::GetMode() != Engine::EngineMode::RELEASE) 
		{
			WSTRING engineModeText = Engine::EngineModeToString() + L" Mode";
			SetTextAlign(hdc, TA_RIGHT);
			TextOut(hdc, (int)Size().cx - 15, 15, engineModeText.c_str(), (int)wcslen(engineModeText.c_str()));
		}

		SetTextAlign(hdc, TA_CENTER);

		TextOut(hdc, (int)Size().cx / 2, (int)Size().cy - 30, m_OutputMessage, (int)wcslen(m_OutputMessage));
		EndPaint(hwnd, &ps);
	}
	break;
	case WM_OUTPUTMESSAGE:
	{
		WCHAR* msg = (WCHAR*)wParam;
		wcscpy_s(m_OutputMessage, msg);
		RedrawWindow();
		return 0;
	}
	break;
	default:
		break;
	}
	return Window::MessageHandler(hwnd, message, wParam, lParam);
}
