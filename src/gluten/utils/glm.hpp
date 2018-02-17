#pragma once

#include <type_traits>
#include <tuple>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace utils::glm {

template <typename T>
constexpr std::tuple<int, int> get_vec_size() {
    if constexpr ( std::is_arithmetic_v<T> ) {
        return { 1, 1 };
    } else if constexpr ( std::is_same_v<::glm::tvec1<T::value_type>, T>) {
        return { 1, 1 };
    } else if constexpr ( std::is_same_v<::glm::tvec2<T::value_type>, T>) {
        return { 2, 1 };
    } else if constexpr ( std::is_same_v<::glm::tvec3<T::value_type>, T>) {
        return { 3, 1 };
    } else if constexpr (std::is_same_v<::glm::tvec4<T::value_type>, T>) {
        return { 4, 1 };
    } else if constexpr (std::is_same_v<::glm::tmat2x2<T::value_type>, T>) {
        return { 2, 2 };
    } else if constexpr (std::is_same_v<::glm::tmat3x3<T::value_type>, T>) {
        return { 3, 3 };
    } else if constexpr (std::is_same_v<::glm::tmat4x4<T::value_type>, T>) {
        return { 4, 4 };
    } else if constexpr (std::is_same_v<::glm::tmat3x4<T::value_type>, T>) {
        return { 3, 4 };
    } else {
        return get_vec_size<T::value_type>();
    }
}

template <typename T>
constexpr bool is_glm_vector() {
    return std::is_same_v<::glm::tvec1<T::value_type>, T> ||
           std::is_same_v<::glm::tvec2<T::value_type>, T> ||
           std::is_same_v<::glm::tvec3<T::value_type>, T> ||
           std::is_same_v<::glm::tvec4<T::value_type>, T>;
}

template <typename T>
constexpr bool is_glm_matrix() {
    return std::is_same_v<::glm::tmat2x2<T::value_type>, T> ||
           std::is_same_v<::glm::tmat3x3<T::value_type>, T> ||
           std::is_same_v<::glm::tmat4x4<T::value_type>, T> ||
           std::is_same_v<::glm::tmat3x4<T::value_type>, T>;
}

template <template<typename> typename C, typename T = C::value_type>
constexpr bool is_allowed_container() {
    return std::is_same_v<std::vector<T>, C<T>>;
}

template <typename C>
constexpr bool is_allowed_container() {
    return false;
}

template <typename T>
constexpr bool is_glm_fundamental() {
    return is_glm_vector<T>() || is_glm_matrix<T>();
}

template <typename T>
using vt = typename T::value_type;

template<typename T, bool fundamental, bool glm_fundamental>
struct vec_value_type_helper { };

template <typename T>
struct vec_value_type_helper<T, false, true> {
    using type = typename T::value_type;
};

template <typename T>
struct vec_value_type_helper<T, true, false> {
    using type = T;
};

template <typename T>
struct vec_value_type_helper<T, false, false> {
    using type = typename T::value_type::value_type;
};


template <typename T>
using vec_value_type = typename vec_value_type_helper<T, std::is_arithmetic_v<T>, is_glm_fundamental<T>()>::type;

template <typename T>
const auto* get_data(const T & vec) {
    if constexpr( std::is_arithmetic_v<T> ) {
        return &vec;
    } else if constexpr ( is_glm_fundamental<T>() ) {
        return ::glm::value_ptr(vec);
    } else {
        return reinterpret_cast<const vec_value_type<T>*>(vec.data());
    }
}

template <typename T>
int get_count(const T & val) {
    if constexpr(std::is_arithmetic_v<T> || is_glm_fundamental<T>() ) {
        return 1;
    } else {
        return val.size();
    }
}

enum class EnumVectorType { Vector, Matrix };

template <EnumVectorType vt>
struct VectorType { };

template <typename T>
using get_vector_type = std::conditional_t<std::is_arithmetic_v<T> ||
                                           is_glm_vector<T>() || is_allowed_container<T>(),
                                           VectorType<EnumVectorType::Vector>,
                                           VectorType<EnumVectorType::Matrix>>;


template <unsigned int sizex, unsigned int sizey>
inline void Uniform(int loc, int count, const float * data, 
                    VectorType<EnumVectorType::Matrix>) {
    static_assert(sizex > 0 && sizex <= 4 && sizey > 0 && sizey <= 4, "Invalid vector size!");
    if constexpr(sizex == 2 && sizey == 2) {
        glUniformMatrix2fv(loc, count, GL_FALSE, data);
    } else if constexpr(sizex == 3 && sizey == 3) {
        glUniformMatrix3fv(loc, count, GL_FALSE, data);
    } else if constexpr(sizex == 4 && sizey == 4) {
        glUniformMatrix4fv(loc, count, GL_FALSE, data);
    } else if constexpr(sizex == 3 && sizey == 4) {
        glUniformMatrix3x4fv(loc, count, GL_FALSE, data);
    }
}

template <unsigned int sizex, unsigned int sizey>
inline void Uniform(int loc, int count, const int * data,
                    VectorType<EnumVectorType::Matrix>) = delete;

template <unsigned int sizex, unsigned int sizey>
inline void Uniform(int loc, int count, const float * data, 
                    VectorType<EnumVectorType::Vector>) {
    static_assert(sizey == 1 && sizex > 0 && sizex <= 4, "Invalid vector size!");
    if constexpr(sizex == 1) {
        glUniform1fv(loc, count, data);
    } else if constexpr(sizex == 2) {
        glUniform2fv(loc, count, data);
    } else if constexpr(sizex == 3) {
        glUniform3fv(loc, count, data);
    } else if constexpr(sizex == 4) {
        glUniform4fv(loc, count, data);
    }
}

template <unsigned int sizex, unsigned int sizey>
inline void Uniform(int loc, int count, const int * data, 
                    VectorType<EnumVectorType::Vector>) {
    static_assert(sizey == 1 && sizex > 0 && sizex <= 4, "Invalid vector size!");
    if constexpr(sizex == 1) {
        glUniform1iv(loc, count, data);
    } else if constexpr(sizex == 2) {
        glUniform2iv(loc, count, data);
    } else if constexpr(sizex == 3) {
        glUniform3iv(loc, count, data);
    } else if constexpr(sizex == 4) {
        glUniform4iv(loc, count, data);
    }
}

}