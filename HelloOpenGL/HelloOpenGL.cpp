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
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#pragma comment(lib, "opengl32.lib")

// プロトタイプ宣言 :-/
void processInput(GLFWwindow* window);
void generateTexture(GLuint&, const std::string&);

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
    auto*const window(glfwCreateWindow(640, 480, "Hello OpenGL!", nullptr, nullptr));
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

    // シェーダオブジェクトの作成
    auto shader = Shader("shaders/point.vert", "shaders/point.frag");

    static const GLfloat vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f
    };

    const unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    // テクスチャオブジェクトを作成
    GLuint texture;
    generateTexture(texture, "textures/container.jpg");
    glBindTexture(GL_TEXTURE_2D, texture);

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

    // Elementバッファオブジェクト(EBO)を作成
    GLuint ebo;
    // EBOを1つ作成しIDをeboに格納
    glGenBuffers(1, &ebo);
    // EBOを結合
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // attributeの場所や格納場所を指定
    // STRIDEは次のpositionまでの間隔(attribute * byte)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    // attribute変数を有効にする
    glEnableVertexAttribArray(0);
    // color attribute
    // またposition分オフセットするのでOFFSET=position * byte
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // ワイヤーフレームで描画
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // ウィンドウが開いている間繰り返す
    while (glfwWindowShouldClose(window) == GL_FALSE)
    {
        // escキーで終了
        processInput(window);

        // ウィンドウを消去する
        // GL_COLOR_BUFFER_BITを指定するとカラーバッファだけがglCearColor()で指定した色で塗りつぶされる
        glClear(GL_COLOR_BUFFER_BIT);

        // シェーダプログラムの使用開始
        shader.use();

        // ここで描画処理を行う
        /*const auto timeValue = glfwGetTime();
        const auto greanValue = std::sin(timeValue) / 2.0f + 0.5f;
        const auto vertexColorLocation = glGetUniformLocation(program, "vertexColor");
        glUniform4f(vertexColorLocation, 0.0f, greanValue, 0.0f, 1.0f);*/

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glBindVertexArray(0);

        // カラーバッファを入れ替える
        glfwSwapBuffers(window);
        // イベントを取り出す
        //glfwWaitEvents();
        glfwPollEvents();
    }
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void generateTexture(GLuint& texture_object, const std::string& path)
{
    glGenTextures(1, &texture_object);
    glBindTexture(GL_TEXTURE_2D, texture_object);

    // テクスチャのラップ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // 拡大と縮小時のフィルタ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int_fast16_t width, height, nr_channels;
    auto* data = stbi_load(path.data(), &width, &height, &nr_channels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}
