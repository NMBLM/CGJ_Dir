#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector.h"
#include "Matrix.h"
#include "Qtrn.h"
#include "Camera.h"
#include "ShaderProgram.h"

#include <vector>
#include "GL/glew.h"

#include <algorithm>

#define GRAVITY vec3(0.0f,-9.81f,0.0f)
#define POINT 0.01f
#define LIFE 1.0f

#define VERTEX 10
#define POSITION 11
#define COLOR 12

const int MaxParticles = 10000;

static const GLfloat Vertices[] = { -POINT, -POINT, 0.0f,
                                    POINT, -POINT, 0.0f,
                                    -POINT, POINT, 0.0f,
                                    POINT, POINT, 0.0f, };
   
namespace engine{

    struct Particle;

    struct Particle{
        vec3 Position, Velocity;
        vec4 Color;
        float Life;
        float distance;

        Particle()
            : Position( vec3( 0.0f ) ), Velocity( vec3( 0.01f, 0, 0 ) ), Color( vec4( 0.0f ) ), Life( -1.0f ), distance( 0.0f ){
        }
        Particle( vec3 pos )
            : Position( pos ), Velocity( vec3( 0.01f, 0, 0 ) ), Color( vec4( 0.0f ) ), Life( -1.0f ), distance( 0.0f ){
        }
        bool operator<( Particle& p ){
            // Sort in reverse order : far particles drawn first.
            return this->distance > p.distance;
        }

        friend std::ostream& operator<<( std::ostream& out, const Particle &p ){
            out << "Position " << p.Position << std::endl;
            out << "Velocity " << p.Velocity << std::endl;
            out << "Color " << p.Color << std::endl;
            out << "Life " << p.Life << std::endl;
            return out;
        }

    };
}
using namespace engine;


class ParticleSystem{
    public:
    std::vector<Particle> particles;
    vec3 position;
    unsigned int lastUsedParticle = 0;
    GLuint VaoId;
    ShaderProgram* shader;
    Camera* camera;

    float delta = 0;

    ParticleSystem( ShaderProgram* shaderProgram, Camera* camera );
    ParticleSystem( ShaderProgram* shaderProgram, Camera* camera, vec3 pos );

    void createBufferObjects();

    //void destroyBufferObjects();


    int FirstUnusedParticle();
    void RespawnParticle( Particle& particle );

    void SortParticles(){
        std::sort( &particles[0], &particles[MaxParticles] );
    }

    void update( float delta );
    void draw();
};




#endif