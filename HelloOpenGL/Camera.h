#pragma once
#include <glm/detail/type_vec3.hpp>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>

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

    Camera();
    ~Camera();

    glm::mat4 GetViewMatrix();
    void ProsessKeyboard(Camera_Movement direction, float deltaTime);

private:
    // �R�s�[�֎~
    Camera(const Camera&);
    void operator=(const Camera &other);

    void updateCameraVectors();
};

