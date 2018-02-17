#pragma once

#include <memory>
#include <vector>

#include "gluten/shader/Shader.hpp"
#include "gluten/texture/Texture.hpp"
#include "gluten/camera/Camera.hpp"
#include "gluten/camera/Light.hpp"

namespace gluten::shader {

class Material {
public:
    Material(ShaderProgram & shader) : shader(shader), textures(2) {
    }

    void SetTexture(int i, std::shared_ptr<texture::ITexture> tex) {
        textured = true;

        textures[i] = tex;
    }

    void Use(camera::Camera & camera) {
        shader.Use();
        shader.SetUniformLocation("viewPosition", camera.position);

        int i = 0;
        for (auto tex : textures) {
            if (tex != nullptr) {
                tex->Bind(i);
            }
            i++;
        }
        if (textures[0] == nullptr) {
            texture::ITexture::Unbind();
        }

        int l = 0;
        for (auto light : lights) {
            shader.SetLight(light, l++);
        }
        shader.SetLight(nullptr, l);

        if (textured) {
            shader.SetUniformLocation("material.diffuse", 0);
            shader.SetUniformLocation("material.specular", 1);
        } else {
            shader.SetUniformLocation("material.diffuseColor", diffuseColor);
            shader.SetUniformLocation("material.specularColor", specularColor);
            shader.SetUniformLocation("material.ambientColor", ambientColor);
        }

        shader.SetUniformLocation("material.shininess", shininess);
    }

    glm::vec3 ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 diffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 specularColor = glm::vec3(1.0f, 1.0f, 1.0f);

    float shininess = 32.0f;

    std::vector<std::shared_ptr<camera::Light>> lights;

    std::vector<std::shared_ptr<texture::ITexture>> textures;

    shader::ShaderProgram & GetShader() const {
        return shader;
    }
private: 
    bool textured = false;

    shader::ShaderProgram & shader;
};

}