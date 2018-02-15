#pragma once

#include "gluten/base/Color.hpp"

#include "gluten/texture/STBTextureLoader.hpp"

namespace gluten::texture {

template <class TextureLoader = STBTextureLoader>
class Texture {
    unsigned int texture_id;
public:
    Texture(std::string filename) {
        glGenTextures(1, &texture_id);

        TextureLoader tl(filename);

        Bind();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tl.Width(), tl.Height(), 0, GL_RGB, GL_UNSIGNED_BYTE, tl.Data());
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    void Bind(unsigned int unit = 0) {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, texture_id);
    }

    enum class Axis {
        Horizontal = GL_TEXTURE_WRAP_S, 
        Vertical = GL_TEXTURE_WRAP_T,
        Both,
    };

    enum class Wrap {
        Repeat = GL_REPEAT,
        MirroredRepeat = GL_MIRRORED_REPEAT,
        ClampEdge = GL_CLAMP_TO_EDGE,
        BorderBorder  = GL_CLAMP_TO_BORDER
    };

    void SetWrapMode(Axis axis, Wrap wrap_mode) {
        if (axis == Axis::Both) {
            SetWrapMode(Axis::Horizontal, wrap_mode);
            SetWrapMode(Axis::Vertical, wrap_mode);
        } else {
            glTexParameteri(GL_TEXTURE_2D, axis, wrap_mode);
        }
    }

    void SetBorder(base::Color col) {
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, col.Data());
    }

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

    void SetFilter(Scaling scl, Interpolation interpolation) {
        assert(!(scl == Scaling::Magnification)
               || (interpolation == Nearest || interpolation == Linear) 
               && "Magnification cannot use Mipmap levels"
        );
        glTexParameteri(GL_TEXTURE_2D, scl, interpolation);
    }
};

}