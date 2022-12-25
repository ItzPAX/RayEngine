#pragma once

/*-------------------*/
/*   CORE FEATURES   */
/*      BAREBONE     */
/*-------------------*/

// defines the engine needs
#include "CoreDefinitions.h"

// Critical engine features
#include "Engine/REngine.h"

// Commonly used headers
#include "Common/Logger.h"
#include "Common/Time.h"

// Core headers
#include "Core/PerGameSettings.h"

// Platform dependant stuff [Win32]
#ifdef WIN32
	#include "Platform/Win32/w32Win32Utils.h"
	#include "Platform/Win32/w32SubObject.h"
	#include "Platform/Win32/w32Caption.h"
	#include "Platform/Win32/w32Window.h"
	#include "Platform/Win32/w32IApplication.h"
#endif

// Platform dependant stuff [glfw]
#ifdef OpenGL
	#include <glad/glad.h>
	#pragma comment(lib, "opengl32.lib")

	#include <GLFW/glfw3.h>
	#pragma comment(lib, "glfw3.lib")

	#include "Platform/glfw/glfwGlfwUtils.h"
	#include "Platform/glfw/glfwSubObject.h"
	#include "Platform/glfw/glfwCaption.h"
	#include "Platform/glfw/glfwWindow.h"
	#include "Platform/glfw/glfwIApplication.h"
#endif