#version 430 core

uniform EngineData
{
	mat4 u_ModelViewProj;
};

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;

layout(location = 0) out vec3 vertOutColor;

void main()
{
	gl_Position = u_ModelViewProj * vec4(position, 1);
	vertOutColor = vec3(texcoord.x,texcoord.y,1);
}