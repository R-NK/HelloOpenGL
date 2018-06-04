#version 330 core
out vec4 fragment;

in vec3 vertexColor;
in vec2 TexCord;

uniform sampler2D ourTexture;

void main()
{
	fragment = texture(ourTexture, TexCord);
}