// Fichier : TrackballCamera.hpp

#ifndef TRACKBALLCAMERA_HPP
#define TRACKBALLCAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class TrackballCamera {
private:
    float m_Distance; // Distance par rapport au centre de la scène
    float m_AngleX;   // Angle autour de l'axe X de la scène (rotation vers le haut ou vers le bas)
    float m_AngleY;   // Angle autour de l'axe Y de la scène (rotation vers la gauche ou vers la droite)

public:
    // Constructeur
    TrackballCamera()
        : m_Distance(5.0f), m_AngleX(0.0f), m_AngleY(0.0f) {}

    // Méthode pour avancer/reculer la caméra
    void moveFront(float delta)
    {
        m_Distance += delta;
    }

    // Méthode pour tourner latéralement autour du centre de vision
    void rotateLeft(float degrees)
    {
        m_AngleY += degrees;
    }

    // Méthode pour tourner verticalement autour du centre de vision
    void rotateUp(float degrees)
    {
        m_AngleX += degrees;
    }

    // Méthode pour calculer la ViewMatrix de la caméra
    glm::mat4 getViewMatrix() const
    {
        glm::mat4 view = glm::mat4(1.0f);
        view           = glm::translate(view, glm::vec3(0.0f, 0.0f, -m_Distance));
        view           = glm::rotate(view, glm::radians(m_AngleX), glm::vec3(1.0f, 0.0f, 0.0f));
        view           = glm::rotate(view, glm::radians(m_AngleY), glm::vec3(0.0f, 1.0f, 0.0f));
        return view;
    }
};

#endif // TRACKBALLCAMERA_HPP
