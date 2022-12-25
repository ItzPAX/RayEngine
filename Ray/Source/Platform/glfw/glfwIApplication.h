#pragma once

#define ENTRYAPPGLFW(x) glfw::IApplication* glfwEntryApplication() { return new x; }

namespace glfw {

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
		virtual GLFWwindow* glfwPreInitialize() = 0;

		/* Called to Initialize the Application  */
		virtual VOID glfwInitialize(GLFWwindow* window) = 0;

		/* Game Loop - Called on a loop while the Application is running */
		virtual VOID glfwUpdate(GLFWwindow* window) = 0;
	};

	extern IApplication* glfwEntryApplication();
}