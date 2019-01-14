#include "Particle.h"

using namespace engine;
//bool m_isFirst;
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
        }
    }
}

void ParticleSystem::draw(){
    //glEnable( GL_BLEND );
    //glBlendFunc( GL_SRC_ALPHA, GL_ONE );

    camera->setMatrix();
    shader->use();
    for( Particle particle : particles ){
        if( particle.Life > 0.0f ){
            shader->addUniform( "position", particle.Position );
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
    particle.Position = position + vec3( ( rnd1 - rnd2 + rnd3 ), 0.0f, rnd1 ) * ( 1 / 2.0f );
    particle.Life = LIFE;
    particle.Velocity = VELOCITY + vec3( 0.1f*rnd1 - 0.1f, rnd2, 0.1f*rnd3 - 0.1f );
    //std::cout << "Particle " << std::endl << particle ;

}

ParticleSystemTransform::ParticleSystemTransform( ShaderProgram * draw, ShaderProgram * update, Camera * cam, vec3 pos ){
    m_billboardTechnique = draw;
    m_updateTechnique = update;


    camera = cam;
    position = pos;

    m_currVB = 0;
    m_currTFB = 1;
    m_isFirst = true;


    ZERO_MEM( m_particleBuffer );
    ZERO_MEM( m_transformFeedback );


}
void ParticleSystemTransform::InitParticleSystem(){
    Particles[0].Position = position;
    Particles[0].Velocity = VELOCITY;//vec3( 0 );
    Particles[0].Life = TIMER;
    Particles[0].Type = PARTICLE_LAUNCHER;
    glGenVertexArrays( 1, &VaoId );
    glBindVertexArray( VaoId );
    {
        glGenTransformFeedbacks( 2, m_transformFeedback );
        glGenBuffers( 2, m_particleBuffer );

        for( unsigned int i = 0; i < 2; i++ ){
            glBindTransformFeedback( GL_TRANSFORM_FEEDBACK, m_transformFeedback[i] );
            glBindBuffer( GL_ARRAY_BUFFER, m_particleBuffer[i] );
            glBufferData( GL_ARRAY_BUFFER, sizeof( Particles ), &Particles[0], GL_DYNAMIC_DRAW );
            glBindBufferBase( GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_particleBuffer[i] );
        }
    }
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );

}



void ParticleSystemTransform::update( float _delta ){
    delta = _delta / 100;
}

void ParticleSystemTransform::draw(){

    camera->setMatrix();

    UpdateParticles();

    RenderParticles();

    m_currVB = m_currTFB;
    m_currTFB = ( m_currTFB + 1 ) & 0x1;
}


void ParticleSystemTransform::UpdateParticles(){
    // Three different values between 0 and 1;
    float rnd1 = ( rand() % 100 ) / 100.0f;
    float rnd2 = ( rand() % 100 ) / 100.0f;
    float rnd3 = ( rand() % 100 ) / 100.0f;
    m_updateTechnique->use();
    m_updateTechnique->addUniform( "delta", delta );
    m_updateTechnique->addUniform( "position", position );

    m_updateTechnique->addUniform( "rnd1", rnd1 );
    m_updateTechnique->addUniform( "rnd2", rnd2 );
    m_updateTechnique->addUniform( "rnd3", rnd3 );


    glEnable( GL_RASTERIZER_DISCARD );

    glBindVertexArray( VaoId );
    {

        glBindBuffer( GL_ARRAY_BUFFER, m_particleBuffer[m_currVB] );
        glBindTransformFeedback( GL_TRANSFORM_FEEDBACK, m_transformFeedback[m_currTFB] );

        glEnableVertexAttribArray( APOSITION );
        glEnableVertexAttribArray( AVELOCITY );
        glEnableVertexAttribArray( ALIFE );
        glEnableVertexAttribArray( ATYPE );

        glVertexAttribPointer( APOSITION, 3, GL_FLOAT, GL_FALSE, sizeof( Particle ), ( const GLvoid* )0 );         // Position
        glVertexAttribPointer( AVELOCITY, 3, GL_FLOAT, GL_FALSE, sizeof( Particle ), ( const GLvoid* )12 );        // Velocity
        glVertexAttribPointer( ALIFE, 1, GL_FLOAT, GL_FALSE, sizeof( Particle ), ( const GLvoid* )24 );        // Life
        glVertexAttribPointer( ATYPE, 1, GL_FLOAT, GL_FALSE, sizeof( Particle ), ( const GLvoid* )28 );        // Type

        glBeginTransformFeedback( GL_POINTS );

        if( m_isFirst ){
            m_isFirst = false;
            glDrawArrays( GL_POINTS, 0, 1 );
        } else{
            glDrawTransformFeedback( GL_POINTS, m_transformFeedback[m_currVB] );
        }

        glEndTransformFeedback();

        glDisableVertexAttribArray( APOSITION );
        glDisableVertexAttribArray( AVELOCITY );
        glDisableVertexAttribArray( ALIFE );
        glDisableVertexAttribArray( ATYPE );

    }
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );


    m_updateTechnique->stop();
}


void ParticleSystemTransform::RenderParticles(){
    float rnd1 = ( ( rand() % 100 ) - 50 ) / 100.0f;
    float rnd2 = ( ( rand() % 100 ) - 50 ) / 100.0f;
    float rnd3 = ( ( rand() % 100 ) - 50 ) / 100.0f;

    m_billboardTechnique->use();
    m_billboardTechnique->addUniform( "delta", delta );
    m_billboardTechnique->addUniform( "rnd1", rnd1 );
    m_billboardTechnique->addUniform( "rnd2", rnd2 );
    m_billboardTechnique->addUniform( "rnd3", rnd3 );
    glEnable( GL_BLEND );
    glDepthMask( GL_FALSE );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glDisable( GL_RASTERIZER_DISCARD );
    glBindVertexArray( VaoId );
    {

        glBindBuffer( GL_ARRAY_BUFFER, m_particleBuffer[m_currTFB] );

        glEnableVertexAttribArray( 0 );

        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( Particle ), ( const GLvoid* )0 );  // position

        glDrawTransformFeedback( GL_POINTS, m_transformFeedback[m_currTFB] );

        glDisableVertexAttribArray( 0 );

    }

    glDepthMask( GL_TRUE );
    glDisable( GL_BLEND );

    m_billboardTechnique->stop();
}
