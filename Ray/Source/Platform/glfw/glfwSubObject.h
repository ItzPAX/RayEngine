#pragma once

namespace glfw {

	class RAY_API SubObject {

	public:
		SubObject(STRING classTitle, HICON icon);
		~SubObject();

		// initialize window
		virtual VOID Initialize() = 0;

	protected:
		HICON				m_Icon;
		STRING				m_Title;
		GLFWwindow*			m_Handle;

		// Getters
	public:
		GLFWwindow*			Handle() { return m_Handle; }

		// Setters
	public:
		VOID				Handle(GLFWwindow* hwnd) { m_Handle = hwnd; }
	};
}