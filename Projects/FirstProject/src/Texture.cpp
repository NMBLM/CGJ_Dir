#include "Texture.h"
namespace engine{
    unsigned int Texture::unit = GL_TEXTURE0;

    Texture::Texture(){
        unit++;
        std::cout << "empty" << std::endl;
    }

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
            unit++;
            thisUnit = int( unit );
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
            glGenerateMipmap( GL_TEXTURE_2D );
        } else{
            std::cout << "Failed to load texture in " << filename << std::endl;
            LoadDefault();
        }
        stbi_image_free( data );
    }

    void Texture::LoadDefault(){
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
    unsigned int Texture::getId(){
        return textureId;
    }

    TextureCube::TextureCube(){

    }

    TextureCube::TextureCube( const char* filename, const char* filetype ){
        glGenTextures( 1, &textureId );
        glBindTexture( GL_TEXTURE_CUBE_MAP, textureId );
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        //Generate textures faces name
        /*
                     ___________
                    |           |
                    |           |
                    |    top    |
                    |           |
         ___________|___________|___________ ___________
        |           |           |           |           |
        |           |           |           |           |
        |   left    |   front   |   right   |   back    |
        |           |           |           |           |
        |___________|___________|___________|___________|
                    |           |
                    |           |
                    |   bottom  |
                    |           |
                    |___________|

        */
        std::vector<std::string> sides = {
            "right","left","top","bottom","front","back"
        };
        for( int i = 0; i < 6; i++ ){
            textures_faces.push_back( std::string( filename ) );
            textures_faces[i].append( sides[i] );
            textures_faces[i].append( filetype );
        }
        //int width, height, nrChannels;
        unsigned char *data = nullptr;
        for( GLuint i = 0; i < textures_faces.size(); i++ ){
            data = stbi_load( textures_faces[i].c_str(), &width, &height, &nrChannels, 0 );
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
        }
        stbi_image_free( data );
    }

}