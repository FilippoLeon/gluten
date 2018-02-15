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

int main() {
    gluten::base::Application app;

    gluten::base::Window window(1000, 1000, "Win");

    window.SetClearColor(gluten::base::Color(0.4f, 0.1f, 0.3f));

    gluten::shader::VertexShader vs({ "TEXTURE" }, app.GlutenRoot("/shader/examples/lighting.vs"));
    gluten::shader::FragmentShader fs({ "TEXTURE" }, app.GlutenRoot("/shader/examples/lighting.fs"));
    gluten::shader::ShaderProgram shader(vs, fs);

    gluten::shader::VertexShader vs_notex(app.GlutenRoot("/shader/examples/lighting.vs"));
    gluten::shader::FragmentShader fs_notex(app.GlutenRoot("/shader/examples/lighting.fs"));
    gluten::shader::ShaderProgram shader_notex(vs_notex, fs_notex);

    gluten::geometry::Mesh plane = gluten::geometry::Plane({ 0, 0, 0 });
    gluten::geometry::Mesh cube_msh = gluten::geometry::Cube({ 0, 0, 0 });

    std::shared_ptr<gluten::texture::Texture<>> texture = std::make_shared<gluten::texture::Texture<>>("base_texture.jpg");

    gluten::camera::CameraPerspective cam(window);
    cam.position += glm::vec3(0.0f, 0.0f, 3.0f);

    gluten::camera::Light light;
    light.position = glm::vec3(0.f, 4.f, 0.f);

    gluten::shader::Material material(shader);
    material.SetTexture(0, texture);
    material.SetTexture(1, texture);
    material.light = light; // TODO moveto schene
    gluten::shader::Material material2(shader_notex);
    material2.light = light; // TODO moveto schene

    material2.ambientColor = glm::vec3(0.3f, 0.0f, 0.0f);
    material2.diffuseColor = glm::vec3(0.7f, 0.0f, 0.0f);

    gluten::geometry::Model floor({ plane }, { material });
    gluten::geometry::Model light_cube({ cube_msh }, { material2 });
    gluten::geometry::Model cube({ cube_msh }, { material });
    gluten::geometry::Model cube2({ cube_msh }, { material2 });

    light_cube.scale = glm::vec3(0.1f, 0.1f, 0.1f);
    light_cube.position = glm::vec3(0.f, 4.f, 0.f);
    light_cube.rotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    cube.rotation = glm::angleAxis(glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    cube.position = glm::vec3(1.f, 1.f, 0.f);

    cube2.rotation = glm::angleAxis(glm::radians(-5.0f), glm::vec3(1.0f, 1.0f, 0.0f));
    cube2.position = glm::vec3(3.f, 1.f, 0.f);
    cube2.scale = glm::vec3(0.5f, 0.5f, 0.5f);

    floor.rotation = glm::angleAxis(
        glm::radians(90.0f),
        glm::vec3(1.0f, 0.0f, 0.0f)
    );
    floor.scale = glm::vec3(10.0f, 10.0f, 10.0f);

    int time = 0;
    while (!window.ShouldClose()) {
        window.ProcessInput();
        window.Clear();

        cam.Update();

        light_cube.rotation = glm::angleAxis(glm::radians((float) time++), glm::vec3(0.0f, 1.0f, 0.0f));

        light_cube.Draw(cam);
        cube.Draw(cam);
        floor.Draw(cam);
        cube2.Draw(cam);

        window.SwapBuffers();
        app.PollEvents();
    }
}