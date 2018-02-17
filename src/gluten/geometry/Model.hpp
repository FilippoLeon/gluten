#pragma once

#include <vector>
#include <cassert>

#include "gluten/geometry/Mesh.hpp"

#include "gluten/shader/Material.hpp"

#include "gluten/camera/Camera.hpp"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace gluten::geometry {

class Model {
public:
    Model(std::initializer_list<Mesh> meshes, 
               std::initializer_list<shader::Material> materials) 
        : meshes{ meshes }, materials{ materials } {
        sub_models = meshes.size();
        assert(meshes.size() == materials.size() && "One mesh per material!");
    }

    void Draw(camera::Camera & camera) {
        glm::mat4 transform = camera.GetMatrix();
        glm::mat4 projection = camera.GetProjectionMatrix();

        for (int i = 0; i < sub_models; ++i) {
            materials[i].Use(camera);

            materials[i].GetShader().SetUniformLocation("projection", projection);
            materials[i].GetShader().SetUniformLocation("transform", transform);
            materials[i].GetShader().SetUniformLocation("modelView", GetMatrix());
            materials[i].GetShader().SetUniformLocation("jacobianTransform", GetJacobian());

            meshes[i].Draw<>();
        }
    }

    glm::mat4 GetMatrix() {

        glm::mat4 rot = glm::toMat4(rotation);
        return glm::scale(glm::translate(glm::mat4(), position), scale) * rot;
    }

    glm::mat3 GetJacobian() {
        return glm::mat3(glm::transpose(glm::inverse(GetMatrix())));
    }

    int sub_models = 0;

    std::vector<Mesh> meshes;
    std::vector<shader::Material> materials;

    glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
    glm::quat rotation;
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
};

}