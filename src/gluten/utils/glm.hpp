#pragma once

#include <type_traits>
#include <tuple>
#include <array>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "gluten/base/Color.hpp"

namespace gluten::utils::glm {

template <typename Test, template<class...> class Ref>
struct is_instance_of : std::false_type {};

template <template<class...> class Ref, class... Args>
struct is_instance_of<Ref<Args...>, Ref> : std::true_type {};

template <typename C>
constexpr bool is_allowed_container() {
    return is_instance_of<C, std::vector>::value;
}

template <class T>
struct is_std_array : std::false_type {};

template <class T, unsigned int N>
struct is_std_array<std::array<T,N>> : std::true_type {};

template <typename T>
constexpr std::tuple<int, int> get_vec_size() {
    if constexpr ( std::is_same_v<T, base::Color> ) {
        return {4, 1};
    } else if constexpr ( std::is_arithmetic_v<T> ) {
        return {1, 1};
    } else if constexpr ( std::is_same_v<::glm::tvec1<typename T::value_type>, T>) {
        return { 1, 1 };
    } else if constexpr ( std::is_same_v<::glm::tvec2<typename T::value_type>, T>) {
        return { 2, 1 };
    } else if constexpr ( std::is_same_v<::glm::tvec3<typename T::value_type>, T>) {
        return { 3, 1 };
    } else if constexpr (std::is_same_v<::glm::tvec4<typename T::value_type>, T>) {
        return { 4, 1 };
    } else if constexpr (std::is_same_v<::glm::tmat2x2<typename T::value_type>, T>) {
        return { 2, 2 };
    } else if constexpr (std::is_same_v<::glm::tmat3x3<typename T::value_type>, T>) {
        return { 3, 3 };
    } else if constexpr (std::is_same_v<::glm::tmat4x4<typename T::value_type>, T>) {
        return { 4, 4 };
    } else if constexpr (std::is_same_v<::glm::tmat3x4<typename T::value_type>, T>) {
        return { 3, 4 };
    } else if constexpr (is_std_array <T>::value) {
        return { std::tuple_size<T>::value, 1 };
    } else {
        return get_vec_size<typename T::value_type>();
    }
}

template <typename T>
constexpr bool is_glm_vector() {
    if constexpr (std::is_fundamental_v<T> || std::is_same_v<T, base::Color>) {
        return false;
    } else {
        return std::is_same_v<::glm::tvec1<typename T::value_type>, T> ||
               std::is_same_v<::glm::tvec2<typename T::value_type>, T> ||
               std::is_same_v<::glm::tvec3<typename T::value_type>, T> ||
               std::is_same_v<::glm::tvec4<typename T::value_type>, T>;
    }
}

template <typename T>
constexpr bool is_glm_matrix() {
    if constexpr (std::is_fundamental_v<T> || std::is_same_v<T, base::Color>) {
        return false;
    } else {
        return std::is_same_v<::glm::tmat2x2<typename T::value_type>, T> ||
               std::is_same_v<::glm::tmat3x3<typename T::value_type>, T> ||
               std::is_same_v<::glm::tmat4x4<typename T::value_type>, T> ||
               std::is_same_v<::glm::tmat3x4<typename T::value_type>, T>;
    }
}

template <typename T>
constexpr bool is_glm_fundamental() {
    if constexpr(std::is_fundamental_v<T>) {
        return false;
    } else {
        return is_glm_vector<T>() || is_glm_matrix<T>();
    }
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

template<typename T, std::size_t N>
struct vec_value_type_helper<std::array<T, N>, false, false> {
    using type = T;
};

template <typename T>
struct vec_value_type_helper<T, false, false> {
    using type = typename T::value_type::value_type;
};

template <>
struct vec_value_type_helper<base::Color, true, false> {
    using type = float;
};


template <typename T>
using vec_value_type = typename vec_value_type_helper<T, std::is_arithmetic_v<T>
                                                         || std::is_same_v<T, base::Color>, is_glm_fundamental<T>()>::type;

template <typename T>
const auto* get_data(const T & vec) {
    if constexpr( std::is_arithmetic_v<T> || std::is_same_v<T, base::Color> ) {
        return reinterpret_cast<const vec_value_type<T>*>(&vec);
    } else if constexpr ( is_glm_fundamental<T>() ) {
        return ::glm::value_ptr(vec);
    } else {
        return reinterpret_cast<const vec_value_type<T>*>(vec.data());
    }
}

template <typename T>
int get_count(const T & val) {
    if constexpr(std::is_arithmetic_v<T> 
                 || is_glm_fundamental<T>() 
                 || std::is_same_v<T, base::Color> 
                 || is_std_array<T>::value ) {
        return 1;
    } else {
        return val.size();
    }
}

enum class EnumVectorType { Vector, Matrix };

template <EnumVectorType vt>
struct VectorType { };

template <typename T>
using get_vector_type = std::conditional_t<is_glm_matrix<T>(),
                                           VectorType<EnumVectorType::Matrix>,
                                           VectorType<EnumVectorType::Vector>
                                           >;

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