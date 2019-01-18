#include "TextureInfo.h"

namespace engine {
    TextureInfo::TextureInfo() {
    }

    TextureInfo::TextureInfo(const std::string & texName, const std::string & uniform, const unsigned int u, const unsigned int index) {
        this->unit = u;
        this->index = index;
        Catalog<Texture*>* textureCatalog = Catalog<Texture*>::instance();
        this->texture = textureCatalog->get(texName);
        this->uniform = uniform;
    }

    void TextureInfo::Activate(ShaderProgram* sp) {
        glActiveTexture(unit);
        glBindTexture( texture->getType(), texture->getId() );
        glUniform1i(sp->UniformId(uniform.c_str()), index);
    }
    void TextureInfo::ActivateForBuffer( ){
        glActiveTexture( unit );
        glBindTexture( texture->getType(), texture->getId() );
    }

    /*
    1.disable table
    2.alter camera position
    3.draw the scene except last quad
    4.activate the framebuffer
    5.draw the quad with a specific shader
    6.enable the table
    7.return camera to original position
    8.draw the scene in its entirety
    */
}