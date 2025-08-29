#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>


class Camera {
public:

    void moveForward(float deltaTime)
    {
         cameraPos += cameraSpeed * cameraFront * deltaTime;
    };
    void moveBackward(float deltaTime)
    {
          cameraPos -= cameraSpeed * cameraFront * deltaTime;
    }
    void moveLeft(float deltaTime)
    {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed* deltaTime;
    }
    void moveRight(float deltaTime)
    {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;
    }

    void rotate(float xoffset, float yoffset)
    {
        xoffset *= m_mouseSensitivity;
        yoffset *= m_mouseSensitivity;

        yaw   += xoffset;
        pitch += yoffset;

        pitch = glm::clamp(pitch, -89.0f, 89.0f);

        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(direction);
    }

    glm::mat4 lookAt() const
    {
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        return view;
    }

    void setMoveSpeed(float metersPerSecond)
    {
        this->m_moveSpeed = metersPerSecond;
    }
    void setMouseSensitivity(float degPerPixel)
    {
        this->m_mouseSensitivity = degPerPixel;
    }

    void setPerspective(float fovY_degrees, float aspect, float nearZ, float farZ);
    void setAspect(float aspect);

private:
    glm::vec3 cameraPos = {0.0f, 0.0f,  3.0f} ;
    glm::vec3 cameraFront = {0.0f, 0.0f, -1.0f} ;
    glm::vec3 cameraUp = {0.0f, 1.0f,  0.0f};
    glm::vec3 direction = {0.0f, 0.0f, 0.0f};

    float yaw = -90.0f;
    float pitch = 0.0f;

    float m_moveSpeed       = 2.5f;             // m/s
    float m_mouseSensitivity= 0.1f;             // deg per mouse unit

    float lastX = 400, lastY = 300;
    const float cameraSpeed = 0.5f;

};
#endif
