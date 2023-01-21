#pragma once
#include "w32SubObject.h"

#include <Uxtheme.h>
#pragma comment(lib, "uxtheme.lib")

namespace Win32 {

	class RAY_API Window : public Win32::SubObject, public Win32::Caption {

	public:
		Window(WSTRING title, HICON icon, WindowType type = RESIZEABLE);
		~Window();

		virtual VOID Initialize() override;
		virtual VOID Present(bool bVsync) override;
		virtual	LRESULT	MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;

		VOID SetTitle(LPCWSTR title);
		glm::vec4 GetInnerSize();
		VOID MakeCurrentContext();

		// Message Handler functions
		VOID OnNonClientCreate();
		VOID OnNonClientActivate(BOOL active);
		VOID OnNonClientPaint(HRGN region);
		VOID RedrawWindow();
		VOID OnNonClientLeftMouseButtonDown();
		VOID OnGetMinMaxInfo(MINMAXINFO* minmax);
		VOID OnExitSizeMove();
		VOID OnPaint();
		VOID OnResize();
		VOID OnInput(WPARAM wParam, LPARAM lParam);

		// Window Title etc.
		VOID PaintCaption(HDC hdc);

	protected:
		SIZE		m_Size;
		WindowType	m_Type;

		BOOL		m_Active;

		HGLRC		m_Context;
		HDC			m_DC;

		glm::vec2	m_MouseSpeed;

		// Getters
	public:
		SIZE		Size() { return m_Size; }
		BOOL		Active() { return m_Active; }
		HGLRC		Context() { return m_Context; }
		glm::vec2*	MouseSpeed() { return &m_MouseSpeed; }

		// Setters
	public:
		VOID		Size(SIZE size) { m_Size = size; }
		VOID		Size(INT cx, INT cy) { m_Size.cx = cx; m_Size.cy = cy; }
		VOID		Active(BOOL active) { m_Active = active; }
		VOID		Context(HGLRC context) { m_Context = context; }
	};
}