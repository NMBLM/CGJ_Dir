#include "Particle.h"

using namespace engine;

ParticleSystem::ParticleSystem( ShaderProgram* shaderProgram, Camera* cam ){
    shaderDraw = shaderProgram;
    camera = cam;
    position = vec3( 0.1f );
    for( GLuint i = 0; i < MaxParticles; ++i )
        Particles[i] = Particle( position );
    createBufferObjects();
}
ParticleSystem::ParticleSystem( ShaderProgram* shaderProgram, Camera* cam, vec3 pos ){
    shaderDraw = shaderProgram;
    camera = cam;
    position = pos;
    for( GLuint i = 0; i < MaxParticles; ++i )
        Particles[i] = Particle( position );
    createBufferObjects();
}

ParticleSystem::ParticleSystem( ShaderProgram* draw, ShaderProgram* update, Camera* cam, vec3 pos ){
    shaderUpdate = update;
    shaderDraw = draw;
    camera = cam;
    position = pos;
    Particles[0].Position = pos;
    Particles[0].Velocity = vec3( 0.1f, 5.0f, 0.1f );
    Particles[0].Life = 1.0f;

    m_currVB = 0;
    m_currTFB = 1;
    m_isFirst = true;
    createBufferObjects();
}

void ParticleSystem::createBufferObjects(){
    /**/
    glGenTransformFeedbacks( 2, m_transformFeedback );
    glGenBuffers( 2, m_particleBuffer );

    for( unsigned int i = 0; i < 2; i++ ){
        glBindTransformFeedback( GL_TRANSFORM_FEEDBACK, m_transformFeedback[i] );
        glBindBuffer( GL_ARRAY_BUFFER, m_particleBuffer[i] );
        glBufferData( GL_ARRAY_BUFFER, sizeof( Particles ), Particles, GL_DYNAMIC_DRAW );
        glBindBufferBase( GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_particleBuffer[i] );
    }

    /**/
    const GLchar* Varyings[3];
    Varyings[0] = "Position1";
    Varyings[1] = "Velocity1";
    Varyings[2] = "Life1";

    glTransformFeedbackVaryings( shaderUpdate->id, 3, Varyings, GL_INTERLEAVED_ATTRIBS );

    /** /
    GLuint VboVertices;
    glGenVertexArrays( 1, &VaoId );
    glBindVertexArray( VaoId );
    {
        /** /
        glGenBuffers( 1, &VboVertices );
        glBindBuffer( GL_ARRAY_BUFFER, VboVertices );
        glBufferData( GL_ARRAY_BUFFER, 12 * sizeof( GLfloat ), &Vertices[0], GL_STATIC_DRAW );
        glEnableVertexAttribArray( 0 );
        glEnableVertexAttribArray( 1 );
        glEnableVertexAttribArray( 2 );
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( Particle ), ( const GLvoid* )0 );
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( Particle ), ( const GLvoid* )12 );
        glVertexAttribPointer( 0, 1, GL_FLOAT, GL_FALSE, sizeof( Particle ), ( const GLvoid* )24 );
        /** /
    }
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glDeleteBuffers( 1, &VboVertices );
    /**/
}

void ParticleSystem::update( float _delta ){
    delta = _delta / 100;
}


void ParticleSystem::draw(){
    camera->setMatrix();

    shaderUpdate->use();
    shaderUpdate->addUniformFloat( "delta", delta );
    glEnable( GL_RASTERIZER_DISCARD );

    glBindBuffer( GL_ARRAY_BUFFER, m_particleBuffer[m_currVB] );
    glBindTransformFeedback( GL_TRANSFORM_FEEDBACK, m_transformFeedback[m_currTFB] );
    GLuint vaoId = 0;
    glGenVertexArrays( 1, &vaoId );
    glBindVertexArray( vaoId );
    {
        glEnableVertexAttribArray( POSITION );
        glEnableVertexAttribArray( VELOCITY );
        glEnableVertexAttribArray( LIFEB );

        glVertexAttribPointer( POSITION, 3, GL_FLOAT, GL_FALSE, sizeof( Particle ), ( const GLvoid* )0 );         // position
        glVertexAttribPointer( VELOCITY, 3, GL_FLOAT, GL_FALSE, sizeof( Particle ), ( const GLvoid* )12 );        // velocity
        glVertexAttribPointer( LIFEB, 1, GL_FLOAT, GL_FALSE, sizeof( Particle ), ( const GLvoid* )24 );          // lifetime

        glBeginTransformFeedback( GL_POINTS );

        //Initialize for the first time
        if( m_isFirst ){
            glDrawArrays( GL_POINTS, 0, 1 );

            m_isFirst = false;
        } else{
            glDrawTransformFeedback( GL_POINTS, m_transformFeedback[m_currVB] );
        }

        glEndTransformFeedback();

        glDisableVertexAttribArray( POSITION );
        glDisableVertexAttribArray( VELOCITY );
        glDisableVertexAttribArray( LIFEB );
    }
    glBindVertexArray( 0 );

    shaderDraw->use();

    //glEnable( GL_BLEND );
    //glBlendFunc( GL_SRC_ALPHA, GL_ONE );

    glDisable( GL_RASTERIZER_DISCARD );

    glBindBuffer( GL_ARRAY_BUFFER, m_particleBuffer[m_currTFB] );
    vaoId = 0;
    glGenVertexArrays( 1, &vaoId );
    glBindVertexArray( vaoId );
    {
        glEnableVertexAttribArray( POSITION );

        glVertexAttribPointer( POSITION, 3, GL_FLOAT, GL_FALSE, sizeof( Particle ), ( const GLvoid* )0 );  // position

        glDrawTransformFeedback( GL_POINTS, m_transformFeedback[m_currTFB] );

        glDisableVertexAttribArray( POSITION );
    }
    glBindVertexArray( 0 );

    //glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    //glDisable( GL_BLEND );

    shaderDraw->stop();
    m_currVB = m_currTFB;
    m_currTFB = ( m_currTFB + 1 ) & 0x1;
}

int ParticleSystem::FirstUnusedParticle(){

    // Search from last used particle, this will usually return almost instantly
    for( unsigned int i = lastUsedParticle; i < MaxParticles; ++i ){
        if( Particles[i].Life <= 0.0f ){
            lastUsedParticle = i;
            return i;
        }
    }
    // Otherwise, do a linear search
    for( unsigned int i = 0; i < MaxParticles; ++i ){
        if( Particles[i].Life <= 0.0f ){
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
    particle.Life = LIFE;
    particle.Velocity = vec3( 0.1f + rnd1, 5.0f + rnd2, 0.1f + rnd3 );
    //particle.distance = (particle.Position - vec3(0.0f,0.0f,5.0f)).length();
    //std::cout << "random " << random << std::endl;
    //std::cout << "Color " << rColor << std::endl;
    //std::cout << "Particle " << std::endl << particle ;

}


