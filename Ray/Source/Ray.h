#pragma once

// Windows API
#ifdef WIN32
	#include <Windows.h>
	#include <windowsx.h>
#endif

// STL Container
#include <string>
#include <sstream>
#include <list>
#include <comdef.h>
#include <deque>
#include <unordered_map>

// error handling
#include <assert.h>
#include <stdexcept>

// Pointers
#include <memory>

// I/O
#include <fstream>
#include <filesystem>
#include <direct.h>
#include <thread>

// OpenGL libs
#include <glad/glad_wgl.h>
#include <glad/glad.h>

// Time
#include <chrono>

// Core engine features
#include "Core/Core.h"