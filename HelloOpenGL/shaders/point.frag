#version 330 core
out vec4 fragment;

in vec4 vertexColor;

void main()
{
	//fragment = vec4(1.0, 0.5, 0.2, 1.0);
	fragment = vertexColor;
}