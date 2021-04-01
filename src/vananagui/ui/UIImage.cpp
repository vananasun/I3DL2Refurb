#include "vananagui/ui/UIImage.h"
#include "vananagui/renderer/Texture.h"
#include "vananagui/Vanme.h"
#include "vananagui/base.h"



UIImage::UIImage(const char* image, glm::vec2 pos) {
    this->tex = new Texture(image);
    this->pos = pos;
}

void UIImage::render() {
    Van::me().spriteRenderer->render(*this->tex, this->pos);
}
