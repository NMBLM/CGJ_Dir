#include "Texture.h"
namespace engine{


    Texture::Texture( const char* filename ){
        glGenTextures( 1, &textureId );
        glBindTexture( GL_TEXTURE_2D, textureId );
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        // load and generate the texture
        int width, height, nrChannels;
        unsigned char *data = stbi_load( filename, &width, &height, &nrChannels, 0 );
        if( data ){
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
            glGenerateMipmap( GL_TEXTURE_2D );
        } else{
            std::cout << "Failed to load texture in " << filename << std::endl;
            loadDefault();
        }
        stbi_image_free( data );
    }

    void Texture::loadDefault(){
        unsigned char *data = stbi_load( DEFAULT_TEXTURE, &width, &height, &nrChannels, 0 );
        if( data ){
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
            glGenerateMipmap( GL_TEXTURE_2D );
        } else{
            std::cout << "Failed to load default texture" << std::endl;
            exit( -10 );
        }
        stbi_image_free( data );
    }

    void Texture::activate( const unsigned int unit ){
        glActiveTexture( unit );
        glBindTexture( GL_TEXTURE_2D, textureId );
    }

}

