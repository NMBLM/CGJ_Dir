#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include "GL/glew.h"
#include "stb_image.h"
#include <iostream>

#define DEFAULT_TEXTURE "Textures/errorTexture.jpg"
namespace engine{

    class Texture{

        int width;
        int height;
        int nrChannels;
        unsigned int textureId;

        public:
        Texture() = default;
        Texture( const char* filename );
        ~Texture() = default;

        void loadDefault();
        void activate(const unsigned int unit);
    };
}
#endif // !TEXTURE_H

