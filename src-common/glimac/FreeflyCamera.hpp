#ifndef FREEFLYCAMERA_HPP
#define FREEFLYCAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

class FreeflyCamera {
private:
    glm::vec3 m_Position;
    float     m_Phi;
    float     m_Theta;
    glm::vec3 m_FrontVector;
    glm::vec3 m_LeftVector;
    glm::vec3 m_UpVector;

    void computeDirectionVectors()
    {
        m_FrontVector = glm::vec3(
            cos(m_Theta) * sin(m_Phi),
            sin(m_Theta),
            cos(m_Theta) * cos(m_Phi)
        );

        m_LeftVector = glm::vec3(
            sin(m_Phi + glm::pi<float>() / 2.0f),
            0,
            cos(m_Phi + glm::pi<float>() / 2.0f)
        );

        m_UpVector = glm::cross(m_FrontVector, m_LeftVector);
    }

public:
    FreeflyCamera()
        : m_Position(glm::vec3(0.0f)), m_Phi(glm::pi<float>()), m_Theta(0.0f)
    {
        computeDirectionVectors();
    }

    void moveLeft(float t)
    {
        m_Position += t * m_LeftVector;
    }

    void moveFront(float t)
    {
        m_Position += t * m_FrontVector;
    }

    void rotateLeft(float degrees)
    {
        m_Phi += glm::radians(degrees);
        computeDirectionVectors();
    }

    void rotateUp(float degrees)
    {
        m_Theta += glm::radians(degrees);
        computeDirectionVectors();
    }

    glm::mat4 getViewMatrix() const
    {
        return glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
    }
};

#endif // FREEFLYCAMERA_HPP
