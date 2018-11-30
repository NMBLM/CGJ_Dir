#pragma once
#ifndef QUATERNION_H
#define QUATERNION_H


#include "matrix.h"
#include "vector.h"
#include <iostream>
#include <string>



namespace engine{

    struct qtrn;

    struct qtrn{
        float t, x, y, z;

        qtrn();
        qtrn( float theta, const vec4& axis );
        qtrn( float t, float x, float y, float z );
        const void clean();

        const void qToAngleAxis( float& theta, vec4& axis ) const;
        const float quadrance() const;
        const float length() const;

        friend const mat4 qToMatrix( const qtrn q );
        friend const qtrn conjugate( const qtrn& q );
        friend const qtrn inverse( const qtrn& q );
        friend qtrn operator+( const qtrn& q1, const qtrn& q2 );
        friend qtrn operator-( const qtrn& q1, const qtrn& q2 );
        friend qtrn operator*( const qtrn& q, const float s );
        friend qtrn operator*( const float s, const qtrn& q );
        friend qtrn operator*( const qtrn& q1, const qtrn& q2 );
        qtrn operator=( const qtrn& q );
        friend const bool operator==( const qtrn& q1, const qtrn& q2 );
        friend const bool operator!=( const qtrn& q1, const qtrn& q2 );
        friend const qtrn normalize( const qtrn& q );

        friend const qtrn lerp( const qtrn& q0, const qtrn& q1, float k );
        friend const qtrn slerp( const qtrn& q0, const qtrn& q1, float k );
        friend std::ostream & operator<< ( std::ostream& out, const qtrn& q );
        friend const void printAngleAxis( const std::string& s, const qtrn& q );


    };
}


///////////////////////////////////////////////////////////////////////

#endif