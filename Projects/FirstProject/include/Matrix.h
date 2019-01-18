#pragma once
#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"
#include <iostream>


#define DEGREES_TO_RADIANS 0.017453292519f
#define RADIANS_TO_DEGREES 57.29577951308f

namespace engine{

    struct mat2;
    struct mat3;
    struct mat4;
    struct MatrixFactory;

    struct mat2{
        float mat[4] = {};
        /*
        |	0	1	|
        |	2	3	|
        */
        //constructor
        mat2( const float k ); // diagonal

        mat2( const float m11, const  float m12, 	// First Row
            const float m21, const  float m22 ); 	// Second Row

        const float* data( mat2& m );
        void clean();

        mat2 operator= ( const mat2& m );
        mat2 operator+= ( const mat2& m );
        mat2 operator-= ( const mat2& m );
        friend mat2 operator+ ( const mat2& m1, const mat2& m2 );
        friend mat2 operator- ( const mat2& m1, const mat2& m2 );
        friend mat2 operator* ( const mat2& m1, const mat2& m2 );
        friend mat2 operator* ( const mat2& m, const float alpha );
        friend mat2 operator* ( const float alpha, const mat2& m );
        friend vec2 operator* ( const mat2& m, const vec2& v );

        //transpose, determinant and inverse (except for 4x4 matrices)
        const mat2 transpose();
        const float determinant();
        const mat2 inverse();

        //input/output
        friend std::ostream& operator<< ( std::ostream& out, const mat2& m );
        friend std::istream& operator>> ( std::istream& in, mat2& m );


    };

    struct mat3{
        float mat[9] = {};
        /*
        |	0	1	2	|
        |	3	4	5	|
        |	6	7	8	|
        */
        //constructor
        mat3( const float k );

        mat3( const float m11, const  float m12, const  float m13,	// First Row
            const float m21, const  float m22, const  float m23,	// Second Row
            const float m31, const  float m32, const  float m33 );	// Third Row
        mat3( const mat3& m );
        mat3( const mat4& m );

        const float* data();
        void clean();




        //assignment, comparison



        //sum, subtraction 

        //multiplication (by another matrix, a scalar, or a vector)
        mat3 operator= ( const mat3& m );
        mat3 operator+= ( const mat3& m );
        mat3 operator-= ( const mat3& m );
        friend bool operator== ( const mat3& m1, const mat3& m2 );
        friend mat3 operator+ ( const mat3& m1, const mat3& m2 );
        friend mat3 operator- ( const mat3& m1, const mat3& m2 );
        friend mat3 operator* ( const mat3& m1, const mat3& m2 );
        friend mat3 operator* ( const mat3& m, const float alpha );
        friend mat3 operator* ( const float alpha, const mat3& m );
        friend vec3 operator* ( const mat3& m, const vec3& v );

        //transpose, determinant and inverse (except for 4x4 matrices)
        const mat3 transpose();
        const float determinant();
        friend const mat3 inverse( mat3& m );

        //input/output
        friend std::ostream& operator<< ( std::ostream& out, const mat3& m );
        friend std::istream& operator>> ( std::istream& in, mat3& m );
    };

    struct mat4{
        float mat[16] = {};
        /*
        |	0	1	2	3	|
        |	4	5	6	7	|
        |	8	9	10	11	|
        |	12	13	14	15	|
        */

        //constructor
        mat4();
        mat4( const float k );

        mat4( const float m11, const  float m12, const  float m13, const  float m14,	 // First  Row
            const float m21, const  float m22, const  float m23, const  float m24,	 // Second Row
            const float m31, const  float m32, const  float m33, const  float m34,  // Third  Row
            const float m41, const  float m42, const  float m43, const  float m44 ); // Fourth Row	

        mat4( const mat3& m, const float w = 1 );
        mat4( const mat4& m );
        const float* data() const;
        void clean();

        mat4 operator= ( const mat4& m );
        mat4 operator+= ( const mat4& m );
        mat4 operator-= ( const mat4& m );
        friend mat4 operator+ ( const mat4& m1, const mat4& m2 );
        friend mat4 operator- ( const mat4& m1, const mat4& m2 );
        friend mat4 operator* ( const mat4& m1, const mat4& m2 );
        friend mat4 operator* ( const mat4& m, const float alpha );
        friend mat4 operator* ( const float alpha, const mat4& m );
        friend vec4 operator* ( const mat4& m, const vec4& v );
        friend mat4 inverse( const mat4& m );
        friend float determinant( const mat4& m );
        friend std::ostream& operator<< ( std::ostream& out, const mat4& m );
        friend std::istream& operator>> ( std::istream& in, mat4& m );

    };


    struct MatrixFactory{

        // mat2
        static const mat2 createIdentityMatrix2();
        static const mat2 createScaleMatrix2( const float sx, const float sy );

        // mat3
        static const mat3 createIdentityMatrix3();
        static const mat3 createRotationMatrix3( const float angle, const vec3& v );
        static const mat3 createDualMatrix( const vec3& v );
        static const mat3 createSquareDualMatrix( const vec3& v );
        static const mat3 createScaleMatrix3( const float x, const float y, const float z );
        static const mat3 createNormalMatrix( const mat4& m );
        static const inline mat3 transformMatrix4to3( const mat4& m );

        // mat4
        static const mat4 createIdentityMatrix4();
        static const mat4 createRotationMatrix4( const float angle, const vec4& v );
        static const mat4 createTranslationMatrix( const float x, const float y, const float z );
        static const mat4 createTranslationMatrix( const vec3& v );
        static const mat4 createScaleMatrix4( const float x, const float y, const float z );
        static const inline mat4 transformMatrix3to4( const mat3& m );
        static const mat4 createLookAt( const vec3 eye, const vec3 center, const vec3 up );
        static const mat4 createLookAt( const vec3 eye, const vec3 view, const vec3 up, const vec3 side );
        static const mat4 createOrtographicProjectionMatrix( const float left, const float right,
            const float bottom, const float top,
            const float near, const float far );
        static const mat4 createPerspectiveProjectionMatrix( const float fovY, const float aspect, const float near, const float far );// aspect = width / height
        static const mat4 createReflectionMatrix(vec4 reflectionPlane );// aspect = width / height
        const mat4 createReflectionMatrix2 ( vec4 rp, vec3 p );
    };

}

#endif