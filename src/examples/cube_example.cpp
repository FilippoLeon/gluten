#include <random>

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "gluten/base/Application.hpp"
#include "gluten/base/Window.hpp"

#include "gluten/base/Color.hpp"

#include "gluten/shader/Shader.hpp"

#include "gluten/geometry/Mesh.hpp"

#include "gluten/texture/Texture.hpp"

int main() {
    gluten::base::Application app;

    gluten::base::Window window(600, 600, "Win");

    window.SetClearColor(gluten::base::Color(0.4f, 0.1f, 0.3f));

    gluten::shader::VertexShader vs(app.GlutenRoot("/shader/examples/uv_map_nocolor.vs"));
    gluten::shader::FragmentShader fs(app.GlutenRoot("/shader/examples/textured_nocolor.fs"));
    gluten::shader::ShaderProgram shader(vs, fs);

    gluten::geometry::Mesh msh = gluten::geometry::Cube({ 0, 0, 1 });

    gluten::texture::Texture<> texture("asd.jpg");

    int time = 0;
    while (!window.ShouldClose()) {
        window.ProcessInput();
        window.Clear();

        shader.Use();

        glm::mat4 view = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -3.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f /* screenWidth / screenHeight */, 0.1f, 100.0f);
        glm::mat4 model = glm::toMat4(glm::angleAxis(glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

        glm::mat4 transform = projection * view * model;
        //m = glm::toMat4(glm::angleAxis(2 * glm::pi<float>() * time++ / 300, glm::vec3(0, 0, 1)));
        //m = glm::toMat4(glm::angleAxis(2 * glm::pi<float>() * time++ / 200, glm::vec3(0, 1, 0)));
        shader.SetUniformLocation("transform", transform);

        texture.Bind();
        msh.Draw<>();

        window.SwapBuffers();
        app.PollEvents();
    }
}