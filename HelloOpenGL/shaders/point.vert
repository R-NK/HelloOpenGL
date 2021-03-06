#version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aColor;
layout (location = 1) in vec2 aTexCord;

out vec3 vertexColor;
out vec2 TexCord;

uniform mat4 model;
uniform mat4 pv;

void main()
{
	gl_Position = pv * model * vec4(aPos, 1.0);
	vertexColor = vec3(1.0f, 1.0f, 1.0f);
	TexCord = aTexCord;
}