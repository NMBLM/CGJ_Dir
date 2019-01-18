#include "FrameBuffer.h"
#include "Catalog.h"
#include <GL/glew.h>
#include <iostream>
#include "Texture.h"

namespace engine{
    class FrameBuffer;

    FrameBuffer::FrameBuffer() = default;

    FrameBuffer::FrameBuffer( const char* const textureName, int textureType ){
        Catalog<Texture*>* textureCatalog = Catalog<Texture*>::instance();

        //1. CREATE FRAMEBUFFER
        //1.1 Generate it and bind it so we can attach to it
        glGenFramebuffers( 1, &bufferId );
        glBindFramebuffer( GL_FRAMEBUFFER, bufferId );

        glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureType,
                                textureCatalog->get( textureName )->getId(), 0 );

        //1.6 Finally check if frameBuffer is complete
        if( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
            std::cout << "FrameBuffer not complete!" << std::endl;
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    }


    FrameBuffer::~FrameBuffer() = default;

    unsigned FrameBuffer::getId() const
    {
        return bufferId;
    }
}

