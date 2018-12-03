#include "camera.h"

using namespace engine;

GLuint Camera::CamVboId[1];

Camera::Camera(){
}

Camera::Camera( const vec3 eye, const vec3 center, const vec3 up ){
    this->eye = eye;
    v = center - eye;
    v = v * ( 1 / v.length() ); // normalize
    s = v.cross( up );
    s = s * ( 1 / s.length() );  // normalize
    u = s.cross( v ); //is a unit vector only because s and v are perpendicular to each other and are unit vectors |s x v| = |s||v|*sin(angle)
    initalizeVbo();
}

mat4 Camera::ProjectionMatrix(){
    return projection;
}

void Camera::ProjectionMatrix( mat4 proj ){
    projection = proj;
}

void Camera::initalizeVbo(){

    glGenBuffers( 1, Camera::CamVboId );
    glBindBuffer( GL_UNIFORM_BUFFER, Camera::CamVboId[0] );
    {
        glBufferData( GL_UNIFORM_BUFFER, sizeof( GLfloat[16] ) * 2, 0, GL_STREAM_DRAW );
        glBindBufferBase( GL_UNIFORM_BUFFER, 0, Camera::CamVboId[0] );
    }
    glBindBuffer( GL_UNIFORM_BUFFER, 0 );
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );

}

void Camera::setMatrix(){
    const GLsizeiptr matrixSize = sizeof( GLfloat[16] );
    glBindBuffer( GL_UNIFORM_BUFFER, Camera::CamVboId[0] );
    glBufferSubData( GL_UNIFORM_BUFFER, 0, matrixSize, ViewMatrix().data() );
    glBufferSubData( GL_UNIFORM_BUFFER, matrixSize, matrixSize, ProjectionMatrix().data() );
    glBindBuffer( GL_UNIFORM_BUFFER, 0 );
}

// FIXED CAMERA

FixedCamera::FixedCamera(){
}

FixedCamera::FixedCamera( const vec3 eye, const vec3 center, const vec3 up ){
    initalizeVbo();
    qPos = qtrn( 1, 0.2f, 0.1f, 0 );
    this->eye = eye;
}


void FixedCamera::gimbalLockSwitch(){
    gLock = !gLock;
    std::cout << "Gimbal Lock Mode: " << ( ( gLock ) ? "True" : "False" ) << std::endl;
}

mat4 FixedCamera::ViewMatrix(){
    mat4 T = MatrixFactory::createTranslationMatrix( 0.0f, 0.0f, -eye.z );

    if( gLock ){
        mat4 Rz, Rx, Ry, rot;
        Rx = MatrixFactory::createRotationMatrix4( -pitch, vec4( 1.0f, 0.0f, 0.0f, 1.0f ) );
        Ry = MatrixFactory::createRotationMatrix4( -yaw, vec4( 0.0f, 1.0f, 0.0f, 1.0f ) );
        Rz = MatrixFactory::createRotationMatrix4( -roll, vec4( 0.0f, 0.0f, 1.0f, 1.0f ) );
        rot = Rz * Rx * Ry;
        return T * rot;
    } else{
        return T * qToMatrix( qPos );
    }
}

void FixedCamera::cameraLookAround( float x, float y, const float deltatime ){

    int sideX = ( x >= 0 ) ? 1 : -1;
    int sideY = ( y >= 0 ) ? 1 : -1;
    float mulX = ( x < 1 && x > -1 ) ? 0.0f : 1.5f;
    float mulY = ( y < 1 && y > -1 ) ? 0.0f : 1.5f;
    qtrn qX, qY, qZ, q, qV;
    // GIMBAL LOCK ON

    if( gLock ){
        yaw += sideX * mulX * SPEED * 10.0f * deltatime;
        pitch += sideY * mulY * SPEED * 10.0f  * deltatime;
    } else{
        // GIMBAL LOCK OFF
        qX = qtrn( -sideX * mulX * SPEED * 10.0f  * deltatime, vec3( 0.0f, 1.0f, 0.0f ) );
        qY = qtrn( -sideY * mulY * SPEED * 10.0f * deltatime, vec3( 1.0f, 0.0f, 0.0f ) );
        qPos = qX * qY * qPos;
    }

}

void FixedCamera::cameraMoveRight( const float deltatime ){
    qtrn qX, q;
    if( !gLock ){

        qX = qtrn( -3.0f * deltatime * SPEED, vec3( 0.0f, 1.0f, 0.0f ) );
        qPos = qX * qPos;
    } else{
        yaw += 3.0f * deltatime * SPEED;
    }

}

void FixedCamera::cameraMoveLeft( const float deltatime ){
    qtrn qX, q;
    if( !gLock ){

        qX = qtrn( 3.0f * deltatime * SPEED, vec3( 0.0f, 1.0f, 0.0f ) );
        qPos = qX * qPos;
    } else{
        yaw += -3.0f * deltatime * SPEED;
    }

}

void FixedCamera::cameraMoveForward( const float deltatime ){
    qtrn qY, q;

    if( !gLock ){
        qY = qtrn( 3.0f * deltatime * SPEED, vec3( 1.0f, 0.0f, 0.0f ) );
        qPos = qY * qPos;
    } else{
        pitch += -3.0f * deltatime * SPEED;
    }

}

void FixedCamera::cameraMoveBack( const float deltatime ){
    qtrn qY, q;
    if( !gLock ){
        qY = qtrn( -3.0f * deltatime * SPEED, vec3( 1.0f, 0.0f, 0.0f ) );
        qPos = qY * qPos;
    } else{
        pitch += 3.0f * deltatime * SPEED;
    }

}

void FixedCamera::cameraRollRight( const float deltatime ){
    qtrn qZ, q;
    if( gLock ){
        roll += 3.0f * deltatime * SPEED;

    } else{
        qZ = qtrn( -3.0f * deltatime * SPEED, vec3( 0.0f, 0.0f, -1.0f ) );
        qPos = qZ * qPos;
    }


}

void FixedCamera::cameraRollLeft( const float deltatime ){
    qtrn qZ, q;
    if( gLock ){
        roll += -3.0f * deltatime * SPEED;
    } else{
        qZ = qtrn( 3.0f * deltatime * SPEED, vec3( 0.0f, 0.0f, -1.0f ) );
        qPos = qZ * qPos;
    }
}


void FixedCamera::zoom( const int dir, const float deltatime ){
    if( dir > 0 ){
        eye = eye + vec3( 0.0f, 0.0f, -2.0f ) * vSPEED * deltatime;
    } else{
        eye = eye - vec3( 0.0f, 0.0, -2.0f ) * vSPEED * deltatime;
    }

}



// FREE CAMERA
FreeCamera::FreeCamera(){
    initalizeVbo();
}

FreeCamera::FreeCamera( const vec3 eye, const vec3 center, const vec3 up ){
    this->eye = eye;
    qPos = qtrn( 1, 0.0f, 0.0f, 0.0f );
    v = center - eye;
    v = v * ( 1 / v.length() ); // normalize
    s = v.cross( up );
    s = s * ( 1 / s.length() );  // normalize
    u = s.cross( v ); //is a unit vector only because s and v are perpendicular to each other and are unit vectors |s x v| = |s||v|*sin(angle)
    initalizeVbo();
}

mat4 FreeCamera::ViewMatrix(){
    /** /
    float angle;
    vec3 axis;
    qPos.qToAngleAxis( angle, axis );
    std::cout << axis << std::endl;
    mat4 T = MatrixFactory::createTranslationMatrix( eye.x, eye.y, -eye.z );
    return  qToMatrix( qPos ) * T;
    /**/
    v = normalize( v );
    s = normalize( s );
    u = normalize( u );
    return MatrixFactory::createLookAt( eye, eye + v, u );
    /**/
}

void FreeCamera::cameraLookAround( float x, float y, const float deltatime ){
    int sideX = ( x >= 0 ) ? 1 : -1;
    int sideY = ( y >= 0 ) ? 1 : -1;
    float mulX = ( x > 2 || x < -2 ) ? 3.0f : 1.5f;
    float mulY = ( y > 2 || y < -2 ) ? 3.0f : 1.5f;
    mulX = ( x < 1 && x > -1 ) ? 0.0f : mulX;
    mulY = ( y < 1 && y > -1 ) ? 0.0f : mulY;
    /** /
    qtrn qX, qY;
    qX = qtrn( -sideX * mulX * SPEED * deltatime, vec3( 0.0f, 1.0f, 0.0f ) );
    qY = qtrn( -sideY * mulY * SPEED * deltatime, vec3( 1.0f, 0.0f, 0.0f ) );
    qPos = qX  * qY *  qPos;

    v = normalize( vec3( qToMatrix( qPos ) * vec3( 0.0f, 0.0f, 1.0f ) ) );
    s = normalize( vec3( qToMatrix( qPos ) * vec3( 1.0f, 0.0f, 0.0f ) ) );
    u = normalize( s.cross( v ) );
    /**/
    mat3 rotU = MatrixFactory::createRotationMatrix3( mulX *  sideX *  deltatime * SPEED , u );
    v = rotU * v;
    s = v.cross( u );
    mat3 rotS = MatrixFactory::createRotationMatrix3( mulY * sideY * deltatime * SPEED , s );
    v = rotS * v;
    u = s.cross( v );
    v = normalize( v );
    s = normalize( s );
    u = normalize( u );
    /**/
}

void FreeCamera::cameraMoveRight( const float deltatime ){
    eye = eye + s * vSPEED * deltatime;

}

void FreeCamera::cameraMoveLeft( const float deltatime ){
    eye = eye - s * vSPEED * deltatime;

}

void FreeCamera::cameraMoveForward( const float deltatime ){
    eye = eye + v * vSPEED * deltatime;

}

void FreeCamera::cameraMoveBack( const float deltatime ){
    eye = eye - v * vSPEED * deltatime;
}
