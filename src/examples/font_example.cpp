#include <random>

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "gluten/base/Application.hpp"
#include "gluten/base/Window.hpp"
#include "gluten/base/Color.hpp"

#include "gluten/shader/Shader.hpp"
#include "gluten/shader/Material.hpp"

#include "gluten/geometry/Mesh.hpp"
#include "gluten/geometry/Model.hpp"

#include "gluten/texture/Texture.hpp"

#include "gluten/camera/Camera.hpp"
#include "gluten/camera/Light.hpp"

#include "gluten/fonts/Font.hpp"

int main() {
    gluten::base::Application app;

    gluten::base::Window window(1000, 1000, "Win");

    gluten::shader::VertexShader text_vs({ "TEXTURE" }, app.GlutenRoot("/shader/examples/text.vs"));
    gluten::shader::FragmentShader text_fs({ "TEXTURE" }, app.GlutenRoot("/shader/examples/text.fs"));
    gluten::shader::ShaderProgram text_shader(text_vs, text_fs);
    gluten::fonts::Font font("Roboto-Black.ttf", text_shader);

    window.SetClearColor(gluten::base::Color(0.4f, 0.1f, 0.3f));

    gluten::shader::VertexShader vs({ "TEXTURE" }, app.GlutenRoot("/shader/examples/lighting.vs"));
    gluten::shader::FragmentShader fs({ "TEXTURE" }, app.GlutenRoot("/shader/examples/lighting.fs"));
    gluten::shader::ShaderProgram shader(vs, fs);

    gluten::geometry::Mesh cube_msh = gluten::geometry::Cube({ 0, 0, 0 });

    std::shared_ptr<gluten::texture::Texture<>> texture = 
        std::make_shared<gluten::texture::Texture<>>("base_texture.jpg");
    texture->SetDefaults();

    std::shared_ptr<gluten::camera::DirectionalLight> dir_light = 
        std::make_shared<gluten::camera::DirectionalLight>();
    dir_light->direction = glm::vec3(0.f, 4.f, 0.f);

    gluten::camera::CameraPerspective cam(window);
    cam.position += glm::vec3(0.0f, 1.0f, 3.0f);
    cam.rotation = glm::rotate(cam.rotation, 0.1f, glm::vec3(1, 0, 0));

    gluten::camera::CameraOrthographic canvas_cam(window);

    gluten::shader::Material material(shader);
    material.SetTexture(0, texture);
    material.SetTexture(1, texture);
    material.lights.push_back(dir_light);

    gluten::geometry::Model cube({ cube_msh }, { material });

    cube.rotation = glm::angleAxis(glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    cube.position = glm::vec3(1.f, 1.f, 0.f);

    int time = 0;
    while (!window.ShouldClose()) {
        window.ProcessInput();
        window.Clear();

        cam.Update();

        //cube.Draw(cam);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        font.Draw(canvas_cam, "abracadabra__()444abppd", gluten::base::Color(1., 0., 0.), { 10, 10 }, 1);

        window.SwapBuffers();
        app.PollEvents();
    }
}