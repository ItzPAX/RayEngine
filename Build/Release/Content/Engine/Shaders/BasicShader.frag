#version 420 core

uniform	vec3 u_ObjectColor;

out vec4 color;

void main()
{
	color = vec4(u_ObjectColor, 1);
}