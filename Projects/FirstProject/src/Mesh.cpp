#include "Mesh.h"
using namespace engine;


Mesh::Mesh(){

}

Mesh::Mesh( std::vector<vec3> vertices,
            std::vector<vec2> texcoords,
            std::vector<vec3> normals,
            bool tcLoaded, bool nLoaded ){

    this->Vertices = std::vector<vec3>( vertices );
    this->Texcoords = std::vector<vec2>( texcoords );
    this->Normals = std::vector<vec3>( normals );
    this->TexcoordsLoaded = tcLoaded;
    this->NormalsLoaded = nLoaded;
    calculateTangentSpace();
    createBufferObjects();
}


void engine::Mesh::calculateTangentSpace() {
    //TODO calculate tangents
}

/////////////////////////////////////////////////////////////////////// VAOs & VBOs

void Mesh::createBufferObjects(){
    GLuint VboVertices, VboTexcoords, VboNormals;

    glGenVertexArrays( 1, &VaoId );
    glBindVertexArray( VaoId );
    {
        glGenBuffers( 1, &VboVertices );
        glBindBuffer( GL_ARRAY_BUFFER, VboVertices );
        glBufferData( GL_ARRAY_BUFFER, Vertices.size() * sizeof( vec3 ), &Vertices[0], GL_STATIC_DRAW );
        glEnableVertexAttribArray( VERTICES );
        glVertexAttribPointer( VERTICES, 3, GL_FLOAT, GL_FALSE, sizeof( vec3 ), 0 );

        if( TexcoordsLoaded ){
            glGenBuffers( 1, &VboTexcoords );
            glBindBuffer( GL_ARRAY_BUFFER, VboTexcoords );
            glBufferData( GL_ARRAY_BUFFER, Texcoords.size() * sizeof( vec2 ), &Texcoords[0], GL_STATIC_DRAW );
            glEnableVertexAttribArray( TEXCOORDS );
            glVertexAttribPointer( TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof( vec2 ), 0 );
        }
        if( NormalsLoaded ){
            glGenBuffers( 1, &VboNormals );
            glBindBuffer( GL_ARRAY_BUFFER, VboNormals );
            glBufferData( GL_ARRAY_BUFFER, Normals.size() * sizeof( vec3 ), &Normals[0], GL_STATIC_DRAW );
            glEnableVertexAttribArray( NORMALS );
            glVertexAttribPointer( NORMALS, 3, GL_FLOAT, GL_FALSE, sizeof( vec3 ), 0 );
        }
    }
    //TODO adde tangents do buffer
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glDeleteBuffers( 1, &VboVertices );
    glDeleteBuffers( 1, &VboTexcoords );
    glDeleteBuffers( 1, &VboNormals );
}

void Mesh::destroyBufferObjects(){
    glBindVertexArray(VaoId);
    glDisableVertexAttribArray(VERTICES);
    glDisableVertexAttribArray(TEXCOORDS);
    glDisableVertexAttribArray(NORMALS);
    glDeleteVertexArrays(1, &VaoId);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void engine::Mesh::draw(){
    glBindVertexArray( VaoId);
    glDrawArrays(GL_TRIANGLES, 0, ( GLsizei )Vertices.size());
    glBindVertexArray( 0 );
}

