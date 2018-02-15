#pragma once

#include <glm/glm.hpp>

namespace gluten::camera {

class Light {
public:
    Light() = default;

    glm::vec3 ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 diffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 fade = glm::vec3(1.0f, 0.09f, 0.032f);

    glm::vec3 position;
    glm::vec3 direction;

    glm::vec2 cutoff = glm::vec2(0.99, 0.98);

    int type;
};

class PointLight : public Light {
public:
    PointLight() {
        type = 0;
    }

};

class DirectionalLight : public Light {
public:
    DirectionalLight() {
        type = 1;
    }

};

class SpotLight : public Light {
public:
    SpotLight() {
        type = 2;
    }
};

}