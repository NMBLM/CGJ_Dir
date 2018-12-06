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

#define POSITION 5
#define VELOCITY 6
#define LIFEB 7

const int MaxParticles = 10000;

static const GLfloat Vertices[] = { 0.0f, 0.0f, 0.0f };
/*{
-POINT, -POINT, 0.0f,
POINT, -POINT, 0.0f,
-POINT, POINT, 0.0f,
POINT, POINT, 0.0f,
};*/

namespace engine{

    struct Particle;

    struct Particle{
        vec3 Position;
        vec3 Velocity;
        float Life;
        //vec4 Color;
        //float distance;

        Particle()
            : Position( vec3( 0.0f ) ), Velocity( vec3( 0.01f, 0, 0 ) ), Life( -1.0f ){
        }
        Particle( vec3 pos )
            : Position( pos ), Velocity( vec3( 0.01f, 0, 0 ) ), Life( -1.0f ){
        }

        friend std::ostream& operator<<( std::ostream& out, const Particle &p ){
            out << "Position " << p.Position << std::endl;
            out << "Velocity " << p.Velocity << std::endl;
            //out << "Color " << p.Color << std::endl;
            out << "Life " << p.Life << std::endl;
            return out;
        }

        friend bool operator<( const Particle& p, const Particle& p2 ){
            return p.Position.z > p2.Position.z;
        }
        Particle operator=( const Particle& p ){
            Position = p.Position;
            Velocity = p.Velocity;
            Life = p.Life;
            return *this;
        }

    };
}
using namespace engine;


class ParticleSystem{
    public:
    Particle Particles[MaxParticles];
    vec3 position;
    unsigned int lastUsedParticle = 0;

    bool m_isFirst;
    unsigned int m_currVB = 0;
    unsigned int m_currTFB = 0;
    GLuint m_particleBuffer[2];
    GLuint m_transformFeedback[2];

    ShaderProgram* shaderDraw;
    ShaderProgram* shaderUpdate;
    Camera* camera;

    GLuint VaoId;
    float delta = 0;

    ParticleSystem( ShaderProgram* shaderProgram, Camera* camera );
    ParticleSystem( ShaderProgram* shaderProgram, Camera* camera, vec3 pos );
    ParticleSystem( ShaderProgram* update, ShaderProgram* draw, Camera* camera, vec3 pos );

    void createBufferObjects();

    //void destroyBufferObjects();


    int FirstUnusedParticle();
    void RespawnParticle( Particle& particle );

    void SortParticles(){
        std::sort( &Particles[0], &Particles[MaxParticles] );
    }

    void update( float delta );
    void draw();
};




#endif