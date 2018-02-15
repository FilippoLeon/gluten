#include <random>

#include <glm/glm.hpp>

#include "gluten/base/Application.hpp"
#include "gluten/base/Window.hpp"

#include "gluten/base/Color.hpp"

#include "gluten/shader/Shader.hpp"

#include "gluten/geometry/Triangle.hpp"
#include "gluten/geometry/Mesh.hpp"

int main() {
    gluten::base::Application app;

    gluten::base::Window window(600, 600, "Win");

    window.SetClearColor(gluten::base::Color(0.1f, 0.1f, 0.3f));

    gluten::shader::VertexShader vs(app.GlutenRoot("/shader/examples/empty.vs"));
    gluten::shader::FragmentShader fs(app.GlutenRoot("/shader/examples/uniform.fs"));
    gluten::shader::ShaderProgram shader(vs, fs);

    gluten::geometry::Triangle tr{
        - 1.2f, -1.2f, 0.0f,
        1.2f, -1.2f, 0.0f,
        0.0f,  1.2f, 0.0f
    };

    gluten::geometry::Mesh msh{ {
            0.5f,  0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f
        }, {
            0, 1, 3,
            1, 2, 3
        }
    };

    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(0.0, 1.0);

    while (!window.ShouldClose()) {
        window.ProcessInput();
        window.Clear();

        shader.Use();

        float rnd = distribution(generator);
        shader.SetUniformLocation("color", glm::vec4(1.0f, 1.0f, rnd, 1.0f));
        tr.Draw();
        shader.SetUniformLocation("color", glm::vec4(1.0f, 1.0f, 1.0f - rnd, 1.0f));

        msh.Draw<>();

        window.SwapBuffers();
        app.PollEvents();
    }
}