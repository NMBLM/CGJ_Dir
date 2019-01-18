#pragma once
#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H
#include <string>


#define FRAME_BUFFER_MANAGER Catalog<FrameBuffer*>::instance()


namespace engine{
    class FrameBuffer{

        unsigned int bufferId{};


        public:

        static constexpr const char* const REFLECTION = "REFLECTION";

        FrameBuffer();
        FrameBuffer(const char* const textureName, int textureType);
        ~FrameBuffer();
        unsigned int getId() const;
    };
}

#endif


