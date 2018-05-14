#pragma once
#include <GL/glew.h>
#include <string>

class Shader
{
public:
	// program ID
	unsigned int ID;

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	// シェーダを有効化
	void use();
	// utility uniform functions
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	~Shader();
private:
	static bool readShaderSource(const GLchar* path, std::string& buffer);
};

