#pragma once

namespace Ray  {

	class RAY_API Simulation : public Win32::IApplication, public Win32::Window, public Win32::EngineData {
	public:
		Simulation();
		~Simulation();

		virtual VOID PreInitialize() override;
		virtual LRESULT MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	};
}