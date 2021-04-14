#ifndef RENDER_ENGINE_SHADER_H
#define RENDER_ENGINE_SHADER_H

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <exception>

#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <glm/gtc/type_ptr.hpp>

#include "texture.h"
#include "light.h"


struct ShaderLocation {
    static const GLuint position;
    static const GLuint normal;
    static const GLuint texcoord;
};


struct ShaderMatrixName {
    static const std::string model;
    static const std::string view;
    static const std::string projection;
};


class Shader {
public:
    GLint id = 0;

    Shader(const std::string &vertex, const std::string &fragment);

    Shader(const std::string &vertex, const std::string &geometry, const std::string &fragment);

    void uniform_vec4_set(const std::string &name, const glm::vec4 &v);

    void uniform_float_set(const std::string &name, GLfloat value);

    void uniform_vec3_set(const std::string &name, const glm::vec3 &v);

    void uniform_mat4_set(const std::string &name, const glm::mat4 &m);

    void uniform_tex2d_set(const std::string &name, GLuint texture_unit);

    void use() const;

protected:

    /**
     * 缓存：着色器中 uniform 变量的名称 和  location
     * @example
     * { "name": location, }
     */
    std::map<std::string, GLint> uniform_location_map;

    static GLuint shader_prog_get(const std::string &vert_shader_file,
                                  const std::string &frag_shader_file,
                                  const std::string &geom_shader_file="");

    static GLuint shader_compile(const std::string &file_name, GLenum shader_type);

    GLint unifrom_location_get(const std::string &name);
};


/**
 * 适配各种 Light 类和 shader 的
 */
class ShaderExtLight {
private:

    /* 设置光的基本颜色 */
    static void set_light(Shader &shader, const Light &light, const std::string &name);

    /* 设置距离衰减系数 */
    static void set_attenuation(Shader &shader, const AttenuationDistance& attenuation, const std::string &name);

public:
    static void set(Shader &shader, const PointLight &light, const std::string &name);

    static void set(Shader &shader, const DirLight &light, const std::string &name);

    static void set(Shader &shader, const SpotLight &light, const std::string &name);

};


class ShaderExtMVP {
public:
    static void set(Shader &shader, const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &proj);
};


class ShaderTextureName {
public:
    static std::string get(TextureType texture_type, GLuint index) {
        std::string type_str;
        switch (texture_type) {
            case diffuse: type_str = "diffuse"; break;
            case specular: type_str = "specular"; break;
            case normal: type_str = "normal"; break;
        }

        return "material.texture_" + type_str + "_" + std::to_string(index);
    }
};


#endif //RENDER_SHADER_H
