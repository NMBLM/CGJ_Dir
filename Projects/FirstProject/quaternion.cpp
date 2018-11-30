#include "quaternion.h"
#include "vector.h"
#include "matrix.h"

namespace engine{
    qtrn::qtrn(){
        t = 1.0f;
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }

    qtrn::qtrn( float t, float x, float y, float z ){
        this->t = t;
        this->x = x;
        this->y = y;
        this->z = z;
        this->clean();

    }

    qtrn::qtrn( const float theta, const vec4& axis ){
        vec3 axisn = normalize( vec3( axis ) );

        float a = theta * ( float )DEGREES_TO_RADIANS;
        t = cos( a / 2.0f );
        float s = sin( a / 2.0f );
        x = axisn.x * s;
        y = axisn.y * s;
        z = axisn.z * s;

        this->clean();
        qtrn q = normalize( *this );
        t = q.t;
        x = q.x;
        y = q.y;
        z = q.z;
        this->clean();


    }

    const void qtrn::clean(){
        t = ( fcmp( t, 0 ) ) ? 0 : t;
        x = ( fcmp( x, 0 ) ) ? 0 : x;
        y = ( fcmp( y, 0 ) ) ? 0 : y;
        z = ( fcmp( z, 0 ) ) ? 0 : z;
    }



    const void qtrn::qToAngleAxis( float & theta, vec4 & axis ) const{
        qtrn qn = normalize( *this );
        theta = 2.0f * acos( qn.t ) * ( float )RADIANS_TO_DEGREES;
        float s = sqrt( 1.0f - qn.t*qn.t );
        if( fcmp( s, 0 ) ){
            axis.x = 1.0f;
            axis.y = 0.0f;
            axis.z = 0.0f;
            axis.w = 1.0f;
        } else{
            axis.x = qn.x / s;
            axis.y = qn.y / s;
            axis.z = qn.z / s;
            axis.w = 1.0f;
        }
    }

    const float qtrn::quadrance() const{
        return t * t + x * x + y * y + z * z;
    }

    const float qtrn::length() const{
        return sqrt( this->quadrance() );
    }

    const qtrn conjugate( const qtrn & q ){
        qtrn qconj = { q.t, -q.x, -q.y, -q.z };
        return qconj;
    }

    const qtrn inverse( const qtrn & q ){
        return conjugate( q )* ( 1.0f / q.quadrance() );

    }

    qtrn qtrn::operator=( const qtrn & q ){
        t = q.t;
        x = q.x;
        y = q.y;
        z = q.z;
        return *this;
    }

    const mat4 qToMatrix( const qtrn q ){
        qtrn qn = normalize( q );
        float xx = qn.x * qn.x;
        float xy = qn.x * qn.y;
        float xz = qn.x * qn.z;
        float xt = qn.x * qn.t;
        float yy = qn.y * qn.y;
        float yz = qn.y * qn.z;
        float yt = qn.y * qn.t;
        float zz = qn.z * qn.z;
        float zt = qn.z * qn.t;

        mat4 res = mat4( 1.0f - 2.0f * ( yy + zz ), 2.0f * ( xy - zt ), 2.0f * ( xz + yt ), 0.0f,
            2.0f * ( xy + zt ), 1.0f - 2.0f * ( xx + zz ), 2.0f * ( yz - xt ), 0.0f,
            2.0f * ( xz - yt ), 2.0f * ( yz + xt ), 1.0f - 2.0f * ( xx + yy ),
            0.0f, 0.0f, 0.0f, 0.0f, 1.0f );
        res.clean();
        return res;
    }

    const qtrn lerp( const qtrn & q0, const qtrn & q1, float k ){
        float cos_angle = q0.x*q1.x + q0.y*q1.y + q0.z*q1.z + q0.t*q1.t;
        float k0 = 1.0f - k;
        float k1 = ( cos_angle > 0 ) ? k : -k;
        qtrn qi = ( q0* k0 ) + ( q1 * k1 );
        return normalize( qi );
    }

    const qtrn slerp( const qtrn & q0, const qtrn & q1, float k ){
        float cosine = q0.x*q1.x + q0.y*q1.y + q0.z*q1.z + q0.t*q1.t;
        if( fcmp( cosine, 1.0f ) || fcmp( cosine, -1.0f ) ){
            return qtrn( q0 );
        }
        float angle = acos( cosine );
        if( fcmp( angle, 0 ) ){
            return qtrn( q0 );
        }
        float k0 = sin( ( 1 - k )*angle ) / sin( angle );

        float k1 = sin( k*angle ) / sin( angle );
        qtrn qi = ( q0 * k0 ) + ( q1 * k1 );
        return normalize( qi );
    }



    qtrn operator+( const qtrn & q1, const qtrn & q2 ){
        qtrn q;
        q.t = q2.t + q1.t;
        q.x = q2.x + q1.x;
        q.y = q2.y + q1.y;
        q.z = q2.z + q1.z;
        return q;
    }

    qtrn operator-( const qtrn & q1, const qtrn & q2 ){
        qtrn q;
        q.t = q2.t - q1.t;
        q.x = q2.x - q1.x;
        q.y = q2.y - q1.y;
        q.z = q2.z - q1.z;
        return q;
    }

    qtrn operator*( const qtrn & q, const float s ){
        qtrn sq;
        sq.t = s * q.t;
        sq.x = s * q.x;
        sq.y = s * q.y;
        sq.z = s * q.z;
        return qtrn( s * q.t, s * q.x, s * q.y, s * q.z );
    }

    qtrn operator*( const float s, const qtrn & q ){
        qtrn sq;
        sq.t = s * q.t;
        sq.x = s * q.x;
        sq.y = s * q.y;
        sq.z = s * q.z;
        return sq;
    }


    qtrn operator*( const qtrn & q1, const qtrn & q2 ){
        qtrn q;
        q.t = q1.t * q2.t - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
        q.x = q1.t * q2.x + q1.x * q2.t + q1.y * q2.z - q1.z * q2.y;
        q.y = q1.t * q2.y + q1.y * q2.t + q1.z * q2.x - q1.x * q2.z;
        q.z = q1.t * q2.z + q1.z * q2.t + q1.x * q2.y - q1.y * q2.x;
        return q;
    }


    const bool operator==( const qtrn & q1, const qtrn & q2 ){
        return fcmp( q1.t, q2.t ) && fcmp( q1.x, q2.x ) && fcmp( q1.y, q2.y ) && fcmp( q1.z, q2.z );
    }

    const bool operator!=( const qtrn & q1, const qtrn & q2 ){
        return !( fcmp( q1.t, q2.t ) && fcmp( q1.x, q2.x ) && fcmp( q1.y, q2.y ) && fcmp( q1.z, q2.z ) );
    }

    const qtrn normalize( const qtrn & q ){
        float s = 1 / q.length();
        return q * s;
    }
    std::ostream & operator<<( std::ostream & out, const qtrn & q ){
        out << " = (" << q.t << ", " << q.x << ", " << q.y << ", " << q.z << ")" << std::endl;
        return out;
    }

    const void printAngleAxis( const std::string& s, const qtrn & q ){
        std::cout << s << " = [ ";

        float thetaf;
        vec4 axis_f = vec4( 0.0f );
        q.qToAngleAxis( thetaf, axis_f );
        std::cout << "  angle = " << thetaf;
        std::cout << " , axis = " << axis_f;
        std::cout << "]" << std::endl;
    }

}
