#include <stdio.h>
#include <stdlib.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include "vananagui/renderer/ShaderProgram.h"

static ShaderProgram* s_boundShader;

void ShaderProgram::checkCompileErrors(GLuint handle) {
    GLint status;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
    if (GL_FALSE == status) {
        GLint length;
        glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &length);
        char* logStr = (char*)malloc(length);
        glGetShaderInfoLog(handle, length, NULL, logStr);
        printf("Shader compilation failed:\n%s\n", logStr);
        free(logStr);
    }
}

void ShaderProgram::checkLinkErrors() {
    GLint status;
    glGetProgramiv(this->program, GL_LINK_STATUS, &status);
    if (GL_FALSE == status) {
        // allocate the OpenGL log string
        GLint length;
        glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &length);
        char* logStr = (char*)malloc(length);
        // retrieve log
        glGetProgramInfoLog(this->program, length, NULL, logStr);
        // print error
        printf("Shader linkage failed:\n%s\n", logStr);
        // free log string
        free(logStr);
    }

    int error;
    if ((error = glGetError()) != GL_NO_ERROR) {
        printf("Linkage error code: %xh.\n", error);
        return;
    }
}

bool ShaderProgram::checkFramebufferStatus() {
    GLenum status = (GLenum)glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    switch(status) {
    case GL_FRAMEBUFFER_COMPLETE_EXT:
        return true;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
        printf("Framebuffer incomplete, incomplete attachment\n");
        return false;
    case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
        printf("Unsupported framebuffer format\n");
        return false;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
        printf("Framebuffer incomplete, missing attachment\n");
        return false;
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
        printf("Framebuffer incomplete, attached images must have same dimensions\n");
        return false;
    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
        printf("Framebuffer incomplete, attached images must have same format\n");
        return false;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
        printf("Framebuffer incomplete, missing draw buffer\n");
        return false;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
        printf("Framebuffer incomplete, missing read buffer\n");
        return false;
    }
    return false;
}



ShaderProgram::ShaderProgram() {
    this->program = glCreateProgram();
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(this->program);
}

/**
 * @returns ShaderProgram* previously bound shader program.
 */
ShaderProgram* ShaderProgram::use() {
    glUseProgram(this->program);
    ShaderProgram* old = s_boundShader;
    s_boundShader = this;
    return old;
}

void ShaderProgram::addStage(const char* source, ShaderStage stage) {
    // Compile shader
    GLuint shader = glCreateShader(stage);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    checkCompileErrors(shader);

    // Attach shader to program
    glAttachShader(this->program, shader);
    VERTEX == stage ? this->vert = shader : this->frag = shader;
}

void ShaderProgram::link() {
    glLinkProgram(this->program);
    checkLinkErrors();
    if (this->frag) { glDetachShader(this->program, this->frag); glDeleteShader(this->frag); }
    if (this->vert) { glDetachShader(this->program, this->vert); glDeleteShader(this->vert); }
}

GLint ShaderProgram::locateUniform(const char* name) {
    GLint loc = glGetUniformLocation(this->program, name);
    #ifdef DEBUG
    if (-1 == loc)
        printf("Unable to find uniform location \"%s\"\n", name);
    #endif
    return loc;
}

GLint ShaderProgram::locateAttribute(const char* name) {
    GLint loc = glGetAttribLocation(this->program, name);
    #ifdef DEBUG
    if (-1 == loc)
        printf("Unable to find attribute location \"%s\"\n", name);
    #endif
    return loc;
}

void ShaderProgram::uniform4f(GLint loc, glm::vec4 v) {
    glUniform4f(loc, v.x, v.y, v.z, v.w);
}

void ShaderProgram::uniform3f(GLint loc, glm::vec3 v) {
    glUniform3f(loc, v.x, v.y, v.z);
}

void ShaderProgram::uniform2f(GLint loc, glm::vec2 v) {
    glUniform2f(loc, v.x, v.y);
}

void ShaderProgram::uniform4i(GLint loc, glm::ivec4 v) {
    glUniform4i(loc, v.x, v.y, v.z, v.w);
}

void ShaderProgram::uniform3i(GLint loc, glm::ivec3 v) {
    glUniform3i(loc, v.x, v.y, v.z);
}

void ShaderProgram::uniform2i(GLint loc, glm::ivec2 v) {
    glUniform2i(loc, v.x, v.y);
}

void ShaderProgram::uniformMat4f(GLint loc, glm::mat4 m) {
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
}

void ShaderProgram::sampler(GLint loc, GLuint texture, int unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(loc, unit); // funnily enough it has to be 0 + n in the shader
}
