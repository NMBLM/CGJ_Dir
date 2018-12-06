#include "Particle.h"

using namespace engine;

ParticleSystem::ParticleSystem( ShaderProgram* shaderProgram, Camera* cam ){
    shader = shaderProgram;
    camera = cam;
    position = vec3( 0.1f );
    for( GLuint i = 0; i < MaxParticles; ++i )
        particles.push_back( Particle( position ) );
    createBufferObjects();
}
ParticleSystem::ParticleSystem( ShaderProgram* shaderProgram, Camera* cam, vec3 pos ){
    shader = shaderProgram;
    camera = cam;
    position = pos;
    for( GLuint i = 0; i < MaxParticles; ++i )
        particles.push_back( Particle( position ) );
    createBufferObjects();
}
void ParticleSystem::createBufferObjects(){
    GLuint VboVertices;

    glGenVertexArrays( 1, &VaoId );
    glBindVertexArray( VaoId );
    {
        glGenBuffers( 1, &VboVertices );
        glBindBuffer( GL_ARRAY_BUFFER, VboVertices );
        glBufferData( GL_ARRAY_BUFFER, 12 * sizeof( GLfloat ), &Vertices[0], GL_STATIC_DRAW );
        glEnableVertexAttribArray( 0 );
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ), 0 );
    }
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glDeleteBuffers( 1, &VboVertices );

}

void ParticleSystem::update( float _delta ){
    float delta = _delta / 100.0f;
    GLuint nr_new_particles = 20;
    // Add new particles
    for( GLuint i = 0; i < nr_new_particles; ++i ){
        int unusedParticle = FirstUnusedParticle();
        RespawnParticle( particles[unusedParticle] );
    }
    // Uupdate all particles
    for( GLuint i = 0; i < MaxParticles; ++i ){
        Particle &p = particles[i];
        p.Life -= delta; // reduce life
        if( p.Life > 0.0f ){	// particle is alive, thus update
            p.Velocity += GRAVITY * delta;
            p.Position = p.Position + ( p.Velocity * delta );
            p.Color.w -= delta * 1.5f;
        }
    }
}

void ParticleSystem::draw(){
    //glEnable( GL_BLEND );
    //glBlendFunc( GL_SRC_ALPHA, GL_ONE );
    camera->setMatrix();
    shader->use();
    //float angle = 0;
    //vec3 axis = vec3( 0.0f, 0.0f, 1.0f );
    //camera->rotationQtrn().qToAngleAxis( angle, axis );
    //shader->addUniformVec( "rotation", ( ( -1 ) * axis ) );
    shader->addUniformMat( "rotation", camera->rotation()  );
    for( Particle particle : particles ){
        if( particle.Life > 0.0f ){
            shader->addUniformVec( "color", particle.Color );
            shader->addUniformVec( "position", particle.Position );
            glBindVertexArray( VaoId );
            //glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
            glDrawArrays( GL_POINTS, 0, 1 );
            glBindVertexArray( 0 );
        }
    }
    //glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    //glDisable( GL_BLEND );
    shader->stop();

}

int ParticleSystem::FirstUnusedParticle(){

    // Search from last used particle, this will usually return almost instantly
    for( unsigned int i = lastUsedParticle; i < MaxParticles; ++i ){
        if( particles[i].Life <= 0.0f ){
            lastUsedParticle = i;
            return i;
        }
    }
    // Otherwise, do a linear search
    for( unsigned int i = 0; i < MaxParticles; ++i ){
        if( particles[i].Life <= 0.0f ){
            lastUsedParticle = i;
            return i;
        }
    }
    // Override first particle if all others are alive
    lastUsedParticle = 0;
    return 0;
}

void ParticleSystem::RespawnParticle( Particle &particle ){
    float rnd1 = ( ( rand() % 100 ) - 50 ) / 100.0f;
    float rnd2 = ( ( rand() % 100 ) - 50 ) / 100.0f;
    float rnd3 = ( ( rand() % 100 ) - 50 ) / 100.0f;
    float rColor = ( ( rand() % 100 ) / 100.0f );
    float gColor = ( ( rand() % 100 ) / 100.0f );
    float bColor = ( ( rand() % 100 ) / 100.0f );

    particle.Position = position + vec3( rnd1, 0.0f, ( rnd1 - rnd2 + rnd3 ) );
    particle.Color.x = rColor;
    particle.Color.y = gColor;
    particle.Color.z = bColor;
    particle.Color.w = 1.0f;
    particle.Life = LIFE;
    particle.Velocity = vec3( 0.1f + rnd1, 5.0f + rnd2, 0.1f + rnd3 );
    //particle.distance = (particle.Position - vec3(0.0f,0.0f,5.0f)).length();
    //std::cout << "random " << random << std::endl;
    //std::cout << "Color " << rColor << std::endl;
    //std::cout << "Particle " << std::endl << particle ;

}


