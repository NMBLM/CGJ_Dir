#pragma once
#ifndef ANIME_H
#define ANIME_H
#include <vector>
#include "vector.h"
#include "matrix.h"
#include "quaternion.h"
using namespace engine;
class Animation{
    qtrn rStart, rEnd;
    vec4 pStart, pEnd;
    public:
    Animation( qtrn rs, vec4 ps, qtrn re, vec4 pe );
    mat4 animate( mat4 model, float delta );
};

class Animator{
    std::vector<Animation*> sequence;
    int index = 0;
    float valueK = 0.0f;
    int refwrd = 0;
    public:
    mat4 lastMatrix = MatrixFactory::createIdentityMatrix4();

    Animator();
    void addAnimation( Animation* a );
    mat4 calcAnimation( mat4 model );
    void update( float deltatime );
    void activate();
};
#endif