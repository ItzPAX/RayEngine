#pragma once

/*-------------------*/
/*   CORE FEATURES   */
/*      BAREBONE     */
/*-------------------*/

// defines the engine needs
#include "CoreDefinitions.h"

// Maths
#include "Common/Math/Vec4D.h"
#include "Common/Math/Triangle.h"

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