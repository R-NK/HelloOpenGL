#version 330 core
out vec4 fragment;

in vec3 vertexColor;

void main()
{
	fragment = vec4(vertexColor, 1.0);
}