#include "stdafx.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>


Camera::Camera(const int_fast16_t screenWidth, const int_fast16_t screenHeight)
{
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    updateCameraVectors();
}


Camera::~Camera()
{
}

Camera::Camera(const Camera&){}

void Camera::operator=(const Camera& other){}

glm::mat4 Camera::GetViewAndProjection()
{
    const auto view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    const auto projection = glm::perspective(static_cast<float>(glm::radians(fov)), static_cast<float>(screenWidth / screenHeight), 0.1f, 100.0f);
    return projection * view;
}

void Camera::ProsessKeyboard(const Camera_Movement direction, float deltaTime)
{
    const auto velocity = movementSpeed * deltaTime;
    if (direction == FORWARD)
        cameraPos += cameraFront * velocity;
    if (direction == BACKWARD)
        cameraPos -= cameraFront * velocity;
    if (direction == LEFT)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * velocity;
    if (direction == RIGHT)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * velocity;
}

void Camera::ProsessMouseCallback(double xpos, double ypos)
{
    auto xoffset = xpos - lastX;
    auto yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // ピッチを±89度に制限
    if (pitch > 89.0f)
    {
        pitch = 89.0f;
    }
    if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::ProsessScrollCallback(double xoffset, double yoffset)
{
    if (fov >= 1.0f && fov <= 45.0f)
    {
        fov -= yoffset;
    }
    if (fov <= 1.0f)
    {
        fov = 1.0f;
    }
    if (fov >= 45.0f)
    {
        fov = 45.0f;
    }
}

void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = static_cast<float>(cos(glm::radians(pitch)) * cos(glm::radians(yaw)));
    front.y = static_cast<float>(sin(glm::radians(pitch)));
    front.z = static_cast<float>(cos(glm::radians(pitch)) * sin(glm::radians(yaw)));
    cameraFront = glm::normalize(front);
}