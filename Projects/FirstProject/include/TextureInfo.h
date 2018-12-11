#pragma once
#ifndef TEXTURE_INFO_H
#define TEXTURE_INFO_H

#include "GL/glew.h"

namespace engine {
    class TextureInfo {
    public:
        static unsigned int unit;

        TextureInfo();
        void increaseUnit();
        ~TextureInfo() = default;
    };

}


#endif // !TEXTURE_INFO_H


