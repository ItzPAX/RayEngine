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

/* DEPRECATED, JUST DO THIS IN SOME HEADER FILE*/
// for defining variables used in the game
class Data
{
public:
	VertexArrayObjectPtr m_VAO;
	ShaderProgramPtr m_Shader;
	UniformBufferPtr m_Uniform;
	UIPtr m_UI;

	std::chrono::system_clock::time_point m_PreviousTime;
	float m_Scale = 0;
};

// Data for the uniform buffer
struct UniformData
{
	Mat4 m_World;
	Mat4 m_Projection;
};

struct Vertex
{
	Vec3D m_Position;
	Vec2D m_TexCoord;
};