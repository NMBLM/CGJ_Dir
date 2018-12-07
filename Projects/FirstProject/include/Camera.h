#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "Vector.h"
#include "Matrix.h"
#include "Qtrn.h"
#include "GL/glew.h"


using namespace engine;


class Camera{
    public:
    static GLuint CamVboId[1];
    
    protected:
    vec3 v;
    vec3 s;
    vec3 u;
    mat4 projection;
    vec3 eye;
    qtrn qPos;
    const float SPEED = 4.0f;
    const float vSPEED = 1.0f;
    float lastFrame = 0.0f;

    public:
    Camera();
    Camera( const vec3 eye, const vec3 center, const vec3 up );

    virtual mat4 ViewMatrix() = 0;
    mat4 ProjectionMatrix();

    virtual void ProjectionMatrix( mat4 proj );
    virtual void cameraLookAround( float x, float y, const float deltatime ) = 0;
    virtual void cameraMoveRight( const float deltatime ) = 0;
    virtual void cameraMoveLeft( const float deltatime ) = 0;
    virtual void cameraMoveForward( const float deltatime ) = 0;
    virtual void cameraMoveBack( const float deltatime ) = 0;
    void initalizeVbo();
    void setMatrix();
    mat3 rotation(){
        return qToMatrix( qPos );
    }
    qtrn rotationQtrn(){
        return qPos;
    }

};

class FixedCamera: public Camera{
    public:
    bool gLock = false;
    int print = 0;
    float pitch = 0.0f;
    float yaw = 0.0f;
    float roll = 0.0f;
    const float SPEED = 2.0f;

    FixedCamera();
    FixedCamera( const vec3 eye, const vec3 center, const vec3 up );

    void gimbalLockSwitch();
    virtual mat4 ViewMatrix();
    virtual void cameraLookAround( float x, float y, const float deltatime );
    virtual void cameraMoveRight( const float deltatime );
    virtual void cameraMoveLeft( const float deltatime );
    virtual void cameraMoveForward( const float deltatime );
    virtual void cameraMoveBack( const float deltatime );
    void cameraRollRight( const float deltatime );
    void cameraRollLeft( const float deltatime );
    void zoom( const int dir, const float deltatime );
    //virtual void setMatrix();

};

class FreeCamera: public Camera{
    public:


    FreeCamera();
    FreeCamera( const vec3 eye, const vec3 center, const vec3 up );

    virtual mat4 ViewMatrix();
    virtual void cameraLookAround( float x, float y, const float deltatime );
    virtual void cameraMoveRight( const float deltatime );
    virtual void cameraMoveLeft( const float deltatime );
    virtual void cameraMoveForward( const float deltatime );
    virtual void cameraMoveBack( const float deltatime );
    //virtual void setMatrix();

};

#endif