#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace gluten::texture {

class Framebuffer {
public:
    enum class Bit {
        Color = GL_COLOR_BUFFER_BIT,
        Depth = GL_DEPTH_BUFFER_BIT,
    };

    Framebuffer(int width, int height) 
        : width(width), height(height) {
        glGenFramebuffers(1, &framebuffer_object);
    }

    int Width() const { return width; }
    int Height() const { return height; }

    unsigned char * Data() {
        return nullptr;
    }

    int GetType() const {
        return GL_DEPTH_COMPONENT;
    }

    void UseViewport() {
        glViewport(0, 0, width, height);
    }

    void Clear(int bit) {
        glClear(bit);
    }

    void Bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object);
    }

    static void Unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Attach(int texture_id) {
        Bind();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 
                               GL_TEXTURE_2D, texture_id, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        Unbind();
    };

    int width, height;
    unsigned int framebuffer_object;
};

}