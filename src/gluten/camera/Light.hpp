#pragma once

#include <glm/glm.hpp>

namespace gluten::camera {

class Light {
public:
    Light() = default;

    glm::vec3 direction;
    glm::vec3 position;

    glm::vec3 ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 diffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 specularColor = glm::vec3(1.0f, 1.0f, 1.0f);

};


class SpotLight : public Light {
public:

};

class FarLight : public Light {
public:

};

class BulbLight : public Light {
public:

};


}