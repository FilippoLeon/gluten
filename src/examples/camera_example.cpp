#include <random>

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "gluten/base/Application.hpp"
#include "gluten/base/Window.hpp"

#include "gluten/base/Color.hpp"

#include "gluten/shader/Shader.hpp"

#include "gluten/geometry/Mesh.hpp"

#include "gluten/texture/Texture.hpp"

#include "gluten/camera/Camera.hpp"

int main() {
    gluten::base::Application app;

    gluten::base::Window window(600, 600, "Win");

    window.SetClearColor(gluten::base::Color(0.4f, 0.1f, 0.3f));

    gluten::shader::VertexShader vs(app.GlutenRoot("/shader/examples/uv_map_nocolor.vs"));
    gluten::shader::FragmentShader fs(app.GlutenRoot("/shader/examples/textured_nocolor.fs"));
    gluten::shader::ShaderProgram shader(vs, fs);

    gluten::geometry::Mesh msh = gluten::geometry::Cube({ 0, 0, 1 });

    gluten::texture::Texture<> texture(gluten::texture::STBTextureLoader("asd.jpg"));

    gluten::camera::CameraPerspective cam;

    int time = 0;
    while (!window.ShouldClose()) {
        window.ProcessInput();
        window.Clear();

        cam.Update(window);

        shader.Use();

        glm::mat4 model = glm::toMat4(glm::angleAxis(glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

        glm::mat4 transform = cam.GetMatrix() * model;
        shader.SetUniformLocation("transform", transform);

        texture.Bind();
        msh.Draw<>();

        window.SwapBuffers();
        app.PollEvents();
    }
}