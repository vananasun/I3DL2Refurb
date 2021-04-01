#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>
#include <glm/ext/vector_float2.hpp>



class Texture {
public:
    /**
     * @constructor
     *
     * Loads a WebP texture from a binary resource structure.
     *
     * @param const char* resourcename  - Name of image in resource script.
     * @param int frames (optional)     - Number of animation frames.
     */
    Texture(const char* resourcename, int frames = 1);

    /**
     * @destructor
     */
    ~Texture();

    /**
     * Draw the texture.
     *
     * @param VEC2I pos            Coordinates.
     * @param VEC2I origin         Origin to rotate around.
     * @param VEC2I scale          Scale of the image.
     * @param int frame (optional) Strip's vertical frame to pick from.
     * @param float rotation (optional) Rotation around the Z axis.
     */
    void draw(glm::vec2 pos, glm::vec2 origin, glm::vec2 scale, int frame = 0, float rotation = 0.0);

    /**
     * Draw a horizontal filled slider texture.
     *
     * @param VEC2i pos   Rectangle coordinates.
     * @param VEC2f scale Image scale.
     * @param float value The value of the bar [0.0, 1.0].
     */
    void drawBarHorizontally(glm::vec2 pos, glm::vec2 scale, float value);



    GLuint id = GL_INVALID_VALUE;
    int width, height, frames;
};
