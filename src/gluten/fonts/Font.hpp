#pragma once 

#include <memory>
#include <string>
#include <map>

#include "gluten/shader/Shader.hpp"

#include "gluten/base/Color.hpp"

#include "gluten/geometry/Mesh.hpp"

#include "gluten/camera/Camera.hpp"

namespace gluten::fonts {

struct Character {
    unsigned int texture_id;
    unsigned int width, height;
    int left, top;
    long int advance;
};

class Font {
public:
    Font(std::string filename, shader::ShaderProgram & shader, unsigned int height = 48);

    ~Font();

    void Draw(camera::CameraOrthographic cam, std::string text,
              base::Color color, std::array<float, 2> pos, float scale = 1.0f);

private:
    static bool freetype_isinit;
    static int freetype_instances;

    std::map<char, Character> charmap;

    shader::ShaderProgram & shader;

    geometry::Mesh mesh;
};

}