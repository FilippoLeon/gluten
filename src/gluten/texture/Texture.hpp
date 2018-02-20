#pragma once

#include <cassert>

#include "gluten/base/Color.hpp"

#include "gluten/texture/STBTextureLoader.hpp"

namespace gluten::texture {

class ITexture {
public:
    enum class Axis {
        Horizontal = GL_TEXTURE_WRAP_S,
        Vertical = GL_TEXTURE_WRAP_T,
        Depth = GL_TEXTURE_WRAP_R,
        All,
    };

    enum class Wrap {
        Repeat = GL_REPEAT,
        MirroredRepeat = GL_MIRRORED_REPEAT,
        ClampEdge = GL_CLAMP_TO_EDGE,
        BorderBorder = GL_CLAMP_TO_BORDER
    };

    enum class Scaling {
        Minification = GL_TEXTURE_MIN_FILTER,
        Magnification = GL_TEXTURE_MAG_FILTER,
    };

    enum class Interpolation {
        Nearest = GL_NEAREST,
        Linear = GL_LINEAR,
        NearestMipmap_Nearest = GL_NEAREST_MIPMAP_NEAREST,
        LinearMipmap_Nearest = GL_LINEAR_MIPMAP_NEAREST,
        NearestMipmap_Linear = GL_NEAREST_MIPMAP_LINEAR,
        LinearMipmap_Linear = GL_LINEAR_MIPMAP_LINEAR,
    };

    void SetDefaults() {
        SetWrapMode(Axis::All, Wrap::ClampEdge);
        SetFilter(Scaling::Minification, Interpolation::Linear);
        SetFilter(Scaling::Magnification, Interpolation::Linear);
    }

    void SetWrapMode(Axis axis, Wrap wrap_mode) {
        Bind();
        if (axis == Axis::All) {
            SetWrapMode(Axis::Horizontal, wrap_mode);
            SetWrapMode(Axis::Vertical, wrap_mode);
            SetWrapMode(Axis::Depth, wrap_mode);
        } else {
            glTexParameteri(GetType(), (int) axis, (int) wrap_mode);
        }
    }

    void SetBorder(base::Color col) {
        Bind();
        glTexParameterfv(GetType(), GL_TEXTURE_BORDER_COLOR, col.Data());
    }

    void SetFilter(Scaling scl, Interpolation interpolation) {
        Bind();
        assert( (!(scl == Scaling::Magnification)
                   || (interpolation == Interpolation::Nearest
                   || interpolation == Interpolation::Linear))
               && "Magnification cannot use Mipmap levels"
        );
        glTexParameteri(GetType(), (int) scl, (int) interpolation);
    }

    virtual unsigned int GetType() = 0;

    virtual void Bind(unsigned int unit = 0) {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GetType(), texture_id);
    }

    static void Unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
protected:
    unsigned int texture_id;
};

template <class TextureLoader = STBTextureLoader>
class Texture : public ITexture {
public:
    Texture(TextureLoader & tl) {
        glGenTextures(1, &texture_id);

        Bind();
        glTexImage2D(GL_TEXTURE_2D, 0, tl.GetType(), tl.Width(), tl.Height(),
                     0, tl.GetType(), GL_UNSIGNED_BYTE, tl.Data());
        glGenerateMipmap(GL_TEXTURE_2D);

        tl.Attach(texture_id);
    }

    unsigned int GetType() {
        return texture_type;
    }
private:
    unsigned int texture_type = GL_TEXTURE_2D;
};

template <class TextureLoader = STBTextureLoader>
class TextureCube : public ITexture {
public:
    TextureCube(const std::initializer_list<std::string> & filenames) 
        : filenames{ filenames } {
        glGenTextures(1, &texture_id); 
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

        assert(filenames.size() >= 6 && "Not enough cubemap components!");
        Bind();

        int width, height, channels;
        int i = 0;
        for (auto filename: this->filenames) {
            TextureLoader tl(filename);

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i++, 0, GL_RGB, 
                         tl.Width(), tl.Height(), 0, 
                         GL_RGB, GL_UNSIGNED_BYTE, tl.Data());
            glGenerateMipmap(GetType());
        }
    }

    unsigned int GetType() {
        return texture_type;
    }
private:
    std::vector<std::string> filenames;

    unsigned int texture_type = GL_TEXTURE_CUBE_MAP;
};


}