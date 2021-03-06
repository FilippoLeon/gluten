#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <string>

#include "gluten/utils/include_fmt.hpp"

#include "gluten/debug/log.hpp"
#include "gluten/debug/exceptions.hpp"
#include "gluten/osutils/pathutils.hpp"
#include "gluten/utils/glm.hpp"

#include "gluten/camera/Light.hpp"

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
    Shader(std::string filename) : Shader({}, filename) {
    }

    std::vector<std::string> defines;

    Shader(std::initializer_list<std::string> defines, std::string filename) {
        if (std::string ext = osutils::GetExtension(filename); ext != Derived::extension) {
            debug::LogWarning("Extension {} does not match propery Shader extension {}", 
                              ext, Derived::extension);
        }

        this->defines.push_back("#version 330 core\n");
        for (const std::string & d: defines) {
            this->defines.push_back("#define " + d + "\n");
        }

        shader_id = glCreateShader(Derived::shader_type);

        std::ifstream stream(filename);
        if (!stream.is_open()) {
#if USE_FMT
            throw debug::RuntimeException(fmt::format("Could not open file '{}'.", filename));
#else 
            throw debug::RuntimeException("Cannot open file.");
#endif
        }
        std::string source;

        stream.seekg(0, std::ios::end);
        source.reserve((std::size_t) stream.tellg());
        stream.seekg(0, std::ios::beg);

        source.assign((std::istreambuf_iterator<char>(stream)),
                       std::istreambuf_iterator<char>());

        const char * str_arr = source.c_str();
        if (this->defines.size() > 0) {
            std::vector<const char *> strs;
            strs.reserve(this->defines.size() + 1);
            for (std::string & d : this->defines) {
                strs.push_back(d.c_str());
            }
            strs.push_back(str_arr);

            glShaderSource(shader_id, strs.size(), strs.data(), nullptr);
        } else {
            glShaderSource(shader_id, 1, &str_arr, nullptr);
        }
        glCompileShader(shader_id); 
        
        int  success;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

        if (!success) {
            char error_string[ERROR_STRING_LENGTH];
            glGetShaderInfoLog(shader_id, ERROR_STRING_LENGTH, NULL, error_string);

#if USE_FMT
            std::string msg = fmt::format("Error while compiling shader '{}':\n{}", 
                                          filename, error_string);
            throw debug::RuntimeException(msg);
#else 
            throw debug::RuntimeException("Cannot compile shader:\n" + std::string(error_string));
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

    static constexpr const char * extension = "vs";
    static constexpr GLenum shader_type = GL_VERTEX_SHADER;

    VertexShader(std::initializer_list<std::string> defines,
                 std::string filename) : Shader<VertexShader>(defines, filename) {}

    VertexShader(std::string filename) : Shader<VertexShader>(filename) {}
};

class FragmentShader : Shader<FragmentShader> {
public:
    friend class ShaderProgram;

    static constexpr const char * extension = "fs";
    static constexpr GLenum shader_type = GL_FRAGMENT_SHADER;

    FragmentShader(std::initializer_list<std::string> defines,
                   std::string filename) : Shader<FragmentShader>(defines, filename) {}

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
            throw debug::RuntimeException(msg);
#else 
            throw debug::RuntimeException("Cannot link shader:\n" + std::string(error_string));
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

    void SetLight(std::shared_ptr<camera::Light> light, int i) {
        std::string is = std::to_string(i);

        if (light == nullptr) {
            SetUniformLocation("lights[" + is + "].type", -1);
            return;
        }
        SetUniformLocation("lights[" + is + "].ambientColor", light->ambientColor);
        SetUniformLocation("lights[" + is + "].diffuseColor", light->diffuseColor);
        SetUniformLocation("lights[" + is + "].specularColor", light->specularColor);

        SetUniformLocation("lights[" + is + "].viewMatrix", light->GetViewMatrix());
        SetUniformLocation("lights[" + is + "].position", light->GetPosition());
        SetUniformLocation("lights[" + is + "].direction", light->GetDirection());
        SetUniformLocation("lights[" + is + "].cutoff", light->cutoff);
        SetUniformLocation("lights[" + is + "].type", light->type);
        SetUniformLocation("lights[" + is + "].fade", light->fade);
    }

private:
    unsigned int program_id;
};


}