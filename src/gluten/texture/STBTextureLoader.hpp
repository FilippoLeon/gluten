#pragma once

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace gluten::texture {

class STBTextureLoader {
public:
    STBTextureLoader(std::string filename);

    ~STBTextureLoader();

    int Width() const { return width; }
    int Height() const { return height; }
    int Channels() const { return channels; }

    void Bind() {};

    static void Unbind() {};

    void Attach(int) {};

    int GetType() const {
        return GL_RGB;
    }

    unsigned char * Data() { return data; }

private:
    int width, height, channels;
    unsigned char *data;
};

}