#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(){
    id = glCreateProgram();
}

ShaderProgram::ShaderProgram( GLuint i ){
    id = i;
}

void ShaderProgram::attachShader( GLuint type, const char * name, const char * filename ){
    std::ifstream v( filename );
    std::string data;
    data.assign( ( std::istreambuf_iterator<char>( v ) ),
                 std::istreambuf_iterator<char>() );
    const GLchar * shader = data.c_str();
    int success = 0;
    char log[512];
    GLuint tempid = glCreateShader( type );

    shaderid.insert( std::make_pair( name, tempid ) );

    glShaderSource( tempid, 1, &shader, 0 );
    glCompileShader( tempid );
    glGetShaderiv( tempid, GL_COMPILE_STATUS, &success );
    if( !success ){
        glGetShaderInfoLog( tempid, 512, NULL, log );
        std::cout << "ERROR::SHADER::" << name << "::COMPILATION_FAILED\n" << log << std::endl;
    };
    glAttachShader( id, tempid );

}

void ShaderProgram::detachShader( const char * name ){
    GLuint sid = shaderid.find( name )->second;
    glDetachShader( id, sid );
    glDeleteShader( sid );
}

void ShaderProgram::link(){
    int success = 0;
    char log[512];
    glLinkProgram( id );
    glGetProgramiv( id, GL_LINK_STATUS, &success );
    if( !success ){
        glGetProgramInfoLog( id, 512, NULL, log );
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << log << std::endl;
    }
}

void ShaderProgram::bindAttribLocation( const int atnum, const char * atname ){
    glBindAttribLocation( id, atnum, atname );
}

void ShaderProgram::uniformBlockBinding( GLint bid, GLuint bpo ){
    glUniformBlockBinding( id, bid, bpo );
}


GLint ShaderProgram::UniformId( const char * uniformName ){
    return glGetUniformLocation( id, uniformName );
}

GLint ShaderProgram::uniformBlockIndex( const char * uniformName ){
    return glGetUniformBlockIndex( id, uniformName );
}

void ShaderProgram::addUniform( const char * name, vec4 v ){
    glUniform4fv( UniformId( name ), 1, v.data() );
}

void ShaderProgram::addUniform( const char * name, vec3 v ){
    glUniform3fv( UniformId( name ), 1, v.data() );
}

void ShaderProgram::addUniform( const char * name, vec2 v ){
    glUniform2fv( UniformId( name ), 1, v.data() );
}

void ShaderProgram::addUniform( const char * name, float f ){
    glUniform1f( UniformId( name ), f );
}

void ShaderProgram::addUniform( const char * name, int i){
    glUniform1i( UniformId( name ), i );
}

void ShaderProgram::addUniform( const char * name, mat3 m ){
    glUniformMatrix3fv( UniformId( name ), 1, GL_FALSE, m.data() );
}

void ShaderProgram::addUniform( const char * name, mat4 m ){
    glUniformMatrix4fv( UniformId( name ), 1, GL_FALSE, m.data() );
}


void ShaderProgram::use(){
    glUseProgram( id );
    //for (auto& m : unimat4) {
    //	glUniformMatrix4fv(UniformId(m.first.c_str()), 1, GL_FALSE, m.second.data());
    //}

    //for (auto& m : univec4) {
    //	glUniform4fv(UniformId(m.first.c_str()), 1, m.second.data());
    //}

    //for (auto& m : univec3) {
    //	glUniform3fv(UniformId(m.first.c_str()), 1,  m.second.data());
    //}

    //for (auto& m : univec2) {
    //	glUniform2fv(UniformId(m.first.c_str()), 1, m.second.data());
    //}

    //for (auto& m : unifloat) {
    //	glUniform1f(UniformId(m.first.c_str()), m.second);
    //}
}

void ShaderProgram::stop(){
    glUseProgram( 0 );
}




