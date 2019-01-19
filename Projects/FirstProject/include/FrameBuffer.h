#pragma once
#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H
#include <string>


#define FRAME_BUFFER_MANAGER Catalog<FrameBuffer*>::instance()


namespace engine{
    class FrameBuffer{

        unsigned int bufferId;

        public:

        static constexpr const char* const REFLECTION = "REFLECTION";
        static constexpr const char* const G_POSITION = "G_POSITION";
        static constexpr const char* const G_NORMAL = "G_NORMAL";
        static constexpr const char* const G_ALBEDOS_PEC = "G_ALBEDO_SPEC";
        static constexpr const char* const G_BRIGHT = "G_BRIGHT";
        static constexpr const char* const G_DEPTH_BUFFER = "G_DEPTH_BUFFER";

        FrameBuffer();
        FrameBuffer(const char* const textureName, int textureType);
        ~FrameBuffer();
        unsigned int getId() const;
    };
}

#endif


