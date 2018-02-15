#pragma once

namespace gluten::base {

struct Color {
    Color() = default;

    Color(float r, float g, float b, float a = 1.0f) 
        : r(r), g(g), b(b), a(a) {

    }

    float * Data() {
        return (float*) this;
    }

    float r, g, b, a;
};

}