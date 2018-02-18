#include "Font.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H 

#include "gluten/geometry/Mesh.hpp"

#include "gluten/debug/exceptions.hpp"

#include "gluten/texture/Texture.hpp"

namespace gluten::fonts {

int Font::freetype_instances = 0;
bool Font::freetype_isinit = false;
FT_Library freetype_lib;

Font::Font(std::string filename, shader::ShaderProgram & shader , int height)
    : shader(shader), mesh(geometry::Plane({ 0, 0, 1 })) {
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

    FT_Set_Pixel_Sizes(face, 0, height);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (char c = 0; c < 127; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            throw debug::RuntimeException("Could't load char {}!" + (int) c);
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

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                    character.width, character.height,
                    0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer
        );


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

void Font::Draw(camera::CameraOrthographic cam, std::string text,
                base::Color color, std::array<float, 2> pos, float scale) {
    //void RenderText(Shader &s, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {
    shader.Use();
    shader.SetUniformLocation("textColor", color);

    glm::mat4 proj = cam.GetProjectionMatrix();
    shader.SetUniformLocation("projection", proj);
    glActiveTexture(GL_TEXTURE0);
    mesh.Bind();

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = charmap[*c];

        std::array<float, 2> local_pos = {
            pos[0] + ch.left * scale,
            pos[1] + (ch.top - ch.height) * scale
        };

        std::array<float, 2> size = { 
            ch.width * scale,  
            ch.height * scale
        };

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ch.texture_id);
        shader.SetUniformLocation("position", local_pos);
        shader.SetUniformLocation("size", size);
        mesh.Draw();

        pos[0] += (ch.advance >> 6) * scale;
    }

    geometry::Mesh::Unbind();
    texture::ITexture::Unbind();

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

}