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
    //�𑜓x
    int_fast16_t screenWidth;
    int_fast16_t screenHeight;

    // ���[���h���W�ɂ�����J�����ʒu
    glm::vec3 cameraPos = glm::vec3{ 0.0f, 0.0f, 3.0f };;
    // ���[���h���W�ɂ�����J�����̌����ʒu
    glm::vec3 cameraFront = glm::vec3{ 0.0f, 0.0f, -1.0f };
    // ���[���h���W�ɂ�����J�����̏����
    glm::vec3 cameraUp = glm::vec3{ 0.0f, 1.0f, 0.0f };

    float pitch = 0.0f;
    float yaw = -90.0f;

    // Field of View
    float fov = 45.0f;

    // �����̃}�E�X�|�W�V����
    float lastX = 400.0f;
    float lastY = 300.0f;

    // �}�E�X���x
    float sensitivity = 0.05f;

    // �ړ����x
    float movementSpeed = 1.5f;

    Camera(const int_fast16_t screenWidth, const int_fast16_t screenHeight);
    ~Camera();

    glm::mat4 GetViewAndProjection();

    void ProsessKeyboard(Camera_Movement direction, float deltaTime);

    void ProsessMouseCallback(double xpos, double ypos);

    void ProsessScrollCallback(double xoffset, double yoffset);

private:
    // �R�s�[�֎~
    Camera(const Camera&);
    void operator=(const Camera &other);

    void updateCameraVectors();
};

