#pragma once
#include <GL/gl.h>
#include <GL/glext.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

enum ShaderStage {
    FRAGMENT = GL_FRAGMENT_SHADER_ARB, VERTEX = GL_VERTEX_SHADER_ARB
};

class ShaderProgram {
protected:
    GLuint frag, vert, program;

    void checkCompileErrors(GLuint handle);
    void checkLinkErrors();
    bool checkFramebufferStatus();
public:
    ShaderProgram();
    ~ShaderProgram();

    /**
     * Bind the shader for usage.
     *
     * @return ShaderProgram* Previously bound shader.
     */
    ShaderProgram* use();

    /**
     * Add a stage to the shader program.
     *
     * @param const char* source GLSL code.
     * @param ShaderStage stage  VERTEX, FRAGMENT.
     */
    void addStage(const char* source, ShaderStage stage);

    /**
     * Link stages into a ready-for-use shader program.
     */
    void link();

    /**
     * Get shader program handle.
     *
     * @return GLuint
     */
    GLuint getProgram() { return program; };

    /**
     * Locate a uniform variable.
     *
     * @param const char* name The variable name.
     *
     * @return GLint loc
     */
    GLint locateUniform(const char* name);

    /**
     * Locate an attribute.
     *
     * @param const char* name
     *
     * @return GLint loc
     */
    GLint locateAttribute(const char* name);

    /**
     * Bind a uniform vector to the shader program.
     *
     * @param GLint loc
     * @param glm::vec4 v
     */
    void uniform4f(GLint loc, glm::vec4 v);

    /**
     * Bind a uniform vector to the shader program.
     *
     * @param GLint loc
     * @param glm::vec3 v
     */
    void uniform3f(GLint loc, glm::vec3 v);

    /**
     * Bind a uniform vector to the shader program.
     *
     * @param GLint loc
     * @param glm::vec2 v
     */
    void uniform2f(GLint loc, glm::vec2 v);

    /**
     * Bind a uniform vector to the shader program.
     *
     * @param GLint loc
     * @param glm::ivec4 v
     */
    void uniform4i(GLint loc, glm::ivec4 v);

    /**
     * Bind a uniform vector to the shader program.
     *
     * @param GLint loc
     * @param glm::ivec3 v
     */
    void uniform3i(GLint loc, glm::ivec3 v);

    /**
     * Bind a uniform vector to the shader program.
     *
     * @param GLint loc
     * @param glm::ivec2 v
     */
    void uniform2i(GLint loc, glm::ivec2 v);

    /**
     * Bind a uniform 4x4 matrix to the shader program.
     *
     * @param GLint loc
     * @param glm::mat4 m
     */
    void uniformMat4f(GLint loc, glm::mat4 m);

    /**
     * Binds a texture sampler uniform to the shader program.
     *
     * @param GLint  loc             Sampler's uniform location.
     * @param GLuint texture         The GL texture identifier.
     * @param int    unit (optional) GL texture unit to bind to, the value must
     *                               be 0 + n.
     */
    void sampler(GLint loc, GLuint texture, int unit = 0);

};
