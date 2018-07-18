#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera
{
public:
    //解像度
    int_fast16_t screenWidth;
    int_fast16_t screenHeight;

    // ワールド座標におけるカメラ位置
    glm::vec3 cameraPos = glm::vec3{ 0.0f, 0.0f, 3.0f };;
    // ワールド座標におけるカメラの向く位置
    glm::vec3 cameraFront = glm::vec3{ 0.0f, 0.0f, -1.0f };
    // ワールド座標におけるカメラの上方向
    glm::vec3 cameraUp = glm::vec3{ 0.0f, 1.0f, 0.0f };

    float pitch = 0.0f;
    float yaw = -90.0f;

    // Field of View
    float fov = 45.0f;

    // 初期のマウスポジション
    float lastX = 400.0f;
    float lastY = 300.0f;

    // マウス感度
    float sensitivity = 0.05f;

    // 移動速度
    float movementSpeed = 1.5f;

    Camera(const int_fast16_t screenWidth, const int_fast16_t screenHeight);
    ~Camera();

    glm::mat4 GetViewAndProjection();

    void ProsessKeyboard(Camera_Movement direction, float deltaTime);

    void ProsessMouseCallback(double xpos, double ypos);

    void ProsessScrollCallback(double xoffset, double yoffset);

private:
    // コピー禁止
    Camera(const Camera&);
    void operator=(const Camera &other);

    void updateCameraVectors();
};

