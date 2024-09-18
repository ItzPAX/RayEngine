#version 420 core

layout(std140, binding=0) uniform VertexData
{
	mat4 u_ModelViewProj;
	mat4 u_Model;
	mat4 u_Scaling;
};

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;  

out mat3 vertOutTBN;
out vec3 vertOutFragPos;
out vec2 vertOutTexCoords;

void main()
{
	vec3 T = normalize(vec3(u_Model * vec4(tangent,   0.0)));
	vec3 B = normalize(vec3(u_Model * vec4(bitangent, 0.0)));
	vec3 N = normalize(vec3(u_Model * vec4(normal,    0.0)));
	
	vertOutTBN = mat3(T,B,N);
	vertOutFragPos = vec3(u_Model * vec4(position, 1));
	
	vertOutTexCoords = texcoord;
	
	gl_Position = u_Scaling * u_ModelViewProj * vec4(position, 1);
}