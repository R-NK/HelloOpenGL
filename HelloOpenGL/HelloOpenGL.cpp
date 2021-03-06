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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"

#pragma comment(lib, "opengl32.lib")

// プロトタイプ宣言 :-/
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void generateTexture(GLuint&, const std::string&);

// ワールド座標におけるカメラ位置
auto cameraPos = glm::vec3{ 0.0f, 0.0f, 3.0f };
// ワールド座標におけるカメラの向く位置
auto cameraFront = glm::vec3{ 0.0f, 0.0f, -1.0f };
// ワールド座標におけるカメラの上方向
const auto cameraUp = glm::vec3{ 0.0f, 1.0f, 0.0f };

// カメラのピッチ，ヨー
auto pitch = 0.0;
auto yaw = -90.0;

// Field of View
auto fov = 45.0;

auto deltaTime = 0.0f;
auto lastFlame = 0.0f;

// 解像度
const auto screenWidth = 800;
const auto screenHeight = 600;

// 初期のマウスポジション
auto lastX = 400.0, lastY = 300.0;

Camera camera{ screenWidth, screenHeight };

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
    auto*const window(glfwCreateWindow(screenWidth, screenHeight, "Hello OpenGL!", nullptr, nullptr));
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

    // マウスカーソルをキャプチャ
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // マウスカーソルのコールバックを登録
    glfwSetCursorPosCallback(window, mouse_callback);

    // マウスカーソルの初期位置を設定
    glfwSetCursorPos(window, lastX, lastY);

    // マウススクロールのコールバックを登録
    glfwSetScrollCallback(window, scroll_callback);

    // 垂直同期のタイミングを待つ
    glfwSwapInterval(1);

    // 背景色を指定する
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // シェーダオブジェクトの作成
    auto shader = Shader("shaders/point.vert", "shaders/point.frag");

    //static const GLfloat vertices1[] = {
    //    // positions          // colors           // texture coords
    //     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
    //     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
    //    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
    //    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f
    //};

    //const unsigned int indices[] = {
    //    0, 1, 3,
    //    1, 2, 3
    //};

    static const GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    // テクスチャオブジェクトを作成
    GLuint texture;
    generateTexture(texture, "textures/Obo.jpg");
    
    GLuint texture2;
    generateTexture(texture2, "textures/T.jpg");

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

    //// Elementバッファオブジェクト(EBO)を作成
    //GLuint ebo;
    //// EBOを1つ作成しIDをeboに格納
    //glGenBuffers(1, &ebo);
    //// EBOを結合
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // attributeの場所や格納場所を指定
    // STRIDEは次のpositionまでの間隔(attribute * byte)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    // attribute変数を有効にする
    glEnableVertexAttribArray(0);
    // color attribute
    // またposition分オフセットするのでOFFSET=position * byte
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);

    // texture coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // ワイヤーフレームで描画
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // シェーダプログラムの使用開始
    shader.use();
    shader.setAttributeValue("texture1", 0);
    shader.setAttributeValue("texture2", 1);

    // Zバッファを有効化
    glEnable(GL_DEPTH_TEST);


    // ウィンドウが開いている間繰り返す
    while (glfwWindowShouldClose(window) == GL_FALSE)
    {
        //deltaTimeの計算
        const auto currentFlame = static_cast<float>(glfwGetTime());
        deltaTime = currentFlame - lastFlame;
        lastFlame = currentFlame;

        // escキーで終了
        processInput(window);

        // ウィンドウを消去する
        // GL_COLOR_BUFFER_BITを指定するとカラーバッファだけがglCearColor()で指定した色で塗りつぶされる
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // テクスチャを有効化

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        const auto pv = camera.GetViewAndProjection();
        shader.setAttributeValue("pv", pv);

        glBindVertexArray(vao);

        for (auto i = 0; i < 10; i++)
        {
            glm::mat4 model;
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians(20.0f * i), glm::vec3{ 1.0f, 0.3f, 0.5f });
            shader.setAttributeValue("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

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

    const auto cameraSpeed = 1.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProsessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProsessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProsessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProsessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    camera.ProsessMouseCallback(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProsessScrollCallback(xoffset, yoffset);
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
    // 上下逆にロード
    stbi_set_flip_vertically_on_load(true);
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
