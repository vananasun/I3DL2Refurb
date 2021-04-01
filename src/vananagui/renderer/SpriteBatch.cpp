#ifdef FALSE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define GLEW_STATIC
#include <gl/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include "ui/renderer/ShaderProgram.h"
#include "vananagui/base.h"



SpriteBatch::SpriteBatch(ShaderProgram& shader, int maxSprites) {
    // shader
    this->shader = shader;
    this->locVertex     = this->shader->locateAttribute("aVertex");
    this->locPosition   = this->shader->locateAttribute("aPosition");
    this->locSize       = this->shader->locateAttribute("aSize");
    this->locProjection = this->shader->locateUniform("uProjection");

    // limits
    this->vertexSize = 2 * 3; // components * number of attributes
    this->maxSprites = maxSprites;
    this->maxIndices = maxSprites * 6;

    // batch data
    this->vertices = static_cast<float*>(malloc(maxSprites * 4 * sizeof(float)));
    this->currentBatchSize = 0;

    // vertex buffer object
    this->vbo = glCreateBuffer();
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, this->vertices, GL_DYNAMIC_DRAW);

    // indices
    this->indices = static_cast<
}

SpriteBatch::~SpriteBatch() {

}

    // batch data
    this.vertices = new Float32Array(maxSprites * 4 * this.vertexSize);
    this.currentBatchSize = 0;

    // vertex buffer object
    this.vbo = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, this.vbo);
    gl.bufferData(gl.ARRAY_BUFFER, this.vertices, gl.DYNAMIC_DRAW);

    // indices
    let indices = new Uint16Array(this.maxIndices);
    for (let i = v = 0; i < this.maxIndices; i += 6, v += 4) {
        indices[i + 0] = v + 0;
        indices[i + 1] = v + 1;
        indices[i + 2] = v + 2;
        indices[i + 3] = v + 2;
        indices[i + 4] = v + 3;
        indices[i + 5] = v + 0;
    }
    this.vib = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.vib);
    gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, indices, gl.STATIC_DRAW);

    // Bind spritesheet texture
    let oldShader = shader.use();
    g_spriteSheet.bindToSampler(this.locAtlas);
    oldShader.use();
}


/**
 * Push a sprite to the batch.
 *
 * @param {string} spriteId
 * @param {number} x
 * @param {number} y
 * @param {number} xScale
 * @param {number} yScale
 */
SpriteBatch.prototype.push = function(spriteId, x = 0, y = 0, xScale = 1, yScale = 1) {
    // prepare position
    let s = g_spriteSheet.map[spriteId];
    x = (xScale >= 0) ? (x + (s.xOffset - s.xOrigin)) : (x - (s.w + s.xOffset - s.xOrigin));
    y = (yScale >= 0) ? (y + (s.yOffset - s.yOrigin)) : (y - (s.h + s.yOffset - s.yOrigin));

    // prepare UVs
    let u0 = s.u0;
    let v0 = s.v0;
    let u1 = s.u1;
    let v1 = s.v1;

    // prepare scale
    xScale *= s.w;
    yScale *= s.h;


    let index = 4 * this.vertexSize * this.currentBatchSize;
    let vertices = this.vertices;

    // 0
    vertices[index++] = 0.0;
    vertices[index++] = 0.0;
    vertices[index++] = x;
    vertices[index++] = y;
    vertices[index++] = xScale;
    vertices[index++] = yScale;
    vertices[index++] = u0;
    vertices[index++] = v0;
    // 1
    vertices[index++] = 1.0;
    vertices[index++] = 0.0;
    vertices[index++] = x;
    vertices[index++] = y;
    vertices[index++] = xScale;
    vertices[index++] = yScale;
    vertices[index++] = u1;
    vertices[index++] = v0;
    // 2
    vertices[index++] = 1.0;
    vertices[index++] = 1.0;
    vertices[index++] = x;
    vertices[index++] = y;
    vertices[index++] = xScale;
    vertices[index++] = yScale;
    vertices[index++] = u1;
    vertices[index++] = v1;
    // 3
    vertices[index++] = 0.0;
    vertices[index++] = 1.0;
    vertices[index++] = x;
    vertices[index++] = y;
    vertices[index++] = xScale;
    vertices[index++] = yScale;
    vertices[index++] = u0;
    vertices[index++] = v1;

    this.currentBatchSize++;
}

/**
 * Flush the sprite batch to the GPU.
 *
 * @param {boolean} useViewOffset
 */
SpriteBatch.prototype.render = function(useViewOffset = true) {
    // bind shader and uniforms
    let oldShader = this.shader.use();
    this.shader.bindMat4(this.locProjection, g_gameCanvas.projectionMatrix);
    if (useViewOffset && g_roomManager.currentRoom.view) {
        let pos = g_roomManager.currentRoom.view.getOffset();
        this.shader.bind2f(this.locViewOffset, pos.x, pos.y);
    } else
        this.shader.bind2f(this.locViewOffset, 0, 0);

    // bind buffers and set pointers
    gl.bindBuffer(gl.ARRAY_BUFFER, this.vbo);
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.vib);
    gl.enableVertexAttribArray(this.locVertex);
    gl.enableVertexAttribArray(this.locPosition);
    gl.enableVertexAttribArray(this.locScale);
    gl.enableVertexAttribArray(this.locUV);
    let stride = this.vertexSize * 4;
    gl.vertexAttribPointer(this.locVertex, 2, gl.FLOAT, false, stride, 0);
    gl.vertexAttribPointer(this.locPosition, 2, gl.FLOAT, false, stride, 2 * 4);
    gl.vertexAttribPointer(this.locScale, 2, gl.FLOAT, false, stride, 4 * 4);
    gl.vertexAttribPointer(this.locUV, 2, gl.FLOAT, false, stride, 6 * 4);

    // upload vertices
    let vertices = this.vertices.subarray(0, 4 * this.vertexSize * this.currentBatchSize);
    gl.bufferSubData(gl.ARRAY_BUFFER, 0, vertices);

}

void SpriteBatch::wash() {
    // bind shader and uniforms
    ShaderProgram* oldShader = this->shader.use();

    // upload vertices
    auto vertices = this->batch
    glBufferSubData(GL_ARRAY_BUFFER, 0, this->batch->size() * sizeof(VERTEX), );

    // draw the damn sprites
    glDrawElements(GL_TRIANGLES, this->batch->size() * 6, GL_UNSIGNED_SHORT, 0);

    // revert shader
    oldShader->use();

    // clear the batch
    this->batch.clear();

}


#endif
