#pragma once

#include <string>

namespace gluten::texture {

class STBTextureLoader {
public:
    STBTextureLoader(std::string filename);

    ~STBTextureLoader();

    int Width() const { return width; }
    int Height() const { return height; }
    int Channels() const { return channels; }
    unsigned char * Data() { return data; }

private:
    int width, height, channels;
    unsigned char *data;
};

}