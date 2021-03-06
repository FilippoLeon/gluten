#pragma once

#include <initializer_list>
#include <vector>
#include <numeric>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "gluten/debug/log.hpp"

namespace gluten::geometry {

class Mesh {
public:
    struct DrawArray_t {} static const DrawArray;

    Mesh(std::initializer_list<float> vertices, DrawArray_t,
         std::vector<int> displacements = { 3 }, std::vector<bool> ignore = {})
        : Mesh(vertices, { }, displacements, ignore) {

    }

    Mesh(std::initializer_list<float> vertices, 
         std::initializer_list<int> triangles, 
         std::vector<int> displacements = { 3 }, std::vector<bool> ignore = {})
        : vertices{ vertices }, triangles{ triangles } {

        glGenVertexArrays(1, &vertex_array_object);
        Bind();

        glGenBuffers(1, &vertex_buffer_object);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
        glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float), this->vertices.data(), GL_STATIC_DRAW);

        if (triangles.size() > 0) {
            glGenBuffers(1, &element_buffer_object);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->triangles.size() * sizeof(int), this->triangles.data(), GL_STATIC_DRAW);
        }

        stride = std::accumulate(displacements.begin(), displacements.end(), 0);

        if ( vertices.size() % stride != 0 ) {
            debug::LogWarning("Vertices size does not match displacement vector!");
        }
        int i = 0;
        int p = 0;
        for (int j = 0; j < (int) displacements.size(); ++j) {
            int d = displacements[j];
            if ( (int) ignore.size() <= j || ignore[j] == 0 ) {
                glVertexAttribPointer(i, d, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(p * sizeof(float)));
                glEnableVertexAttribArray(i++);
            }
            p += d;
        }

        Unbind();
    }

    ~Mesh() {

    }

    void Bind() {
        glBindVertexArray(vertex_array_object);
    }

    static void Unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    template <bool wireframe = false>
    void Draw() {
        if (wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        glBindVertexArray(vertex_array_object);
        if (triangles.size() > 0) {
            glDrawElements(GL_TRIANGLES, this->triangles.size(), GL_UNSIGNED_INT, 0);
        } else {
            glDrawArrays(GL_TRIANGLES, 0, this->vertices.size() / stride);
        }
    }

    unsigned int vertex_array_object, element_buffer_object, vertex_buffer_object;
    int stride;

    std::vector<float> vertices;
    std::vector<int> triangles;
};

Mesh Cube(std::vector<bool> ignore = {});

Mesh Plane(std::vector<bool> ignore = {});

Mesh Skybox(std::vector<bool> ignore = {});

}