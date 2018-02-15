#pragma once

#include <array>
#include <initializer_list>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace gluten::geometry {

class Triangle {
public:
    Triangle(std::initializer_list<float> vertices_list) {
        //static_assert(vertices_list.size() >= 9U, "Insufficient vertex count!");
        std::copy_n(vertices_list.begin(), 9, vertices.begin());

        glGenVertexArrays(1, &vertex_array_object);
        glBindVertexArray(vertex_array_object);

        glGenBuffers(1, &vertex_buffer_object);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    ~Triangle() {
        glDeleteVertexArrays(1, &vertex_array_object);
        glDeleteBuffers(1, &vertex_buffer_object);
    }

    void Draw() {
        glBindVertexArray(vertex_array_object);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    unsigned int vertex_array_object;
    unsigned int vertex_buffer_object;

    std::array<float, 9> vertices;
};

}