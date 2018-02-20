#pragma once

namespace gluten::base {

struct Color {
    Color() = default;

    constexpr Color(float r, float g, float b, float a = 1.0f) 
        : r(r), g(g), b(b), a(a) { }

    float * Data() {
        return (float*) this;
    }

    float r, g, b, a;
};

namespace ColorConstant {
constexpr static Color Red = Color(1, 0, 0);
}

}