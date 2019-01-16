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
            std::cout << filename << " Width " << width << " height " << height << std::endl;
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
        std::cout << "Width " << width << " Height" << height << std::endl;
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

    GLuint Texture::getType(){
        return GL_TEXTURE_2D;
    }

    TextureCube::TextureCube(){
    }

    TextureCube::TextureCube( const char* filename, const char* filetype, bool bigRGB, bool fixed ){
        glGenTextures( 1, &textureId );
        glBindTexture( GL_TEXTURE_CUBE_MAP, textureId );
        std::cout << "CUBE TEXTURE " << textureId;
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
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
        std::vector<std::string> sides_one = {
            "right","left","top","bottom","front","back"
        };
        std::vector<std::string> sides_two = {
            "posx","negx","posy","negy","posz","negz"
        };
        if( !fixed ){
            for( int i = 0; i < 6; i++ ){
                textures_faces.push_back( std::string( filename ) );
                textures_faces[i].append( sides_one[i] );
                textures_faces[i].append( filetype );
            }
        } else if( fixed ){
            for( int i = 0; i < 6; i++ ){
                textures_faces.push_back( std::string( filename ) );
                textures_faces[i].append( sides_two[i] );
                textures_faces[i].append( filetype );
            }
        }
        unsigned char *data = nullptr;
        for( GLuint i = 0; i < textures_faces.size(); i++ ){
            data = stbi_load( textures_faces[i].c_str(), &width, &height, &nrChannels, 0 );
            std::cout << " MAP " << GL_TEXTURE_CUBE_MAP_POSITIVE_X + i << " name: " << textures_faces[i].c_str() << std::endl;
            if( data ){
                if( bigRGB ){
                    glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
                        width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
                }
                if( !bigRGB ){
                    glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
                        width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
                }
            } else{
                std::cout << "Failed to load cubemap texture" << std::endl;
                exit( -10 );
            }
        }
        stbi_image_free( data );
    }
    TextureCube::TextureCube( const char* filename, const char* filetype, bool bigRGB ){
        glGenTextures( 1, &textureId );
        glBindTexture( GL_TEXTURE_CUBE_MAP, textureId );
        std::cout << "CUBE TEXTURE " << textureId;
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
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
        std::vector<std::string> sides_one = {
            "right","left","top","bottom","front","back"
        };
        for( int i = 0; i < 6; i++ ){
            textures_faces.push_back( std::string( filename ) );
            textures_faces[i].append( sides_one[i] );
            textures_faces[i].append( filetype );
        }
        std::string temp = std::string( filetype );
        unsigned char *data = nullptr;
        for( GLuint i = 0; i < textures_faces.size(); i++ ){
            data = stbi_load( textures_faces[i].c_str(), &width, &height, &nrChannels, 0 );
            std::cout << " MAP " << GL_TEXTURE_CUBE_MAP_POSITIVE_X + i << " name: " << textures_faces[i].c_str() << std::endl;
            if( data ){
                if( temp == std::string( ".png" ) ){
                    glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
                        width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
                }
                if( temp == std::string( ".jpg" ) ){
                    glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
                        width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
                }
            } else{
                std::cout << "Failed to load cubemap texture" << std::endl;
                exit( -10 );
            }
        }
        stbi_image_free( data );
    }
    GLuint TextureCube::getType(){
        return GL_TEXTURE_CUBE_MAP;
    }

    RenderTexture::RenderTexture() = default;

    RenderTexture::RenderTexture(int width, int height){
        glGenTextures( 1, &textureId );
        glBindTexture( GL_TEXTURE_2D, textureId );
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        //generate the texture
        unit++;
        thisUnit = int( unit );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
        glGenerateMipmap( GL_TEXTURE_2D );
    }

    GLuint RenderTexture::getType(){
        return GL_TEXTURE_2D;
    }
    
}