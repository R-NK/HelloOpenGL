#include "stdafx.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Camera::Camera()
{
    updateCameraVectors();
}


Camera::~Camera()
{
}

Camera::Camera(const Camera&){}

void Camera::operator=(const Camera& other){}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::ProsessKeyboard(const Camera_Movement direction, float deltaTime)
{
    if (direction == FORWARD)
        cameraPos += sensitivity * cameraFront;
    if (direction == BACKWARD)
        cameraPos -= sensitivity * cameraFront;
    if (direction == LEFT)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraFront;
    if (direction == RIGHT)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraFront;
}

void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    cameraFront = glm::normalize(front);
}