#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>



class RectangleRenderer {
private:

    GLuint vertexBuffer;

    GLint locVertices, locMatrix, locPosition, locColor;


public:

    /**
     * Constructor.
     */
    RectangleRenderer();

    /**
     * Cleanup renderer.
     */
    ~RectangleRenderer();

    /**
     * Draw a colored rectangle.
     *
     * @param glm::vec2 pos
     * @param glm::vec2 size
     * @param glm::vec4 color
     */
    void render(glm::vec2 pos, glm::vec2 size, glm::vec4 color);



};
