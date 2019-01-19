#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <vector>
#include "GL/glew.h"
#include "stb_image.h"
#include <iostream>

#define DEFAULT_TEXTURE "Textures/errorTexture.jpg"
#define NOODLE_TEXTURE_PATH "Textures/errorTexture.jpg" //TODO
#define DEFAULT_TEXTURE "Textures/errorTexture.jpg" //TODO
namespace engine {

    class Texture {

    public:
        static unsigned int unit;
        unsigned int thisUnit{};
        unsigned int textureId{};
        int width{};
        int height{};
        int nrChannels{};

        static constexpr const char* const WOOD = "WOOD";
        static constexpr const char* const TABLE = "TABLE";
        static constexpr const char* const TABLE_SPECULAR = "TABLE_SPECULAR";
        static constexpr const char* const TABLE_NORMAL = "TABLE_NORMAL";

        static constexpr const char* const WORLD_CUBE = "WORLD_CUBE";
        static constexpr const char* const BEACH_BOX = "BEACH_BOX";
        static constexpr const char* const DEFAULT = "ERROR";
        static constexpr const char* const NOODLE_TEXTURE = "NOODLE_TEXTURE";
        static constexpr const char* const NOODLE_MAP_NORMAL = "NOODLE_MAP_NORMAL";
        static constexpr const char* const NOODLE_MAP_SPECULAR = "NOODLE_MAP_SPECULAR";
        static constexpr const char* const NOODLE_MAP_AO = "NOODLE_MAP_AO";
        static constexpr const char* const NOODLE_MAP_DISPLACEMENT = "NOODLE_MAP_DISPLACEMENT";

        static constexpr const char* const  REFLECTION_RENDER_TEXTURE = "REFLECTION_RENDER_TEXTURE";
        static constexpr const char* const  CUBE_REFLECTION_RENDER_TEXTURE = "CUBE_REFLECTION_RENDER_TEXTURE";

        Texture();
        Texture(const char* filename);
    protected:
        ~Texture() = default;

    public:
        void LoadDefault();
        unsigned int getId();
        virtual GLuint getType();
    };

    class TextureCube : public Texture {

    public:
        std::vector<std::string> textures_faces;

        TextureCube();
        TextureCube(const char* filename, const char* filetype, bool bigRGB);
        TextureCube(const char* filename, const char* filetype, bool bigRGB, bool fixed);
        ~TextureCube() = default;
        virtual GLuint getType();

    };

    class RenderTexture : public Texture {
        /*
        glActiveTexture(unit);
        glBindTexture( texture->getType(), texture->getId() );
        glUniform1i(sp->UniformId(uniform.c_str()), index);
        */

    public:
        RenderTexture();
        RenderTexture(int width, int height);
        virtual GLuint getType();
    };

    class RenderCubeTexture: public Texture{
        /*
        glActiveTexture(unit);
        glBindTexture( texture->getType(), texture->getId() );
        glUniform1i(sp->UniformId(uniform.c_str()), index);
        */

        public:
        RenderCubeTexture();
        RenderCubeTexture( int width, int height );
        virtual GLuint getType();
    };
}
#endif // !TEXTURE_H

