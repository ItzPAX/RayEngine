#version 430 core

in vec3 vertOutColor;
in vec2 outTexCoord;

out vec4 outColor;

uniform sampler2D ourTexture;

void main()
{
	outColor = texture(ourTexture, outTexCoord) * vec4(vertOutColor, 1);
}