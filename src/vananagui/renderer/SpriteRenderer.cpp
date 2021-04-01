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
#include "vananagui/renderer/Texture.h"
#include "vananagui/renderer/SpriteRenderer.h"
#include "vananagui/base.h"



SpriteRenderer::SpriteRenderer() {
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

    // uv buffer
    // static const float uvs[] = {
    //     0.0f, 0.0f,
    //     1.0f, 0.0f,
    //     1.0f, 1.0f,
    //     0.0f, 0.0f,
    //     1.0f, 1.0f,
    //     0.0f, 1.0f
    // };
    glCreateBuffers(1, &this->uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->uvBuffer);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, uvs, GL_DYNAMIC_DRAW);

    // uniform and attribute locations
    auto oldShader = Van::me().shaderTexture->use();
    this->locMatrix   = Van::me().shaderTexture->locateUniform("uMatrix");
    this->locSize     = Van::me().shaderTexture->locateUniform("uSize");
    this->locSampler  = Van::me().shaderTexture->locateUniform("uSampler");
    this->locVertices = Van::me().shaderTexture->locateAttribute("aVertices");
    this->locUVs      = Van::me().shaderTexture->locateAttribute("aUVs");
}



SpriteRenderer::~SpriteRenderer() {
    glDeleteBuffers(1, &this->uvBuffer);
    glDeleteBuffers(1, &this->vertexBuffer);
}



void SpriteRenderer::render(Texture& tex, glm::vec2 pos, glm::vec2 scale, float rotation, glm::vec2 origin) {

    auto oldShader = Van::me().shaderTexture->use();
    glm::mat4 m = glm::translate(Van::me().getProjectionMatrix(), glm::vec3(pos.x + origin.x, pos.y + origin.y, 0));
    m = glm::rotate(m, rotation, glm::vec3(0, 0, 1));
    m = glm::translate(m, glm::vec3(-origin.x, -origin.y, 0));
    Van::me().shaderTexture->uniformMat4f(this->locMatrix, m);
    Van::me().shaderTexture->uniform2f(this->locSize, glm::vec2(tex.width * scale.x, tex.height * scale.y));

    // texture
    Van::me().shaderTexture->sampler(this->locSampler, tex.id);

    // vertices
    glEnableVertexAttribArray(this->locVertices);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
    glVertexAttribPointer(this->locVertices, 2, GL_FLOAT, false, 0, 0);

    // uvs
    static float uvs[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };
    glEnableVertexAttribArray(this->locUVs);
    glBindBuffer(GL_ARRAY_BUFFER, this->uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, uvs, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(this->locUVs, 2, GL_FLOAT, false, 0, 0);

    // draw
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // oldShader->use();

}

void SpriteRenderer::renderHorizontalBar(Texture& tex, glm::vec2 pos, glm::vec2 scale, float value) {

    Van::me().shaderTexture->use();
    glm::mat4 m = glm::translate(Van::me().getProjectionMatrix(), glm::vec3(pos.x, pos.y, 0));
    Van::me().shaderTexture->uniformMat4f(this->locMatrix, m);
    Van::me().shaderTexture->uniform2f(this->locSize, glm::vec2(tex.width * scale.x * value, tex.height * scale.y));

    // texture
    Van::me().shaderTexture->sampler(this->locSampler, tex.id);

    // vertices
    glEnableVertexAttribArray(this->locVertices);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
    glVertexAttribPointer(this->locVertices, 2, GL_FLOAT, false, 0, 0);

    // uvs
    float uvs[] = {
        0.0f , 0.0f,
        value, 0.0f,
        value, 1.0f,
        0.0f , 0.0f,
        value, 1.0f,
        0.0f , 1.0f
    };
    glEnableVertexAttribArray(this->locUVs);
    glBindBuffer(GL_ARRAY_BUFFER, this->uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, uvs, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(this->locUVs, 2, GL_FLOAT, false, 0, 0);

    // draw
    glDrawArrays(GL_TRIANGLES, 0, 6);

}


void SpriteRenderer::renderIndexFromStrip(Texture& tex, glm::vec2 pos, int index, int frames, glm::vec2 frameSize) {

    auto oldShader = Van::me().shaderTexture->use();
    glm::mat4 m = glm::translate(Van::me().getProjectionMatrix(), glm::vec3(pos.x, pos.y, 0));
    Van::me().shaderTexture->uniformMat4f(this->locMatrix, m);
    Van::me().shaderTexture->uniform2f(this->locSize, glm::vec2(frameSize.x, frameSize.y));

    // texture
    Van::me().shaderTexture->sampler(this->locSampler, tex.id);

    // vertices
    glEnableVertexAttribArray(this->locVertices);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
    glVertexAttribPointer(this->locVertices, 2, GL_FLOAT, false, 0, 0);

    // uvs
    float w = (frameSize.x) / tex.width;
    float xx = (1.0 / frames) * index;
    float uvs[] = {
        xx  , 0.0f,
        xx+w, 0.0f,
        xx+w, 1.0f,
        xx  , 0.0f,
        xx+w, 1.0f,
        xx  , 1.0f
    };
    glEnableVertexAttribArray(this->locUVs);
    glBindBuffer(GL_ARRAY_BUFFER, this->uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, uvs, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(this->locUVs, 2, GL_FLOAT, false, 0, 0);

    // draw
    glDrawArrays(GL_TRIANGLES, 0, 6);
    oldShader->use();

}
