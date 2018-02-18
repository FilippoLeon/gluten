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
    int texture_id;
    int width, height, left, top, advance;
};

class Font {
public:
    Font(std::string filename, shader::ShaderProgram & shader, int height = 48);

    ~Font();

    void Draw(camera::CameraOrthographic cam, std::string text,
              base::Color color, std::array<float, 2> pos, float scale = 1.);

private:
    static bool freetype_isinit;
    static int freetype_instances;

    std::map<char, Character> charmap;

    shader::ShaderProgram & shader;

    std::unique_ptr<geometry::Mesh> mesh;
};

}