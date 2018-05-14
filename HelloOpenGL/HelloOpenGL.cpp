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
#include <vector>

#pragma comment(lib, "opengl32.lib")

// プロトタイプ宣言 :-/
void processInput(GLFWwindow *window);
GLuint createProgram(const char *vsrc, const char *fsrc);
bool readShaderSource(const char* path, std::string& buffer);

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
	auto *const window(glfwCreateWindow(640, 480, "Hello!", nullptr, nullptr));
	if (window == nullptr)
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

	// シェーダのソースを読み込む
	std::string vsrc;
	readShaderSource("shaders/point.vert", vsrc);

	std::string fsrc;
	readShaderSource("shaders/point.frag", fsrc);

	// プログラムオブジェクトを作成する
	const auto program(createProgram(vsrc.data(), fsrc.data()));

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	// attribute変数を有効にする
	glEnableVertexAttribArray(0);

	// ウィンドウが開いている間繰り返す
	while (glfwWindowShouldClose(window) == GL_FALSE)
	{
		// escキーで終了
		processInput(window);

		// ウィンドウを消去する
		// GL_COLOR_BUFFER_BITを指定するとカラーバッファだけがglCearColor()で指定した色で塗りつぶされる
		glClear(GL_COLOR_BUFFER_BIT);

		// シェーダプログラムの使用開始
		glUseProgram(program);

		// ここで描画処理を行う
		const auto timeValue = glfwGetTime();
		const auto greanValue = std::sin(timeValue) / 2.0f + 0.5f;
		const auto vertexColorLocation = glGetUniformLocation(program, "vertexColor");
		glUniform4f(vertexColorLocation, 0.0f, greanValue, 0.0f, 1.0f);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// カラーバッファを入れ替える
		glfwSwapBuffers(window);
		// イベントを取り出す
		//glfwWaitEvents();
		glfwPollEvents();
	}
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

GLuint createProgram(const char *vsrc, const char *fsrc)
{
	// 空のプログラムオブジェクトを作成する
	const auto program(glCreateProgram());

	int_fast16_t success;
	GLchar infoLog[512];

	if (vsrc != nullptr)
	{
		// バーテックスシェーダのシェーダオブジェクトを作成する
		const auto vobj(glCreateShader(GL_VERTEX_SHADER));
		glShaderSource(vobj, 1, &vsrc, nullptr);
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

	if (fsrc != nullptr)
	{
		// フラグメントシェーダのシェーダオブジェクトを作成する
		const auto fobj(glCreateShader(GL_FRAGMENT_SHADER));
		glShaderSource(fobj, 1, &fsrc, nullptr);
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

bool readShaderSource(const char* path, std::string& buffer)
{
	// pathのNULLチェック
	if (path == nullptr)
	{
		std::cout << "ERROR::1st argument is nullptr.\n" << std::endl;
		return false;
	}
	std::ifstream fin(path, std::ios::in);
	if (!fin)
	{
		std::cout << "ERROR::Can't read the file::" << path << std::endl;
		return false;
	}

	std::stringstream stringstream;
	stringstream << fin.rdbuf();
	fin.close();

	buffer = stringstream.str();

	return true;
}


