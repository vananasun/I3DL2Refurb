#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define GLEW_STATIC
#include <gl/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <webp/decode.h>
#include "vananagui/platform/BinaryResource.h"
#include "vananagui/renderer/Texture.h"
#include "vananagui/Vanme.h"
#include "vananagui/base.h"



Texture::Texture(const char* resourcename, int frames) {

    BinaryResource* res = BinaryResource::Load(resourcename, "WEBP");
    uint8_t* pixels = WebPDecodeBGRA((uint8_t*)res->data, res->size, &this->width, &this->height);
    #ifdef DEBUG
    if (!pixels) {
        printf("Failed to decode WebP of size %ix%i\n", this->width, this->height);
        return;
    }
    #endif

    this->frames = frames;

    glGenTextures(1, &this->id);
    glBindTexture(GL_TEXTURE_2D, this->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, (char*)pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    SAFE_DELETE_PTR(res);
}

Texture::~Texture() {
    glDeleteTextures(1, &this->id);
}
