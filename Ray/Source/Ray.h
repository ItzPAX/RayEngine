#pragma once

// Windows API
#ifdef WIN32
	#include <Windows.h>
#endif

// STL Container
#include <string>
#include <sstream>
#include <list>
#include <comdef.h>

// error handling
#include <assert.h>
#include <stdexcept>

// Pointers
#include <memory>

// I/O
#include <fstream>
#include <filesystem>

// OpenGL libs
#include <glad/glad_wgl.h>
#include <glad/glad.h>

// Core engine features
#include "Core/Core.h"