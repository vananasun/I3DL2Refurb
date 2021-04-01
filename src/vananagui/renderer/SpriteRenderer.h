#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>
#include <glm/ext/vector_float2.hpp>
#include "vananagui/renderer/Texture.h"



class SpriteRenderer {
private:

    GLuint vertexBuffer, uvBuffer;

    GLint locVertices, locUVs, locMatrix, locSize, locSampler;


public:

    /**
     * Constructor.
     */
    SpriteRenderer();

    /**
     * Cleanup renderer.
     */
    ~SpriteRenderer();

    /**
     * Draw a sprite.
     *
     * @param Texture& tex
     * @param glm::vec2 pos
     * @param glm::vec2 scale (optional)
     * @param float rotation (optional)
     * @param glm::vec2 origin (optional)
     */
    void render(Texture& tex, glm::vec2 pos, glm::vec2 scale = {1.0f, 1.0f}, float rotation = 0.0f, glm::vec2 origin = {0.0f, 0.0f});

    /**
     * Draw a textured horizontal slider.
     *
     * @param Texture& tex
     * @param glm::vec2 pos
     * @param glm::vec2 scale
     * @param float value
     */
    void renderHorizontalBar(Texture& tex, glm::vec2 pos, glm::vec2 scale, float value);

    /**
     * Render a frame of a sprite strip.
     *
     * @param Texture& tex
     * @param glm::vec2 pos
     * @param int index
     * @param int frames
     * @param glm::vec2 frameSize
     */
    void renderIndexFromStrip(Texture& tex, glm::vec2 pos, int index, int frames, glm::vec2 frameSize);


};
