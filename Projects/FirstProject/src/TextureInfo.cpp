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
        glUniform1i(sp->UniformId(uniform.c_str()), index);
        glBindTexture(GL_TEXTURE_2D, texture->getId());//TODO permitir diferentes tipos de textures
    }
}