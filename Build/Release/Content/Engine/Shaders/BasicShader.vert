#version 420 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;  

out vec2 TexCoords;

layout(std140, binding=0) uniform VertexData
{
	mat4 u_ModelViewProj;
	mat4 u_Model;
	mat4 u_Scaling;
};

void main()
{
    TexCoords = texcoord;    
    gl_Position = u_Scaling * u_ModelViewProj * vec4(position, 1);
}