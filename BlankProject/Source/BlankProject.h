#pragma once

// Windows API
#include <Windows.h>

// STL Container
#include <string>
#include <list>

// Pointers
#include <memory>

// Time
#include <chrono>

// Resources (icons etc...)
#include "../resource.h"

// Core engine features
#include "Core/Core.h"


// for defining variables used in the game
class Data
{
public:
	VertexArrayObjectPtr m_VAO;
	ShaderProgramPtr m_Shader;
	UniformBufferPtr m_Uniform;

	std::chrono::system_clock::time_point m_PreviousTime;
	float m_Scale = 0;
};

// Data for the uniform buffer
struct UniformData
{
	float m_Scale;
};