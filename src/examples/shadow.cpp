#include <random>
#include <iomanip>
#include <sstream>

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

#include "gluten/texture/Framebuffer.hpp"

#include "gluten/fonts/Font.hpp"

using namespace gluten;


template <typename T>
std::string to_string(const T & v, int n = 2) {
    std::ostringstream out;
    out << std::setprecision(n) << v;
    return out.str();
}

std::string to_string(glm::vec3 vec, int n = 2) {
    return to_string(vec.x, n) 
        + ", " + to_string(vec.y, n) 
        + ", " + to_string(vec.z, n);
}

std::string to_string(glm::vec4 vec, int n = 2) {
    return to_string(vec.x, n)
        + ", " + to_string(vec.y, n)
        + ", " + to_string(vec.z, n)
        + ", " + to_string(vec.w, n);
}
std::string to_string(glm::quat vec, int n = 2) {
    return to_string(vec.x, n)
        + ", " + to_string(vec.y, n)
        + ", " + to_string(vec.z, n)
        + ", " + to_string(vec.w, n);
}

int main() {
    base::Application app;

    base::Window window(1000, 1000, "Win");

    shader::VertexShader text_vs({ "TEXTURE" }, app.GlutenRoot("/shader/examples/text.vs"));
    shader::FragmentShader text_fs({ "TEXTURE" }, app.GlutenRoot("/shader/examples/text.fs"));
    shader::ShaderProgram text_shader(text_vs, text_fs);
    fonts::Font font("Roboto-Black.ttf", text_shader);

    window.SetClearColor(base::Color(0.4f, 0.1f, 0.3f));

    //shader::VertexShader vs({ "TEXTURE" }, app.GlutenRoot("/shader/examples/lighting.vs"));
    //shader::FragmentShader fs({ "TEXTURE" }, app.GlutenRoot("/shader/examples/lighting.fs"));
    shader::VertexShader vs({ "TEXTURE" }, app.GlutenRoot("/shader/examples/lighting_shadow.vs"));
    shader::FragmentShader fs({ "TEXTURE" }, app.GlutenRoot("/shader/examples/lighting_shadow.fs"));
    shader::ShaderProgram shader(vs, fs);

    shader::VertexShader vs_notex(app.GlutenRoot("/shader/examples/lighting_shadow.vs"));
    shader::FragmentShader fs_notex(app.GlutenRoot("/shader/examples/lighting_shadow.fs"));
    //shader::VertexShader vs_notex(app.GlutenRoot("/shader/examples/lighting.vs"));
    //shader::FragmentShader fs_notex(app.GlutenRoot("/shader/examples/lighting.fs"));
    shader::ShaderProgram shader_notex(vs_notex, fs_notex);

    geometry::Mesh plane = geometry::Plane({ 0, 0, 0 });
    geometry::Mesh cube_msh = geometry::Cube({ 0, 0, 0 });

    auto texture = std::make_shared<texture::Texture<>>(
        texture::STBTextureLoader("base_texture.jpg")
    );

    gluten::camera::CameraOrthographic canvas_cam(
        gluten::camera::Rectangle(0, 0, window.Width(), window.Height()), 0, 1
    );

    camera::CameraPerspective cam;
    //camera::CameraOrthographic cam(camera::Rectangle(-10, -10, 10, 10), -50, 50);
    cam.position += glm::vec3(0.0f, 1.0f, 3.0f);
    cam.rotation = glm::rotate(cam.rotation, 0.1f, glm::vec3(1, 0, 0));

    std::shared_ptr<camera::DirectionalLight> dir_light 
        = std::make_shared<camera::DirectionalLight>();
    //dir_light->SetDirection(glm::vec3(-0.1f, -0.32f, -0.94f));
    //dir_light->SetPosition(glm::vec3(-0.6f, -20.f, 7.5f));
    //dir_light->SetPosition(glm::vec3(0.0f, 40.0f, 0.0f));
    //dir_light->SetDirection(glm::vec3(0.0f, 1.0f, 0.0f));
    //dir_light->SetPosition(glm::vec3(0.1f, 1.0f, -0.5f));
    //dir_light->SetDirection(glm::vec3(-0.77f, -1.0f, 0.15f));
    dir_light->SetPosition(glm::vec3(0, 19.0f, 0));
    //dir_light->SetDirection(glm::vec3(0, 0, -1.0));
    dir_light->SetDirection(glm::vec3(0.1, 1.0, 0.1));

    //std::shared_ptr<camera::PointLight> point_light 
    //    = std::make_shared<camera::PointLight>();
    //point_light->SetPosition(glm::vec3(0.f, 4.f, 0.f));
    //point_light->ambientColor = glm::vec3(0.f, 0.f, 0.f);
    //point_light->diffuseColor = glm::vec3(0.1f, 0.1f, 0.1f);
    //point_light->specularColor = glm::vec3(1,1,1);


    //std::shared_ptr<camera::SpotLight> light 
    //    = std::make_shared<camera::SpotLight>();
    //light->SetPosition(cam.position);
    //light->SetDirection(cam.Forward());
    //light->radius = glm::cos(glm::radians(12.5f));

    shader::Material material(shader);
    material.SetTexture(0, texture);
    material.SetTexture(1, texture);
    material.lights.push_back(dir_light); // TODO moveto 
    //material.lights.push_back(point_light); // TODO moveto 
    //material.lights.push_back(light); // TODO moveto schene
    shader::Material material2(shader_notex);
    material2.lights.push_back(dir_light); // TODO moveto schene
    //material2.lights.push_back(point_light); // TODO moveto schene
    //material2.lights.push_back(light); // TODO moveto schene

    material2.ambientColor = glm::vec3(0.3f, 0.0f, 0.0f);
    material2.diffuseColor = glm::vec3(0.7f, 0.0f, 0.0f);

    geometry::Model floor({ plane }, { material });
    geometry::Model light_cube({ cube_msh }, { material2 });
    geometry::Model cube({ cube_msh }, { material });
    geometry::Model cube2({ cube_msh }, { material2 });
    geometry::Model cube3({ cube_msh }, { material2 });
    cube3.scale = glm::vec3(0.1,0.1,1);

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
    floor.scale = glm::vec3(10.0f, 10.0f, 1);

    texture::Framebuffer shadow_framebuffer(2048, 2048);
    //texture::Framebuffer shadow_framebuffer(1000,1000);
    std::shared_ptr<texture::ITexture> shadow_texture = 
        std::make_shared<texture::Texture<texture::Framebuffer>>(shadow_framebuffer);

    shadow_texture->SetBorder(base::Color(1, 0, 0, 1));
    shadow_texture->SetWrapMode(texture::ITexture::Axis::All, texture::ITexture::Wrap::BorderBorder);

    shader::VertexShader shadow_vs(app.GlutenRoot("/shader/examples/shadow.vs"));
    shader::FragmentShader shadow_fs(app.GlutenRoot("/shader/examples/shadow.fs"));
    shader::ShaderProgram shadow_shader(shadow_vs, shadow_fs);

    shader::VertexShader shadow_debug_vs(app.GlutenRoot("/shader/examples/lighting.vs"));
    shader::FragmentShader shadow_debug_fs(app.GlutenRoot("/shader/examples/shadow_debug.fs"));
    shader::ShaderProgram shadow_debug_shader(shadow_debug_vs, shadow_debug_fs);

    //cam.position -= glm::vec3(0, -10, -10);
    //dir_light->SetPosition(cam.position);

    //glEnable(GL_CULL_FACE);

    int time = 0;
    while (!window.ShouldClose()) {
        app.Update();

        window.ProcessInput();
        window.Clear();

        cam.Update(window);

        cube3.position = dir_light->GetPosition();
        cube3.rotation = glm::rotation(dir_light->GetDirection(), glm::vec3(0, 0, -1));
        //dir_light->SetDirection(cam.Forward());
        //dir_light->SetPosition(cam.position);

        light_cube.rotation = glm::angleAxis(
            glm::radians((float) time++), glm::vec3(0.0f, 1.0f, 0.0f)
        );
        //light->SetDirection(cam.Forward());    
        //light->SetPosition(cam.position);

        std::shared_ptr<camera::Camera> light_cam 
            = dir_light->GetCamera();

        //cam.position = light_cam->position;
        //cam.rotation = light_cam->rotation;

        //light_cam->Update(window);

        shadow_framebuffer.UseViewport();
        shadow_framebuffer.Bind();
        shadow_framebuffer.Clear((int) texture::Framebuffer::Bit::Depth);

        glCullFace(GL_FRONT);
        shadow_shader.Use();
        light_cube.Draw(*light_cam, shadow_shader); // make sure to use use shadow shader
        cube.Draw(*light_cam, shadow_shader);
        floor.Draw(*light_cam, shadow_shader);
        cube2.Draw(*light_cam, shadow_shader);
        glCullFace(GL_BACK);

        texture::Framebuffer::Unbind();

        window.UseViewport();
        window.Clear(true);

        shadow_texture->Bind(2);
        light_cube.Draw(cam);
        shadow_texture->Bind(2);
        cube.Draw(cam);
        shadow_texture->Bind(2);
        cube2.Draw(cam);
        //shadow_texture->Bind(2);
        //cube3.Draw(cam);
        shadow_texture->Bind(2);
        floor.Draw(cam);
        //shadow_debug_shader.Use();
        //shadow_texture->Bind();
        //floor.materials[0].SetTexture(0, shadow_texture);
        //floor.materials[0].SetTexture(1, shadow_texture);
        //floor.Draw(cam, shadow_debug_shader);

        float elapsed = 
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::get<0>(app.elapsed_avg.Get())
                ).count() * 1e-6;
        int fps = 1000. / elapsed;

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        font.Draw(canvas_cam, "Debug:    " + to_string(elapsed, 2) + " ms (" + std::to_string(fps) + " FPS)",
                  gluten::base::Color(1., 0., 0.), { 10, 10 }, 0.5);
        font.Draw(canvas_cam, "Position: " + to_string(cam.position),
                  gluten::base::Color(1., 0., 0.), { 10, 40 }, 0.5);
        font.Draw(canvas_cam, "Rotation: " + to_string(cam.rotation),
                  gluten::base::Color(1., 0., 0.), { 10, 70 }, 0.25);
        font.Draw(canvas_cam, "Forward:  " + to_string(cam.Forward()),
                  gluten::base::Color(1., 0., 0.), { 10, 90 }, 0.25);
        font.Draw(canvas_cam, "Viewport: " + to_string(cam.viewportRect.width) + " " + to_string(cam.viewportRect.height),
                  gluten::base::Color(1., 0., 0.), { 10, 110 }, 0.25);

        window.SwapBuffers();
        app.PollEvents();
    }
}