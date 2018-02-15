#include "STBTextureLoader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "gluten/debug/log.hpp"

namespace gluten::texture {

STBTextureLoader::STBTextureLoader(std::string filename) {
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
    if (data == nullptr) {
        debug::LogError("Could not load filename {}.", filename);
    }
}

STBTextureLoader::~STBTextureLoader() {
    stbi_image_free(data);
}

}