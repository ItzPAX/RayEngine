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
class Data
{
public:
	VertexArrayObjectPtr m_VAO;
	ShaderProgramPtr m_Shader;
	UniformBufferPtr m_Uniform;
	FrameBufferPtr m_FrameBuffer;
	FloatingCameraPtr m_FloatingCamera;
	UIPtr m_UI;

	float m_Scale;
	bool m_SetMouse;
};

// Data for the uniform buffer
struct UniformData
{
	glm::mat4 u_ModelViewProj;
};


struct Vertex
{
	glm::vec3 m_Position;
	glm::vec2 m_Color;
};