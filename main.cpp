#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "linmath.h"

#include <stdlib.h>
#include <stdio.h>

#include <iostream>

static const struct {
    float x, y;
    float r, g, b;
} vertices[3] = {
    { -0.6f, -0.4f, 1.f, 0.f, 0.f },
    {  0.6f, -0.4f, 0.f, 1.f, 0.f },
    {   0.f,  0.6f, 0.f, 0.f, 1.f }
};

static const char* vertex_shader_text =
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec2 vPos;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";

static const char* fragment_shader_text =
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";

#define GL_SAFE_CALL(x) { x; GLenum err = glGetError(); __glSafeCall(err, __FILE__, __LINE__); }
#define GLEW_SAFE_CALL(x) { __glewSafeCall((x), __FILE__, __LINE__); }
#define GLFW_SAFE_CALL(x) { __glfwSafeCall((x), __FILE__, __LINE__); }

void __terminate() {
    glfwTerminate();
    exit(EXIT_FAILURE);
}

int __glSafeCall(GLenum err,
                 const char *file, int line, bool abort = true) {
    if(err != GL_NO_ERROR) {
        // gl failed
        std::cerr << "GL Error " << err << ":"
                  << " at " << file << ":" << line
                  << std::endl;
        if(abort) __terminate();
     }
}

int __glewSafeCall(GLenum err,
                   const char *file, int line, bool abort = true) {
    if(err != GLEW_OK) {
        // glew failed
        std::cerr << "GLEW Error " << err << ":"
                  << glewGetErrorString(err)
                  << " at " << file << ":" << line
                  << std::endl;
        if(abort) __terminate();
     }
}

int __glfwSafeCall(int err,
                   const char *file, int line, bool abort = true) {
}

static void error_callback(int error, const char* description) {
    std::cerr << "Error "<< error << ": "
              << description
              << std::endl;
}

static void key_callback(GLFWwindow* window,
                         int key, int,
                         int action, int) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main(void) {

    GLFWwindow * window;

    GLuint vertex_buffer, vertex_shader,
            fragment_shader, program;

    GLint mvp_location, vpos_location, vcol_location;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(640, 480, "OpenGL Test", NULL, NULL);
    if (window == nullptr) {
        __terminate();
    }

    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    GLEW_SAFE_CALL(glewInit());
    if (GLEW_ARB_vertex_program) {
        // Extension ARB_vertex_program available
    }

    GL_SAFE_CALL(glGenBuffers(1, &vertex_buffer));

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    mvp_location = glGetUniformLocation(program, "MVP");
    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 5, (void*) 0);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 5, (void*) (sizeof(float) * 2));
    while (!glfwWindowShouldClose(window)) {
        float aspect_ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        aspect_ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        mat4x4 m;
        mat4x4_identity(m);
        mat4x4_rotate_Z(m, m, (float) glfwGetTime());
        mat4x4 p;
        mat4x4_ortho(p, -aspect_ratio, aspect_ratio, -1.f, 1.f, 1.f, -1.f);
        mat4x4 mvp;
        mat4x4_mul(mvp, p, m);

        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
