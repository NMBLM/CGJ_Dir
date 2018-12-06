#include "Scene.h"

using namespace engine;


engine::Scene::Scene( ShaderProgram* shaders, Camera* cam ){
    root = new SceneNode( nullptr, shaders );
    camera = cam;
}

void engine::Scene::draw(){
    camera->setMatrix();
    root->draw( MatrixFactory::createIdentityMatrix4() );
}

void engine::Scene::addNode( SceneNode * node ){
    root->addNode( node );
}

void engine::Scene::updateModel( mat4 trs ){
    root->updateModel( trs );
}

void engine::Scene::actOnAnimator(){
    root->actOnAnimator();
}

void engine::Scene::update( float deltatime ){
    root->update( deltatime );
}

void engine::Scene::setCamera( Camera * cam ){
    camera = cam;
}



engine::SceneNode::SceneNode(){
    nodes = std::vector <SceneNode*>();
}

engine::SceneNode::SceneNode( Mesh * m, ShaderProgram * shaders, mat4 mat ){
    mesh = m;
    shaderProgram = shaders;
    model = mat;
    nodes = std::vector <SceneNode*>();
}

void engine::SceneNode::draw( mat4 fm ){
    mat4 m = fm * model;
    if( mesh != nullptr ){
        if( anime != nullptr ){
            m = fm * anime->calcAnimation( model );
        }
        shaderProgram->use();
        glUniformMatrix4fv( shaderProgram->UniformId( "ModelMatrix" ), 1, GL_FALSE, m.data() );
        if( !fcmp( color.x, -1 ) ){
            glUniform4fv( shaderProgram->UniformLocation( "forcedColor" ), 1, color.data() );
        }
        unsigned int i = 0;

        std::string t = std::string( "texture" );
        for( auto& texName : textures ){
            glUniform1i( shaderProgram->UniformId( ( t + std::to_string( i ) ).c_str() ), i );
            Catalog<Texture*>::instance()->get( texName )->activate( GL_TEXTURE0 + i );
            i++;
        }
        mesh->draw();
        shaderProgram->stop();
    }
    for( auto& s : nodes ){
        s->draw( m );
    }
}

void engine::SceneNode::updateModel( mat4 trs ){
    model = trs * model;
    if( anime != nullptr ){
        anime->lastMatrix = trs * anime->lastMatrix;
    }
}

bool engine::SceneNode::hasShaderProgram(){
    return shaderProgram != nullptr;
}

void engine::SceneNode::addNode( SceneNode * node ){
    if( !node->hasShaderProgram() ){
        node->shaderProgram = this->shaderProgram;
    }
    nodes.push_back( node );
}

void engine::SceneNode::setColor( vec4 c ){
    color.x = c.x;
    color.y = c.y;
    color.z = c.z;
    color.w = c.w;
}

void engine::SceneNode::update( float deltatime ){
    if( anime != nullptr ){
        anime->update( deltatime );
    }
    for( auto& sn : nodes ){
        sn->update( deltatime );
    }
}

void engine::SceneNode::addAnimator( Animator* a ){
    a->lastMatrix = model;
    anime = a;
}

void engine::SceneNode::addTexture( const std::string & texName ){
    textures.push_back( texName );
}

Animator* engine::SceneNode::getAnimator(){
    return anime;
}

void engine::SceneNode::actOnAnimator(){
    if( anime != nullptr ){
        anime->activate();
    }
    for( auto& s : nodes ){
        s->actOnAnimator();
    }
}

ShaderProgram * engine::SceneNode::getShaderProgram(){
    if( shaderProgram != nullptr ){
        return shaderProgram;
    }
    std::cerr << "No ShaderProgram found in SceneNode";
    exit( 1 );
}

