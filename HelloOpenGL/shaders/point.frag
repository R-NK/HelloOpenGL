#version 330 core
out vec4 fragment;

in vec2 TexCord;

uniform sampler2D texture_diffuse1;

void main()
{
	fragment = texture(texture_diffuse1, TexCord);
}