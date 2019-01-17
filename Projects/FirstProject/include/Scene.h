#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "vector.h"
#include "Mesh.h"
#include "Camera.h"
#include "Matrix.h"
#include "Anime.h"
#include "ShaderProgram.h"
#include "TextureInfo.h"
#include "Catalog.h"

#include "GL/glew.h"
#define SCENE_NODE_MANAGER Catalog<SceneNode*>::instance()

namespace engine{

    class SceneNode {
        Mesh* mesh = nullptr;
        qtrn qDir = qtrn();
        mat4 model = MatrixFactory::createIdentityMatrix4();
        std::vector <SceneNode*> nodes;
        vec4 color = vec4(-1, 0, 0, 1);
        Animator* anime = nullptr;
        bool enabled;
        std::vector<TextureInfo*> textures;

    public:

        static constexpr const char* const NOODLES = "NOODLES";
        static constexpr const char* const TABLE = "TABLE";
        static constexpr const char* const ROOT = "ROOT";
        static constexpr const char* const SKYBOX = "SKYBOX";
        static constexpr const char* const LIGHTS = "LIGHTS";


        ShaderProgram* shaderProgram = nullptr;
        SceneNode();
        SceneNode(Mesh* m = nullptr, ShaderProgram* shaders= nullptr, mat4 model = MatrixFactory::createIdentityMatrix4());
        void draw( mat4 fm);
        void updateModel(mat4 trs);
        bool hasShaderProgram();
        ShaderProgram * getShaderProgram();
        void addNode(SceneNode* node);
        void setColor(vec4 c);

        void update(float deltatime);

        void addAnimator(Animator* a);
        void addTexture(TextureInfo* t);
        Animator* getAnimator();
        void actOnAnimator();

        void enable();
        void disable();
        bool isEnabled();
    };


    class Scene {
        SceneNode* root = nullptr;
        ShaderProgram* dfault;
        Camera * camera = nullptr;

        public:
        Scene( ShaderProgram* shaders, Camera* cam );
        void draw();
        void drawReflection(vec4 reflectionPlane);
        void addNode( SceneNode* node );
        void updateModel( mat4 trs );
        void actOnAnimator();
        void update( float deltatime );
        void setCamera( Camera* cam );
        void activateReflection(vec4 rp);
        void activateReflection(vec4* vec4);
        void deactivateReflection();
    };
}

#endif
