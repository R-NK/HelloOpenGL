// HelloOpenGL.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

#pragma comment(lib, "opengl32.lib")

char readShaderSource(const char* path)
{
	std::ifstream fin(path);
	if (!fin)
	{
		std::cout << "ERROR::Can't read the file::" << path << std::endl;
		throw std::runtime_error("Can't read the file");
	}

	std::stringstream stringstream;
	stringstream << fin.rdbuf();
	fin.close();

	auto data = stringstream.str().data();

	return *data;
}

GLuint createProgram(const char *vsrc, const char *fsrc)
{
	// 空のプログラムオブジェクトを作成する
	const auto program(glCreateProgram());

	int_fast16_t success;
	GLchar infoLog[512];

	if (vsrc != NULL)
	{
		// バーテックスシェーダのシェーダオブジェクトを作成する
		const auto vobj(glCreateShader(GL_VERTEX_SHADER));
		glShaderSource(vobj, 1, &vsrc, NULL);
		glCompileShader(vobj);

		// コンパイルチェック
		glGetShaderiv(vobj, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vobj, 512, nullptr, infoLog);
			std::cout << "ERROR::Shader::Vertex::Compilation_Failed\n" << infoLog << std::endl;
		}
		else
		{
			std::cout << "SUCCESS::Shader::Vertex::Compilation_Success\n" << std::endl;
		}

		// バーテックスシェーダのシェーダオブジェクトをプログラムオブジェクトに組み込む
		glAttachShader(program, vobj);
		glDeleteShader(vobj);
	}

	if (fsrc != NULL)
	{
		// フラグメントシェーダのシェーダオブジェクトを作成する
		const auto fobj(glCreateShader(GL_FRAGMENT_SHADER));
		glShaderSource(fobj, 1, &fsrc, NULL);
		glCompileShader(fobj);

		// コンパイルチェック
		glGetShaderiv(fobj, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fobj, 512, nullptr, infoLog);
			std::cout << "ERROR::Shader::Fragment::Compilation_Failed\n" << infoLog << std::endl;
		}
		else
		{
			std::cout << "SUCCESS::Shader::Fragment::Compilation_Success\n" << std::endl;
		}

		// フラグメントシェーダのシェーダオブジェクトをプログラムオブジェクトに組み込む
		glAttachShader(program, fobj);
		glDeleteShader(fobj);
	}

	// プログラムオブジェクトをリンクする
	glLinkProgram(program);

	// 作成したプログラムオブジェクトを返す
	return program;
}

int main()
{
	// GLFW を初期化する
	if (glfwInit() == GL_FALSE)
	{
		// 初期化に失敗した
		std::cerr << "Can't initialize GLFW" << std::endl;
		return 1;
	}

	// プログラム終了時の処理を登録する
	atexit(glfwTerminate);

	// OpenGL Version 3.3 Core Profile を選択する
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// ウィンドウを作成する
	auto *const window(glfwCreateWindow(640, 480, "Hello!", NULL, NULL));
	if (window == NULL)
	{
		// ウィンドウが作成できなかった
		std::cerr << "Can't create GLFW window." << std::endl;
		return 1;
	}
	// 作成したウィンドウを OpenGL の処理対象にする
	glfwMakeContextCurrent(window);
	// GLEW を初期化する
	//glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		// GLEW の初期化に失敗した
		std::cerr << "Can't initialize GLEW" << std::endl;
		return 1;
	}

	// 垂直同期のタイミングを待つ
	glfwSwapInterval(1);

	// 背景色を指定する
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	const auto vsrc(readShaderSource("point.vert"));

	/*static constexpr GLchar vsrc[] =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
		"}\n";*/

	// fragmentはRGBA
	static constexpr GLchar fsrc[] =
		"#version 330 core\n"
		"out vec4 fragment;\n"
		"void main()\n"
		"{\n"
		" fragment = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n";

	// プログラムオブジェクトを作成する
	const auto program(createProgram(&vsrc, fsrc));

	static const GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f,
	};

	// 頂点配列オブジェクト(VAO)を作成
	GLuint vao;
	// VAOを１つ作成しIDをvaoに格納
	glGenVertexArrays(1, &vao);
	// VAOを結合
	glBindVertexArray(vao);

	// 頂点バッファオブジェクト(VBO)を作成
	GLuint vbo;
	// VBOを1つ作成しIDをvboに格納
	glGenBuffers(1, &vbo);
	// VBOを結合
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// VBOのメモリを確保し，頂点データをコピー
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// attributeの場所や格納場所を指定
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// attribute変数を有効にする
	glEnableVertexAttribArray(0);

	// ウィンドウが開いている間繰り返す
	while (glfwWindowShouldClose(window) == GL_FALSE)
	{
		// ウィンドウを消去する
		// GL_COLOR_BUFFER_BITを指定するとカラーバッファだけがglCearColor()で指定した色で塗りつぶされる
		glClear(GL_COLOR_BUFFER_BIT);

		// シェーダプログラムの使用開始
		glUseProgram(program);

		// ここで描画処理を行う

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//
		// カラーバッファを入れ替える
		glfwSwapBuffers(window);
		// イベントを取り出す
		glfwWaitEvents();
	}
}

