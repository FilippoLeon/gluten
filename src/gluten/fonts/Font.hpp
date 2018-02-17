#pragma once 

#include <string>
#include <map>

#include "gluten/shader/Shader.hpp"

#include "gluten/base/Color.hpp"

namespace gluten::fonts {

struct Character {
    int texture_id;
    int width, height, left, top, advance;
};

class Font {
public:
    Font(std::string filename, shader::ShaderProgram & shader, int height = 16);

    ~Font();

    void Draw(std::string text, base::Color color);

private:
    static bool freetype_isinit;
    static int freetype_instances;

    std::map<char, Character> charmap;

    shader::ShaderProgram & shader;
};

}