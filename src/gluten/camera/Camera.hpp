#pragma once

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "gluten/base/Window.hpp"

namespace gluten::camera {

class Rectangle {
public:
    Rectangle(float posx = 0.0f, float posy = 0.0f, float width = 1.0f, float height = 1.0f)
    : posx(posx), posy(posy), width(width), height(height) {

    }

    float AspectRatio() {
        return width / height;
    }

    float posx, posy, width, height;
};

class Camera {
public:
    Camera(const base::Window & win, Rectangle viewportRect = Rectangle()) 
    : window(win), viewportRect(viewportRect) {

    }

    float AspectRatio() {
        return (float) window.Width() / window.Height();
    }

    void Update() {
        glm::mat3 rot = glm::transpose(glm::toMat3(rotation));
        // Move somewhere else
        if (glfwGetKey(window.get(), GLFW_KEY_UP) == GLFW_PRESS
            || glfwGetKey(window.get(), GLFW_KEY_UP) == GLFW_REPEAT) {
            position += translateSpeed * rot * glm::vec3(0.0f, 1.0f, 0.0f);
        }
        if ( glfwGetKey(window.get(), GLFW_KEY_DOWN) == GLFW_PRESS
            || glfwGetKey(window.get(), GLFW_KEY_DOWN) == GLFW_REPEAT) {
            position -= translateSpeed * rot * glm::vec3(0.0f, 1.0f, 0.0f);
        }
        if (glfwGetKey(window.get(), GLFW_KEY_LEFT) == GLFW_PRESS
                   || glfwGetKey(window.get(), GLFW_KEY_LEFT) == GLFW_REPEAT) {
            position -= translateSpeed * rot * glm::vec3(1.0f, 0.0f, 0.0f);
        }
        if (glfwGetKey(window.get(), GLFW_KEY_RIGHT) == GLFW_PRESS
                   || glfwGetKey(window.get(), GLFW_KEY_RIGHT) == GLFW_REPEAT) {
            position += translateSpeed * rot * glm::vec3(1.0f, 0.0f, 0.0f);
        }
        if (glfwGetKey(window.get(), GLFW_KEY_PAGE_UP) == GLFW_PRESS
                   || glfwGetKey(window.get(), GLFW_KEY_R) == GLFW_REPEAT) {
            position -= zoomSpeed * rot * glm::vec3(0.0f, 0.0f, 1.0f);
        }
        if (glfwGetKey(window.get(), GLFW_KEY_PAGE_DOWN) == GLFW_PRESS
                   || glfwGetKey(window.get(), GLFW_KEY_F) == GLFW_REPEAT) {
            position += zoomSpeed * rot * glm::vec3(0.0f, 0.0f, 1.0f);
        }
        // YAW (y-coord normal)
        if (glfwGetKey(window.get(), GLFW_KEY_Q) == GLFW_PRESS) {
            rotation = glm::rotate(rotation,
                                   glm::radians(-1.0f),
                                   glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f))
                                   // rot * to move along tilt
            );
        }
        if (glfwGetKey(window.get(), GLFW_KEY_E) == GLFW_PRESS) {
            rotation = glm::rotate(rotation,
                                   glm::radians(1.0f),
                                   glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f))
                                   // rot * to move along tilt
            );
        }
        // TILT (z-coord normal)
        if (glfwGetKey(window.get(), GLFW_KEY_Z) == GLFW_PRESS) {
            rotation = glm::rotate(rotation,
                                   glm::radians(-1.0f),
                                   glm::normalize(rot * glm::vec3(0.0f, 0.0f, 1.0f))
            );
        }
        if (glfwGetKey(window.get(), GLFW_KEY_U) == GLFW_PRESS) {
            rotation = glm::rotate(rotation,
                                   glm::radians(1.0f),
                                   glm::normalize(rot * glm::vec3(0.0f, 0.0f, 1.0f))
            );
        }
        // PITCH (x-coord normal)
        if (glfwGetKey(window.get(), GLFW_KEY_F) == GLFW_PRESS) {
            rotation = glm::rotate(rotation, 
                                   glm::radians(-1.0f), 
                                   glm::normalize(rot * glm::vec3(1.0f, 0.0f, 0.0f))
            );
        }
        if (glfwGetKey(window.get(), GLFW_KEY_R) == GLFW_PRESS) {
            rotation = glm::rotate(rotation,
                                   glm::radians(1.0f),
                                   glm::normalize(rot * glm::vec3(1.0f, 0.0f, 0.0f))
            );
        }
        std::cout << position.x << "," << position.y << "," << position.z << std::endl;
    }

    glm::vec3 Forward() {
        return glm::transpose(glm::toMat3(rotation)) * glm::vec3(0.0f, 0.0f, -1.0f);
    }

    virtual glm::mat4 GetMatrix() = 0;
public:
    glm::vec3 position;
    glm::quat rotation;

protected:
    const base::Window & window;
    Rectangle viewportRect;

    float distMin = 0.1f;
    float distMax = 100.0f;

    float translateSpeed = 0.05f;
    float zoomSpeed = 0.1f;
};

class CameraPerspective : public Camera {
public:
    CameraPerspective(const base::Window & win, Rectangle viewportRect = Rectangle())
     : Camera(win, viewportRect) { }

    virtual glm::mat4 GetMatrix() {
        glm::mat4 viewMat = glm::translate(glm::mat4(), -position);
        return glm::perspective(fov, AspectRatio(), distMin, distMax) * glm::toMat4(rotation) * viewMat;
    }
private:
    float fov = glm::radians(45.0f);
};

class CameraOrthographic : public  Camera {

    float size;
};

}