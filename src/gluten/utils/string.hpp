#pragma once

namespace gluten::string {

template <typename T>
std::string to_string(const T & v, int n = 2) {
    std::ostringstream out;
    out << std::setprecision(n) << v;
    return out.str();
}

std::string to_string(glm::vec3 vec, int n = 2) {
    return to_string(vec.x, n)
           + ", " + to_string(vec.y, n)
           + ", " + to_string(vec.z, n);
}

std::string to_string(glm::vec4 vec, int n = 2) {
    return to_string(vec.x, n)
           + ", " + to_string(vec.y, n)
           + ", " + to_string(vec.z, n)
           + ", " + to_string(vec.w, n);
}
std::string to_string(glm::quat vec, int n = 2) {
    return to_string(vec.x, n)
           + ", " + to_string(vec.y, n)
           + ", " + to_string(vec.z, n)
           + ", " + to_string(vec.w, n);
}

}