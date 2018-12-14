#pragma once
#ifndef TEXTURE_INFO_H
#define TEXTURE_INFO_H

#include "Texture.h"
#include "Catalog.h"
#include "ShaderProgram.h"

namespace engine {
    class TextureInfo {

        Texture *texture;
        unsigned int unit;
        unsigned int index;
        std::string uniform;

    public:

        TextureInfo();
        TextureInfo(const std::string& texName, const std::string& uniform, const unsigned int u, const unsigned int index);
        ~TextureInfo() = default;

        void Activate(ShaderProgram* sp);

    };

}


#endif // !TEXTURE_INFO_H


