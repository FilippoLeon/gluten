#pragma once

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "gluten/camera/Camera.hpp"

namespace gluten::camera {

class Light {
public:
    Light() = default;

    virtual std::shared_ptr<Camera> GetCamera() = 0;

    virtual void SetPosition(glm::vec3 position) = 0;
    virtual void SetDirection(glm::vec3 direction) = 0;

    glm::vec3 ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 diffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 fade = glm::vec3(1.0f, 0.09f, 0.032f);


    glm::vec2 cutoff = glm::vec2(0.99, 0.98);

    int type;

    glm::vec3 GetPosition() {
        return position;
    }

    glm::vec3 GetDirection() {
        return direction;
    }

    virtual glm::mat4 GetViewMatrix() = 0;
protected:
    glm::vec3 position;
    glm::vec3 direction;
};

class PointLight : public Light {
public:
    PointLight() {
        type = 0;
        cam = std::make_shared<CameraPerspective>();
    }

    virtual void SetPosition(glm::vec3 position) {
        this->position = position;
        cam->position = position;

    }

    virtual void SetDirection(glm::vec3 direction) {
        this->direction = direction;
        cam->rotation = glm::rotation(glm::vec3(1, 0, 0), direction);
    }

    std::shared_ptr<Camera> GetCamera() {
        return cam;
    }

    virtual glm::mat4 GetViewMatrix() {
        return cam->GetProjectionMatrix() * cam->GetMatrix();
    }

    std::shared_ptr<Camera> cam;
};

class DirectionalLight : public Light {
public:
    DirectionalLight() {
        type = 1;
        //cam = std::make_shared<CameraPerspective>();
        cam = std::make_shared<CameraOrthographic>(Rectangle(-40,-40,40,40), 1, 50);
    }

    std::shared_ptr<Camera> GetCamera() {
        return cam;
    }

    virtual void SetPosition(glm::vec3 position) {
        this->position = position;
        cam->position = position;
    }

    virtual void SetDirection(glm::vec3 direction) {
        cam->rotation = glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0, 0, 0));
        //cam->rotation = glm::rotation(glm::vec3(1, 0, 0), direction);
        //cam->rotation = glm::rotation(glm::vec3(0, 0, 1), direction);
        cam->rotation = glm::rotation(glm::vec3(0.0, 0, -1.0), direction);
        this->direction = direction;
        //cam->rotation = glm::rotation(glm::vec3(1.0, 0.0, 0), direction);
    }

    virtual glm::mat4 GetViewMatrix() {
        return cam->GetMatrix();
    }

    std::shared_ptr<Camera> cam;
};

class SpotLight : public Light {
public:
    SpotLight() {
        type = 2;
        cam = std::make_shared<CameraPerspective>();
    }

    std::shared_ptr<Camera> GetCamera() {
        return cam;
    }

    virtual void SetPosition(glm::vec3 position) {
        this->position = position;
        cam->position = position;
    }

    virtual void SetDirection(glm::vec3 direction) {
        this->direction = direction;
        cam->rotation = glm::rotation(glm::vec3(1,0,0), direction);
    }

    virtual glm::mat4 GetViewMatrix() {
        return cam->GetProjectionMatrix() * cam->GetMatrix();
    }

    std::shared_ptr<Camera> cam;
};

}