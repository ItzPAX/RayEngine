#include "Ray.h"
#include "w32Window.h"

#define DCX_USESTYLE 0x00010000

namespace Win32 {

	Window::Window(WSTRING title, HICON icon, WindowType type)
		: Win32::SubObject(title, title, icon), m_Type(type)
	{
		Size(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	}

	Window::~Window()
	{
	}
	VOID Window::Initialize()
	{
		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);

		RECT R = { 0, 0, m_Size.cx, m_Size.cy };
		AdjustWindowRect(&R, m_Type, false);
		int width = R.right - R.left;
		int height = R.bottom - R.top;

		m_Handle = CreateWindow(m_Class.c_str(), m_Title.c_str(),
			m_Type, ((desktop.right / 2) - (m_Size.cx / 2)), ((desktop.bottom / 2) - (m_Size.cy / 2)), m_Size.cx, m_Size.cy, 0, 0, HInstance(), (void*)this);

		ShowWindow(m_Handle, SW_SHOW);
		UpdateWindow(m_Handle);

		// Creating OpenGL Render Context

		auto hDC = GetDC((HWND)m_Handle);

		int pixelFormatAttributes[] = {
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
			WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB, 32,
			WGL_DEPTH_BITS_ARB, 24,
			WGL_STENCIL_BITS_ARB, 8,
			0
		};

		int pixelFormat = 0;
		UINT numFormats = 0;

		wglChoosePixelFormatARB(hDC, pixelFormatAttributes, nullptr, 1, &pixelFormat, &numFormats);

		PIXELFORMATDESCRIPTOR pixelFormatDesc = {};
		DescribePixelFormat(hDC, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pixelFormatDesc);
		SetPixelFormat(hDC, pixelFormat, &pixelFormatDesc);

		int openGLAttributes[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
			WGL_CONTEXT_MINOR_VERSION_ARB, 6,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0
		};

		m_Context = wglCreateContextAttribsARB(hDC, NULL, openGLAttributes);
	}

	VOID Window::Present(bool bVsync)
	{
		wglSwapIntervalEXT(bVsync);
		wglSwapLayerBuffers(GetDC(Handle()), WGL_SWAP_MAIN_PLANE);
	}

	LRESULT Window::MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_NCCREATE:		{ OnNonClientCreate();										}	return TRUE;
		case WM_NCACTIVATE:		{ OnNonClientActivate(LOWORD(wParam) != WA_INACTIVE);		}   return TRUE;
		case WM_NCPAINT:		{ OnNonClientPaint(reinterpret_cast<HRGN>(wParam));			}	return FALSE;
		case WM_NCLBUTTONDOWN:	{ OnNonClientLeftMouseButtonDown();							}	break;
		case WM_NCLBUTTONDBLCLK:{ Win32::Utils::MaximizeWindow(Handle());					}	return FALSE;
		case WM_SIZE:			{ OnResize();												}	return FALSE;

		case WM_PAINT:			{ OnPaint();												}	break;
		case WM_GETMINMAXINFO:	{ OnGetMinMaxInfo(reinterpret_cast<MINMAXINFO*>(lParam));	}	return FALSE;
		case WM_TIMER:			{ RedrawWindow();											}	break;
		case WM_EXITSIZEMOVE:	{ OnExitSizeMove();											}	break;
		}

		return SubObject::MessageHandler(hwnd, message, wParam, lParam);
	}

	Rect Window::GetInnerSize()
	{
		RECT rc = {};
		GetClientRect(Handle(), &rc);
		return Rect(rc.right - rc.left, rc.bottom - rc.top);
	}

	VOID Window::MakeCurrentContext()
	{
		wglMakeCurrent(GetDC(Handle()), Context());
	}

	VOID Window::OnNonClientCreate()
	{
		SetTimer(Handle(), 1, 100, NULL);
		SetWindowTheme(Handle(), L"", L"");

		Win32::Utils::ModifyClassStyle(Handle(), 0, CS_DROPSHADOW);

		Win32::Caption::AddCaptionButton(new CaptionButton(L"X", CB_CLOSE));
		Win32::Caption::AddCaptionButton(new CaptionButton(L"🗖", CB_MAXIMIZE));
		Win32::Caption::AddCaptionButton(new CaptionButton(L"🗕", CB_MINIMIZE));
	}

	VOID Window::OnNonClientActivate(BOOL active)
	{
		Active(active);
	}

	VOID Window::OnNonClientPaint(HRGN region) 
	{
		// Start draw
		HDC hdc = GetDCEx(Handle(), region, DCX_WINDOW | DCX_INTERSECTRGN | DCX_USESTYLE);

		RECT rect;
		GetWindowRect(Handle(), &rect);

		SIZE size = SIZE{ rect.right - rect.left, rect.bottom - rect.top };

		HBITMAP hbmMem = CreateCompatibleBitmap(hdc, size.cx, size.cy);
		HANDLE hOld = SelectObject(hdc, hbmMem);

		// Draw
		HBRUSH brush = CreateSolidBrush(RGB(46, 46, 46));

		RECT newRect = RECT{ 0, 0, size.cx, size.cy };

		FillRect(hdc, &newRect, brush);

		if (Active()) {
			brush = CreateSolidBrush(RGB(140, 50, 160));
			FrameRect(hdc, &newRect, brush);
		}

		PaintCaption(hdc);

		DeleteObject(brush);

		// End Draw
		BitBlt(hdc, 0, 0, size.cx, size.cy, hdc, 0, 0, SRCCOPY);
		
		SelectObject(hdc, hOld);
		DeleteObject(hbmMem);

		ReleaseDC(Handle(), hdc);
	}

	VOID Window::RedrawWindow() 
	{
		// Reset window
		SetWindowPos(Handle(), 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_DRAWFRAME | SWP_FRAMECHANGED);
		SendMessage(Handle(), WM_PAINT, 0, 0);
	}

	VOID Window::PaintCaption(HDC hdc)
	{
		RECT rect;
		GetWindowRect(Handle(), &rect);

		SIZE size = SIZE{ rect.right - rect.left, rect.bottom - rect.top };

		if (ShowTitle())
		{
			rect = RECT{ 0, 0, size.cx, 30 };

			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, Active() ? RGB(255, 255, 255) : RGB(92, 92, 92));

			DrawText(hdc, m_Title.c_str(), wcslen(m_Title.c_str()), &rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		}

		POINT pt;
		GetCursorPos(&pt);
		GetWindowRect(Handle(), &rect);

		pt.x -= rect.left;
		pt.y -= rect.top;

		int spacing = 0;
		for (auto& button : Caption::CaptionButtons()) {
			spacing += button->m_Width;
			button->m_Rect = RECT{ size.cx - spacing, 0, size.cx - spacing + button->m_Width, 30 };

			// collision check with mouse and box
			if (button->m_Rect.left < pt.x && button->m_Rect.right > pt.x && button->m_Rect.top < pt.y && button->m_Rect.bottom > pt.y)
			{
				HBRUSH brush = CreateSolidBrush(RGB(92, 70, 92));
				FillRect(hdc, &button->m_Rect, brush);

				DeleteObject(brush);
			}

			if (button->m_Text.compare(L"🗖") == 0 && Win32::Utils::IsWindowFullscreen(Handle()))
				button->m_Text = L"🗗";
			else if (button->m_Text.compare(L"🗗") == 0 && !Win32::Utils::IsWindowFullscreen(Handle()))
				button->m_Text = L"🗖";

			DrawText(hdc, button->m_Text.c_str(), wcslen(button->m_Text.c_str()), &button->m_Rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		}
	}

	VOID Window::OnNonClientLeftMouseButtonDown()
	{
		POINT pt;
		GetCursorPos(&pt);

		RECT rect;
		GetWindowRect(Handle(), &rect);

		pt.x -= rect.left;
		pt.y -= rect.top;

		int spacing = 0;
		for (auto& button : Caption::CaptionButtons()) {
			// collision check with mouse and box
			if (button->m_Rect.left < pt.x && button->m_Rect.right > pt.x && button->m_Rect.top < pt.y && button->m_Rect.bottom > pt.y)
			{
				switch (button->m_Command)
				{
				case CB_CLOSE: { SendMessage(Handle(), WM_CLOSE, 0, 0); } break;
				case CB_MINIMIZE: { ShowWindow(Handle(), SW_MINIMIZE); } break;
				case CB_MAXIMIZE: { Win32::Utils::MaximizeWindow(Handle()); } break;
				}
			}
		}
	}

	VOID Window::OnGetMinMaxInfo(MINMAXINFO* minmax) {
		RECT WorkArea; SystemParametersInfo(SPI_GETWORKAREA, 0, &WorkArea, 0);
		minmax->ptMaxSize.x = (WorkArea.right - WorkArea.left);
		minmax->ptMaxSize.y = (WorkArea.bottom - WorkArea.top);
		minmax->ptMaxPosition.x = WorkArea.left;
		minmax->ptMaxPosition.y = WorkArea.top;
		minmax->ptMinTrackSize.x = 400;
		minmax->ptMinTrackSize.y = 300;
	}

	VOID Window::OnExitSizeMove() 
	{
		RECT rect;
		GetWindowRect(Handle(), &rect);
		RECT WorkArea; SystemParametersInfo(SPI_GETWORKAREA, 0, &WorkArea, 0);
		if (rect.top < WorkArea.top + 10 && !Win32::Utils::IsWindowFullscreen(Handle()))
			Win32::Utils::MaximizeWindow(Handle());
	}

	VOID Window::OnPaint()
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(Handle(), &ps);

		RECT rc;
		GetClientRect(Handle(), &rc);

		HBRUSH brush = CreateSolidBrush(RGB(36, 36, 36));

		FillRect(hdc, &rc, brush);

		DeleteObject(brush);

		EndPaint(Handle(), &ps);
	}

	VOID Win32::Window::OnResize()
	{
		Graphics::Instance()->SetViewport(GetInnerSize());
	}
}