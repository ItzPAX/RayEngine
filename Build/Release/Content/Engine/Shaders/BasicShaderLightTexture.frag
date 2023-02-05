#version 330 core

uniform FragmentData
{
	vec3 u_ObjectColor;
	vec3 u_LightColor;
};

uniform sampler2D ourTexture;

in vec2 vertTexCoord;

out vec4 outColor;

void main()
{
	outColor = texture(ourTexture, vertTexCoord) * vec4(u_ObjectColor * u_LightColor, 1);
}