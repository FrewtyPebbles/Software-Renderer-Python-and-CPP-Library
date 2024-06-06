#include "Shader.h"


shader::shader(string source, ShaderType type) : source(source.c_str()), type(type) {}

void shader::compile() {
    std::cout << this->type << "\n";
    switch (this->type)
    {
    case ShaderType::FRAGMENT:
        this->shader_handle = glCreateShader(GL_FRAGMENT_SHADER);
        break;

    case ShaderType::VERTEX:
        this->shader_handle = glCreateShader(GL_VERTEX_SHADER);
        break;
    default:
        break;
    }
    std::cout << "created shaders\n";
    glShaderSource(this->shader_handle, 1, &this->source, NULL);
    glCompileShader(this->shader_handle);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(this->shader_handle, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(this->shader_handle, 512, NULL, infoLog);
        throw std::format("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n{}\n", infoLog);
    }
}

