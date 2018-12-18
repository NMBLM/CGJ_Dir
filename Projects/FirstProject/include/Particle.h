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

//#define GRAVITY vec3(0.0f,-9.81f,0.0f)
#define GRAVITY vec3(0.0f,0.0f,0.0f)

//#define VELOCITY vec3(1.0f,2.0f,1.0f)
#define VELOCITY vec3(0.1f,2.0f,0.1f)

#define POINT 0.01f
#define LIFE 0.4f
#define TIMER 0.01f

#define APOSITION 0
#define AVELOCITY 1
#define ALIFE 2
#define ATYPE 3
#define PARTICLE_LAUNCHER 0.0f

const int MaxParticles = 1000000;
const int nr_new_particles = 20;
static const GLfloat Vertices[] = { 0.0f, 0.0f, 0.0f };

#define ZERO_MEM(a) memset(a, 0, sizeof(a))

namespace engine{

    struct Particle;

    struct Particle{

        vec3 Position;
        vec3 Velocity;
        float Life;
        float Type;

        Particle()
            : Position( vec3( 0.0f ) ), Velocity( vec3( 0.01f, 0, 0 ) ), Life( -1.0f ){
        }
        Particle( vec3 pos )
            : Position( pos ), Velocity( vec3( 0.01f, 0, 0 ) ), Life( -1.0f ){
        }

        friend std::ostream& operator<<( std::ostream& out, const Particle &p ){
            out << "Position " << p.Position << std::endl;
            out << "Velocity " << p.Velocity << std::endl;
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

    void update( float delta );
    void draw();
};

class ParticleSystemTransform{
    public:

    Particle Particles[MaxParticles];
    vec3 position;
    Camera* camera;
    ShaderProgram*  m_billboardTechnique;
    ShaderProgram* m_updateTechnique;
    unsigned int m_currVB;
    unsigned int m_currTFB;
    bool m_isFirst;
    GLuint m_particleBuffer[2];
    GLuint m_transformFeedback[2];
    GLuint VaoId;
    float delta = 0;

    ParticleSystemTransform( ShaderProgram* draw, ShaderProgram* update, Camera* cam, vec3 pos );

    void InitParticleSystem();
    void UpdateParticles();
    void RenderParticles();

    void update( float _delta );
    void draw();
};



#endif
