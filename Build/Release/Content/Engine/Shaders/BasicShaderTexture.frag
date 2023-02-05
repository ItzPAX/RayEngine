#version 330 core

uniform sampler2D ourTexture;

in vec2 vertTexCoord;
in vec3 vertOutColor;

out vec4 outColor;

void main()
{
	outColor = texture(ourTexture, vertTexCoord) * vec4(vertOutColor, 1);
}