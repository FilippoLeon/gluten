#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#if USE_FMT
#include <fmt/format.h>
#endif

#include <fstream>
#include <string>

#include "gluten/debug/log.hpp"
#include "gluten/osutils/pathutils.hpp"
#include "gluten/utils/glm.hpp"

namespace gluten::shader {

constexpr int ERROR_STRING_LENGTH = 512;

class ShaderProgram;

class UniformLocation {
public:
    int operator*() const {
        return loc;
    }

    friend ShaderProgram;
private:
    UniformLocation(int loc) : loc(loc) {}

    int loc;
};

template <class Derived>
class Shader {
public:
    Shader(std::string filename) {
        if (std::string ext = osutils::GetExtension(filename); ext != Derived::extension) {
            debug::LogWarning("Extension {} does not match propery Shader extension {}", 
                              ext, Derived::extension);
        }

        shader_id = glCreateShader(Derived::shader_type);

        std::ifstream stream(filename);
        if (!stream.is_open()) {
#if USE_FMT
            throw std::runtime_error(fmt::format("Could not open file '{}'.", filename));
#else 
            throw std::runtime_error("Cannot open file.");
#endif
        }
        std::string source;

        stream.seekg(0, std::ios::end);
        source.reserve((std::size_t) stream.tellg());
        stream.seekg(0, std::ios::beg);

        source.assign((std::istreambuf_iterator<char>(stream)),
                       std::istreambuf_iterator<char>());

        const char * str_arr = source.c_str();
        glShaderSource(shader_id, 1, &str_arr, nullptr);
        glCompileShader(shader_id); 
        
        int  success;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

        if (!success) {
            char error_string[ERROR_STRING_LENGTH];
            glGetShaderInfoLog(shader_id, ERROR_STRING_LENGTH, NULL, error_string);

#if USE_FMT
            std::string msg = fmt::format("Error while compiling shader '{}':\n{}", 
                                          filename, error_string);
            throw std::runtime_error(msg);
#else 
            throw std::runtime_error("Cannot compile shader.");
#endif
        }
    }

    ~Shader() {
        glDeleteShader(shader_id);
    }


    friend class ShaderProgram;
private:
    unsigned int shader_id;
};

class VertexShader : Shader<VertexShader> {
public:
    friend class ShaderProgram;

    static constexpr char * extension = "vs";
    static constexpr GLenum shader_type = GL_VERTEX_SHADER;

    VertexShader(std::string filename) : Shader<VertexShader>(filename) {}
};

class FragmentShader : Shader<FragmentShader> {
public:
    friend class ShaderProgram;

    static constexpr char * extension = "fs";
    static constexpr GLenum shader_type = GL_FRAGMENT_SHADER;

    FragmentShader(std::string filename) : Shader<FragmentShader>(filename) {}
};

class ShaderProgram {
public:
    ShaderProgram(const VertexShader & vs, const FragmentShader & fs) {
        program_id = glCreateProgram();

        glAttachShader(program_id, vs.shader_id);
        glAttachShader(program_id, fs.shader_id);
        glLinkProgram(program_id);

        int success;
        glGetProgramiv(program_id, GL_LINK_STATUS, &success);
        if ( !success ) {
            char error_string[ERROR_STRING_LENGTH];
            glGetShaderInfoLog(program_id, ERROR_STRING_LENGTH, nullptr, error_string);

#if USE_FMT
            std::string msg = fmt::format("Error while linking shader:\n{}", error_string);
            throw std::runtime_error(msg);
#else 
            throw std::runtime_error("Cannot compile shader.");
#endif
        }
    }

    inline void Use() {
        glUseProgram(program_id);
    }

    UniformLocation GetUniformLocation(std::string name) {
        return UniformLocation((int) glGetUniformLocation(program_id, name.c_str()));
    }

    template <typename T>
    void SetUniformLocation(std::string name, const T & val) {
        SetUniformLocation(GetUniformLocation(name), val);
    }

    template <typename T>
    void SetUniformLocation(UniformLocation loc, const T & val) {
        constexpr auto size = utils::glm::get_vec_size<T>();
        int count = utils::glm::get_count(val);

        utils::glm::Uniform<std::get<0>(size), std::get<1>(size)>(*loc, count, 
                                                                  utils::glm::get_data(val), 
                                                                  utils::glm::get_vector_type<T>()
                                                                  );
    }

private:
    unsigned int program_id;
};


}