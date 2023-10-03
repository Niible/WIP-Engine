#include "Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "../../../Utils/Factory.h"

using namespace Engine;

Shader::Shader(const std::string& vertex_shader_file_path, const std::string& fragment_shader_file_path): m_name("")
{
	const std::string vertex_shader_src = readShaderFile(vertex_shader_file_path);
	const std::string fragment_shader_src = readShaderFile(fragment_shader_file_path);
	const GLuint vertex_shader = compileShader(vertex_shader_src.c_str(), GL_VERTEX_SHADER);
	const GLuint fragment_shader = compileShader(fragment_shader_src.c_str(), GL_FRAGMENT_SHADER);

    shaderProgram = createProgram(vertex_shader, fragment_shader);
}

Shader::Shader(const std::string& name, const std::string& vertex_shader_string, const std::string& fragment_shader_string): m_name(name)
{
    const GLuint vertexShader = compileShader(vertex_shader_string.c_str(), GL_VERTEX_SHADER);
    const GLuint fragmentShader = compileShader(fragment_shader_string.c_str(), GL_FRAGMENT_SHADER);

    shaderProgram = createProgram(vertexShader, fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(shaderProgram);
}

void Shader::bind() {
    glUseProgram(shaderProgram);
}

void Shader::unbind() {
    glUseProgram(0);
}

GLint Shader::getUniformLocation(const char* name) const
{
    return glGetUniformLocation(shaderProgram, name);
}

GLuint Shader::compileShader(const char* src, const GLenum shader_type) {
	const GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLsizei length;
        glGetShaderiv(shaderProgram, GL_INFO_LOG_LENGTH, &length);
        auto* log = new GLchar(length);

        glGetShaderInfoLog(shader, length, &length, log);
        std::cerr << "Shader compilation failed" << log << std::endl;

        delete[] log;
        log = nullptr;
    }

    return shader;
}

GLuint Shader::createProgram(const GLuint vertex_shader, const GLuint fragment_shader)
{
	const GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    GLint success;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {

        GLsizei length;
        glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &length);
        auto* log = new GLchar(length + 1);

        glGetProgramInfoLog(shader_program, length, &length, log);
        std::cerr << "Shader program linking failed: " << log << std::endl;

        delete[] log;
        log = nullptr;

        return 0;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}

std::string Shader::readShaderFile(const std::string& file_path)
{
    std::string result;
    std::ifstream file(file_path, std::ios::in | std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open shader file: " << file_path << std::endl;
        return nullptr;
    }

    file.seekg(0, std::ios::end);
    const size_t size = file.tellg();
    if (size != -1)
    {
        result.resize(size);
        file.seekg(0, std::ios::beg);
        file.read(result.data(), size);
    }

    return result;
}

void Shader::setInt(const std::string& name, const int value)
{
    glUniform1i(getUniformLocation(name.c_str()), value);
}

void Shader::setIntArray(const std::string& name, const int* values, uint32_t count)
{
    glUniform1iv(getUniformLocation(name.c_str()), count, values);
}

void Shader::setFloat(const std::string& name, const float value)
{
    glUniform1f(getUniformLocation(name.c_str()), value);
}

void Shader::setFloat2(const std::string& name, const glm::vec2& value)
{
    glUniform2f(getUniformLocation(name.c_str()), value.x, value.y);
}

void Shader::setFloat3(const std::string& name, const glm::vec3& value)
{
    glUniform3f(getUniformLocation(name.c_str()), value.x, value.y, value.z);
}

void Shader::setFloat4(const std::string& name, const glm::vec4& value)
{
    glUniform4f(getUniformLocation(name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::setMat3(const std::string& name, const glm::mat3& matrix)
{
    glUniformMatrix3fv(getUniformLocation(name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setMat4(const std::string& name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(getUniformLocation(name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

// Shader library

void ShaderLibrary::add(const std::string& name, const std::shared_ptr<Shader>& shader)
{
    //static_assert(!Exists(name), "Shader already exists!");
    m_shaders[name] = shader;
}

void ShaderLibrary::add(const std::shared_ptr<Shader>& shader)
{
    if (!shader->m_name.empty())
		m_shaders[shader->m_name] = shader;
}

std::shared_ptr<Shader> ShaderLibrary::load(const std::string& name, const std::string& vertex_shader_file_path, const std::string& fragment_shader_file_path)
{
    auto shader = factory::create<Shader>(vertex_shader_file_path, fragment_shader_file_path);
	add(name, shader);
	return shader;
}

std::shared_ptr<Shader> ShaderLibrary::get(const std::string& name)
{
    return m_shaders[name];
}

bool ShaderLibrary::exists(const std::string& name) const
{
    return m_shaders.contains(name);
}