#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <vector>
#include "GL/glew.h"
#include "stb_image.h"
#include <iostream>
#include "Vector.h"
#include <vector>

#define DEFAULT_TEXTURE "Textures/errorTexture.jpg"
#define NOODLE_TEXTURE_PATH "Textures/errorTexture.jpg" //TODO
#define DEFAULT_TEXTURE "Textures/errorTexture.jpg" //TODO
namespace engine {

    class Texture {

    public:

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

        static constexpr const char* const SSAO_TEXTURE = "SSAO_TEXTURE";
        static constexpr const char* const SSAO_BLUR_TEXTURE = "SSAO__BLUR_TEXTURE";
        static constexpr const char* const SSAO_NOISE = "SSAO__NOISE";

        static constexpr const char* const HDR_COLOR = "HDR_COLOR";
        static constexpr const char* const BRIGHTNESS = "BRIGHTNESS";

        static constexpr const char* const PINGPONG_COLOR1 = "PINGPONG_COLOR1";
        static constexpr const char* const PINGPONG_COLOR2 = "PINGPONG_COLOR2";

        static constexpr const char* const G_POSITION = "G_POSITION";
        static constexpr const char* const G_NORMAL = "G_NORMAL";
        static constexpr const char* const G_ALBEDO_SPEC = "G_ALBEDO_SPEC";
        static constexpr const char* const G_BRIGHT = "G_BRIGHT";



        Texture();
        Texture(GLint i);
    protected:
        unsigned int textureId;
        GLint target;
        ~Texture() = default;

    public:
        void LoadDefault();
        unsigned int GetId() const;
        GLuint GetType() const;
        void SetTextureWrapS(const unsigned int option) const;
        void SetTextureWrapT(const unsigned int option) const;
        void SetTextureWrapR(const unsigned int option) const;
        void SetTextureMinFilter(const unsigned int option ) const;
        void SetTextureMagFilter(const unsigned  int option ) const;
        void LoadTexture(const char* filename, GLint level = 0, GLint internalFormat = GL_RGB, GLenum format = GL_RGB );

    };

    class TextureCube : public Texture {

    public:
        std::vector<std::string> textures_faces;

        TextureCube();
        TextureCube(const char* filename, const char* filetype, bool bigRGB);
        TextureCube(const char* filename, const char* filetype, bool bigRGB, bool fixed);
        //void LoadTexture( GLint level, GLint internalFormat, GLenum format);
        ~TextureCube() = default;

    };

    class RenderTexture : public Texture {
      
    public:
        RenderTexture();
        RenderTexture(unsigned textureType);
        void LoadTexture(const int width, const int height, GLint level, GLint internalFormat, GLenum format, GLenum type);
        void LoadTexture(const int width, const int height, GLint level, GLint internalFormat, GLenum format, GLenum type,
                        std::vector<vec3>::pointer data);

    };

}
#endif // !TEXTURE_H

