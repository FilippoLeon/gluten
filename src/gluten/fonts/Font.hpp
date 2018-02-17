#pragma once 

#include <string>
#include <map>

#include "gluten/shader/Shader.hpp"

namespace gluten::fonts {

struct Character {
    int texture_id;
    int width, height, left, top, advance;
};

class Font {
public:
    Font(std::string filename, shader::ShaderProgram & shader, int height = 16);

    ~Font();


private:
    static bool freetype_isinit;
    static int freetype_instances;

    std::map<char, Character> charmap;

    shader::ShaderProgram & shader;
};

class Character {

};

}