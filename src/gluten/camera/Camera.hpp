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
            position += 0.01f * rot * glm::vec3(0.0f, 1.0f, 0.0f);
        }
        if ( glfwGetKey(window.get(), GLFW_KEY_DOWN) == GLFW_PRESS
            || glfwGetKey(window.get(), GLFW_KEY_DOWN) == GLFW_REPEAT) {
            position -= 0.01f * rot * glm::vec3(0.0f, 1.0f, 0.0f);
        }
        if (glfwGetKey(window.get(), GLFW_KEY_LEFT) == GLFW_PRESS
                   || glfwGetKey(window.get(), GLFW_KEY_LEFT) == GLFW_REPEAT) {
            position -= 0.01f * rot * glm::vec3(1.0f, 0.0f, 0.0f);
        }
        if (glfwGetKey(window.get(), GLFW_KEY_RIGHT) == GLFW_PRESS
                   || glfwGetKey(window.get(), GLFW_KEY_RIGHT) == GLFW_REPEAT) {
            position += 0.01f * rot * glm::vec3(1.0f, 0.0f, 0.0f);
        }
        if (glfwGetKey(window.get(), GLFW_KEY_R) == GLFW_PRESS
                   || glfwGetKey(window.get(), GLFW_KEY_R) == GLFW_REPEAT) {
            position -= 0.01f * rot * glm::vec3(0.0f, 0.0f, 1.0f);
        }
        if (glfwGetKey(window.get(), GLFW_KEY_F) == GLFW_PRESS
                   || glfwGetKey(window.get(), GLFW_KEY_F) == GLFW_REPEAT) {
            position += 0.01f * rot * glm::vec3(0.0f, 0.0f, 1.0f);
        // YAW (y-coord normal)
        }
        if (glfwGetKey(window.get(), GLFW_KEY_Q) == GLFW_PRESS) {
            rotation *= glm::angleAxis(glm::radians(-1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        if (glfwGetKey(window.get(), GLFW_KEY_E) == GLFW_PRESS) {
            rotation *= glm::angleAxis(glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        std::cout << position.x << "," << position.y << "," << position.z << std::endl;
    }

protected:
    glm::vec3 position;
    glm::quat rotation;

    const base::Window & window;
    Rectangle viewportRect;

    float distMin = 0.1f;
    float distMax = 100.0f;
};

class CameraPerspective : public Camera {
public:
    CameraPerspective(const base::Window & win, Rectangle viewportRect = Rectangle())
     : Camera(win, viewportRect) { }

    glm::mat4 GetMatrix() {
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