#version 430 core

uniform EngineData
{
	mat4 u_ModelViewProj;
};

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec3 color;

out vec3 vertOutColor;
out vec2 outTexCoord;

void main()
{
	gl_Position = u_ModelViewProj * vec4(position, 1);
	vertOutColor = color;
	outTexCoord = texcoord;
}