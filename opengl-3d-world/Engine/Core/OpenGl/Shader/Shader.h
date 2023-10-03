#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <GL/glew.h>
#include <glm/fwd.hpp>
#include "../../../Utils/Factory.h"

namespace Engine
{
	
class Shader {
public:

    Shader(const std::string& vertex_shader_file_path, const std::string& fragment_shader_file_path);
    Shader(const std::string& name, const std::string& vertex_shader_string, const std::string& fragment_shader_string);
    ~Shader();

    void bind();
    void unbind();

    void setInt(const std::string& name, int value);

    void setIntArray(const std::string& name, const int* values, uint32_t count);

    void setFloat(const std::string& name, float value);

    void setFloat2(const std::string& name, const glm::vec2& value);

    void setFloat3(const std::string& name, const glm::vec3& value);

    void setFloat4(const std::string& name, const glm::vec4& value);

    void setMat3(const std::string& name, const glm::mat3& matrix);

    void setMat4(const std::string& name, const glm::mat4& matrix);

    std::string m_name;

private:
    GLuint compileShader(const char* src, GLenum shader_type);

    GLuint createProgram(GLuint vertex_shader, GLuint fragment_shader);

    std::string readShaderFile(const std::string& file_path);

    GLint getUniformLocation(const char* name) const;

    GLuint shaderProgram;

};

class ShaderLibrary
{
public:
    using factory = Factory<TypeList<Shader>, std::shared_ptr>;

    void add(const std::string& name, const std::shared_ptr<Shader>& shader);
    void add(const std::shared_ptr<Shader>& shader);

    std::shared_ptr<Shader> load(const std::string& name, const std::string& vertex_shader_file_path, const std::string& fragment_shader_file_path);

    std::shared_ptr<Shader> get(const std::string& name);

    bool exists(const std::string& name) const;
private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;
};

}
