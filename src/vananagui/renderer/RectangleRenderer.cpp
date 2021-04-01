#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define GLEW_STATIC
#include <gl/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc//matrix_transform.hpp>
#include "vananagui/Vanme.h"
#include "vananagui/renderer/RectangleRenderer.h"
#include "vananagui/base.h"



RectangleRenderer::RectangleRenderer() {
    // vertex buffer
    static const float vertices[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };
    glCreateBuffers(1, &this->vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertices, GL_STATIC_DRAW);

    // uniform and attribute locations
    auto oldShader = Van::me().shaderRectangle->use();
    this->locMatrix   = Van::me().shaderRectangle->locateUniform("uMatrix");
    this->locPosition = Van::me().shaderRectangle->locateUniform("uPosition");
    this->locColor    = Van::me().shaderRectangle->locateUniform("uColor");
    this->locVertices = Van::me().shaderRectangle->locateAttribute("aVertices");
}



RectangleRenderer::~RectangleRenderer() {
    glDeleteBuffers(1, &this->vertexBuffer);
}



void RectangleRenderer::render(glm::vec2 pos, glm::vec2 size, glm::vec4 color) {

    Van::me().shaderRectangle->use();
    glm::mat4 m = Van::me().getProjectionMatrix();
    Van::me().shaderRectangle->uniformMat4f(this->locMatrix, m);
    Van::me().shaderRectangle->uniform4f(this->locPosition, glm::vec4(pos.x, pos.y, size.x, size.y));
    Van::me().shaderRectangle->uniform4f(this->locColor, color);

    // vertices
    glEnableVertexAttribArray(this->locVertices);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
    glVertexAttribPointer(this->locVertices, 2, GL_FLOAT, false, 0, 0);

    // draw
    glDrawArrays(GL_TRIANGLES, 0, 6);

}
