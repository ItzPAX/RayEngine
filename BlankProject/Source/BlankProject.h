#pragma once

// Windows API
#include <Windows.h>

// STL Container
#include <string>
#include <list>
#include <vector>

// Pointers
#include <memory>

// Time
#include <chrono>

// Resources (icons etc...)
#include "../resource.h"

// Core engine features
#include "Core/Core.h"

/* DEPRECATED, JUST DO THIS IN SOME HEADER FILE*/
// for defining variables used in the game
struct Data
{
	FrameBufferPtr m_FrameBuffer;
};