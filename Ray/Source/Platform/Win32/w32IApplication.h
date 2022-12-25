#pragma once

#define ENTRYAPPW32(x) Win32::IApplication* w32EntryApplication() { return new x; }

namespace Win32 {

	class RAY_API IApplication {

		// Application

	public:
		/* Application Constuctor */
		IApplication();

		/* Application Deconstructor */
		virtual ~IApplication() {};


	public:
		/* Called to setup our per game settings  */
		virtual VOID SetupPerGameSettings() = 0;

		/* Called before Initializing the Application  */
		virtual VOID w32PreInitialize() = 0;

		/* Called to Initialize the Application  */
		virtual VOID w32Initialize() = 0;

		/* Game Loop - Called on a loop while the Application is running */
		virtual VOID w32Update() = 0;
	};

	extern IApplication* w32EntryApplication();
}