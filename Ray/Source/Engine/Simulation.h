#pragma once

namespace Ray  {

	class RAY_API Simulation : public Win32::IApplication, public Win32::Window, public glfw::IApplication, public glfw::Window {
	public:
		Simulation();
		~Simulation();

		// WinAPI (DirectX)
		virtual VOID w32PreInitialize() override;
		virtual LRESULT MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;

		//GLFW (OpenGL)
		virtual GLFWwindow* glfwPreInitialize() override;
	};
}