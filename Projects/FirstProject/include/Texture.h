#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include "GL/glew.h"
#include "stb_image.h"
#include <iostream>
#include "TextureInfo.h"

#define DEFAULT_TEXTURE "Textures/errorTexture.jpg"
namespace engine {

    class Texture {
        int width;
        int height;
        int nrChannels;
        unsigned int textureId;

        //TextureInfo *ti;

    public:
        static unsigned int unit;
        static constexpr const char* const WOOD = "WOOD";
        static constexpr const char* const DEFAULT = "ERROR";
        static constexpr const char* const NOODLE_TEXTURE = "NOODLE_TEXTURE";
        static constexpr const char* const NOODLE_MAP_NORMAL = "NOODLE_MAP_NORMAL";
        static constexpr const char* const NOODLE_MAP_SPECULAR = "NOODLE_MAP_SPECULAR";
        static constexpr const char* const NOODLE_MAP_AO = "NOODLE_MAP_AO";
        static constexpr const char* const NOODLE_MAP_DISPLACEMENT = "NOODLE_MAP_DISPLACEMENT";
        
        Texture();
        Texture(const char* filename);
        ~Texture() = default;

        void loadDefault();
        void activate();
    };
}
#endif // !TEXTURE_H

