#version 330 core

uniform VertexData
{
	mat4 u_ModelViewProj;
	mat4 u_Model;
};

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec3 normal;

out vec3 vertOutNormal;
out vec3 vertOutFragPos;

void main()
{
	gl_Position = u_ModelViewProj * vec4(position, 1);
	vertOutNormal = mat3(transpose(inverse(u_Model))) * normal; // calc on cpu
	vertOutFragPos = vec3(u_Model * vec4(position, 1));
}