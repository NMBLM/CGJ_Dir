#pragma once
#ifndef TEXTURE_INFO_H
#define TEXTURE_INFO_H

namespace engine {
    class TextureInfo {
    public:
        static unsigned int unit;

        TextureInfo();
        ~TextureInfo() = default;

        void increaseUnit();
    };

}


#endif // !TEXTURE_INFO_H


