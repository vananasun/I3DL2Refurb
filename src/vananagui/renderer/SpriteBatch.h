#ifdef FALSE
#pragma once
#include <vector>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>
#include <glm/ext/vector_float2.hpp>



class SpriteBatch {
private:

    struct VERTEX {
        float x;
        float y;
        float xScale;
        float yScale;
        // glm::vec2 pos; glm::vec2 size;
    };

    int vertexSize;

    int maxSprites;

    int maxIndices;

    ShaderProgram& shader;

    std::vector<GLuint, VERTEX> batch;


public:
    /**
     * Initializes a sprite batch renderer on the given shader.
     *
     * @param ShaderProgram& shader
     * @param int            maxSprites (optional)
     */
    SpriteBatch(ShaderProgram& shader, int maxSprites = 200);

    /**
     * Destroys the GL associated memory objects from the sprite batch.
     */
    ~SpriteBatch();

    /**
     * Push a sprite to the batch.
     *
     * @param GLuint    tex
     * @param glm::vec2 pos
     * @param glm::vec2 size
     */
    void push(GLuint tex, glm::vec2 pos, glm::vec2 size);

    /**
     * Render all the sprites and empty the batch.
     */
    void wash();

};
#endif
