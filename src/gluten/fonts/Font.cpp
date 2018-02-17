#include "Font.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H 

#include "gluten/debug/exceptions.hpp"

namespace gluten::fonts {

int Font::freetype_instances = 0;
bool Font::freetype_isinit = false;
FT_Library freetype_lib;

Font::Font(std::string filename, shader::ShaderProgram & shader , int height)
    : shader(shader) {
    if (!freetype_isinit) {
        if (FT_Init_FreeType(&freetype_lib)) {
            throw debug::RuntimeException("Could't init FreeType!");
        }
        ++freetype_instances;
        freetype_isinit = true;
    }

    FT_Face face;
    if (FT_New_Face(freetype_lib, filename.c_str(), 0, &face)) {
        throw debug::RuntimeException("Could't find or load font at {}!" + filename);
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            throw debug::RuntimeException("Could't load char {}!" + (int) c);
            continue;
        }

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        Character character = {
            texture,
            face->glyph->bitmap.width, face->glyph->bitmap.rows,
            face->glyph->bitmap_left, face->glyph->bitmap_top,
            face->glyph->advance.x
        };

//        glTexImage2(GL_TEXTURE_2D, 0, GL_RED,
//                    character.width, character.height,
//                    0, GL_RED, GL_UNSIGNED_BYTE,
//        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        charmap.insert(std::pair<char, Character>(c, character));
    }



    FT_Done_Face(face);
}

Font::~Font() {
    --freetype_instances;
    if (freetype_instances == 0) {
        FT_Done_FreeType(freetype_lib);
        freetype_isinit = false;
    }
}

void Font::Draw(std::string text, base::Color color) {
    //void RenderText(Shader &s, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {
//    s.Use();
//    s.Uniform("textColor", color);
//    glActiveTexture(GL_TEXTURE0);
//    glBindVertexArray(VAO);
//
//    std::string::const_iterator c;
//    for (c = text.begin(); c != text.end(); c++) {
//        Character ch = Characters[*c];
//
//        GLfloat xpos = x + ch.Bearing.x * scale;
//        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
//
//        GLfloat w = ch.Size.x * scale;
//        GLfloat h = ch.Size.y * scale;
//
//        GLfloat vertices[6][4] = {
//            { xpos,     ypos + h,   0.0, 0.0 },
//        { xpos,     ypos,       0.0, 1.0 },
//        { xpos + w, ypos,       1.0, 1.0 },
//
//        { xpos,     ypos + h,   0.0, 0.0 },
//        { xpos + w, ypos,       1.0, 1.0 },
//        { xpos + w, ypos + h,   1.0, 0.0 }
//        };
//
//        glBindTexture(GL_TEXTURE_2D, ch.texture_id);
//
//        glBindBuffer(GL_ARRAY_BUFFER, VBO);
//        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
//        glDrawArrays(GL_TRIANGLES, 0, 6);
//
//        x += (ch.Advance >> 6) * scale;
//    }
//    glBindVertexArray(0);
//    glBindTexture(GL_TEXTURE_2D, 0);
}

}