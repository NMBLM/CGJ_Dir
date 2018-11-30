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
    createBufferObjects();
}

Mesh::Mesh( std::string  filename ){

    createMesh( filename );
}
void Mesh::parseVertex( std::stringstream & sin ){
    vec3 v;
    sin >> v;
    vertexData.push_back( v );
}

void Mesh::parseTexcoord( std::stringstream & sin ){
    vec2 t;
    sin >> t;
    texcoordData.push_back( t );
}

void Mesh::parseNormal( std::stringstream & sin ){
    vec3 n;
    sin >> n;
    normalData.push_back( n );
}

void Mesh::parseFace( std::stringstream & sin ){
    std::string token;
    for( int i = 0; i < 3; i++ ){
        std::getline( sin, token, '/' );
        if( token.size() > 0 ) vertexIdx.push_back( std::stoi( token ) );
        std::getline( sin, token, '/' );
        if( token.size() > 0 ) texcoordIdx.push_back( std::stoi( token ) );
        std::getline( sin, token, ' ' );
        if( token.size() > 0 ) normalIdx.push_back( std::stoi( token ) );
    }
}

void Mesh::parseLine( std::stringstream & sin ){
    std::string s;
    sin >> s;
    if( s.compare( "v" ) == 0 ) parseVertex( sin );
    else if( s.compare( "vt" ) == 0 ) parseTexcoord( sin );
    else if( s.compare( "vn" ) == 0 ) parseNormal( sin );
    else if( s.compare( "f" ) == 0 ) parseFace( sin );
}

void Mesh::loadMeshData( std::string& filename ){
    std::ifstream ifile( filename );
    std::string line;
    std::stringstream ss;
    while( std::getline( ifile, line ) ){
        ss = std::stringstream( line );
        parseLine( ss );
    }
    TexcoordsLoaded = ( texcoordIdx.size() > 0 );
    NormalsLoaded = ( normalIdx.size() > 0 );
}

void Mesh::processMeshData(){
    for( unsigned int i = 0; i < vertexIdx.size(); i++ ){
        unsigned int vi = vertexIdx[i];
        vec3 v = vertexData[vi - 1];
        Vertices.push_back( v );
        if( TexcoordsLoaded ){
            unsigned int ti = texcoordIdx[i];
            vec2 t = texcoordData[ti - 1];
            Texcoords.push_back( t );
        }
        if( NormalsLoaded ){
            unsigned int ni = normalIdx[i];
            vec3 n = normalData[ni - 1];
            Normals.push_back( n );
        }
    }
}

void Mesh::freeMeshData(){
    vertexData.clear();
    texcoordData.clear();
    normalData.clear();
    vertexIdx.clear();
    texcoordIdx.clear();
    normalIdx.clear();
}

void Mesh::createMesh( std::string & filename ){
    loadMeshData( filename );
    processMeshData();
    freeMeshData();
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
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glDeleteBuffers( 1, &VboVertices );
    glDeleteBuffers( 1, &VboTexcoords );
    glDeleteBuffers( 1, &VboNormals );
}


void Mesh::destroyBufferObjects(){
    //glBindVertexArray(VaoId);
    //glDisableVertexAttribArray(VERTICES);
    //glDisableVertexAttribArray(TEXCOORDS);
    //glDisableVertexAttribArray(NORMALS);
    //glDeleteVertexArrays(1, &VaoId);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);
}


void engine::Mesh::draw(){
    glBindVertexArray( VaoId );
    glDrawArrays( GL_TRIANGLES, 0, ( GLsizei )Vertices.size() );
    glBindVertexArray( 0 );
}

