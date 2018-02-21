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

    gluten::shader::VertexShader vs_skybox(app.GlutenRoot("/shader/examples/skybox.vs"));
    gluten::shader::FragmentShader fs_skybox(app.GlutenRoot("/shader/examples/skybox.fs"));
    gluten::shader::ShaderProgram shader_skybox(vs_skybox, fs_skybox);

    gluten::geometry::Mesh plane = gluten::geometry::Plane({ 0, 0, 0 });
    gluten::geometry::Mesh cube_msh = gluten::geometry::Cube({ 0, 0, 0 });
    gluten::geometry::Mesh cube_sb_msh = gluten::geometry::Skybox({ 0 });

    std::shared_ptr<gluten::texture::Texture<>> texture = std::make_shared<gluten::texture::Texture<>>(gluten::texture::STBTextureLoader("base_texture.jpg"));

    std::shared_ptr<gluten::texture::TextureCube<>> texture_skybox =
        std::make_shared<gluten::texture::TextureCube<>>(gluten::texture::TextureCube<>{
                "clouds_rgt.png" ,
                "clouds_lft.png" ,
                "clouds_top.png" ,
                "clouds_btm.png" ,
                "clouds_frnt.png" ,
                "clouds_bck.png" 
            }
        );

    texture->SetDefaults();
    texture_skybox->SetDefaults();

    gluten::camera::CameraPerspective cam;
    cam.position += glm::vec3(0.0f, 1.0f, 3.0f);
    cam.rotation = glm::rotate(cam.rotation, 0.1f, glm::vec3(1, 0, 0));

    std::shared_ptr<gluten::camera::DirectionalLight> dir_light 
        = std::make_shared<gluten::camera::DirectionalLight>();
    dir_light->SetDirection(glm::vec3(0.f, 4.f, 0.f));

    gluten::shader::Material material(shader);
    material.SetTexture(0, texture);
    material.SetTexture(1, texture);
    material.lights.push_back(dir_light);

    gluten::shader::Material skybox_material(shader_skybox);
    skybox_material.SetTexture(0, texture_skybox);

    gluten::geometry::Model skybox_model({ cube_sb_msh }, { skybox_material });
    gluten::geometry::Model floor({ plane }, { material });
    gluten::geometry::Model light_cube({ cube_msh }, { material });
    gluten::geometry::Model cube({ cube_msh }, { material });

    light_cube.scale = glm::vec3(0.1f, 0.1f, 0.1f);
    light_cube.position = glm::vec3(0.f, 4.f, 0.f);
    light_cube.rotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    cube.rotation = glm::angleAxis(glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    cube.position = glm::vec3(1.f, 1.f, 0.f);

    floor.rotation = glm::angleAxis(
        glm::radians(90.0f),
        glm::vec3(1.0f, 0.0f, 0.0f)
    );
    floor.scale = glm::vec3(10.0f, 10.0f, 10.0f);

    while (!window.ShouldClose()) {
        window.ProcessInput();
        window.Clear();

        cam.Update(window);

        light_cube.Draw(cam);
        cube.Draw(cam);
        floor.Draw(cam);
        glDepthFunc(GL_LEQUAL);
        skybox_model.Draw(cam); 
        glDepthMask(GL_TRUE);

        window.SwapBuffers();
        app.PollEvents();
    }
}