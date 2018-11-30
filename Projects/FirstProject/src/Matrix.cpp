#include "matrix.h"

#include "vector.h"


namespace engine{


    // MAT 2 /////////////////////////////////////

    mat2::mat2( const float k ){
        mat[0] = k; mat[3] = k;
    }

    mat2::mat2( const float m11, const float m12, const float m21, const float m22 ){
        mat[0] = m11; mat[1] = m12;
        mat[2] = m21; mat[3] = m22;
    }

    const float * mat2::data( mat2 & m ){
        float * data = new float[4]();
        data[0] = m.mat[0]; data[1] = m.mat[2];
        data[2] = m.mat[1]; data[3] = m.mat[3];
        return data;
    }

    void mat2::clean(){
        for( int i = 0; i < 9; i++ ){
            if( fcmp( mat[i], 0 ) ){
                mat[i] = 0;
            }
        }
    }

    mat2 mat2::operator=( const mat2 & m ){
        for( int i = 0; i < 4; i++ ){
            mat[i] = m.mat[i];
        }
        return *this;
    }

    mat2 mat2::operator+=( const mat2 & m ){

        for( int i = 0; i < 4; i++ ){
            this->mat[i] += m.mat[i];
        }
        return *this;
    }

    mat2 mat2::operator-=( const mat2 & m ){
        for( int i = 0; i < 4; i++ ){
            this->mat[i] -= m.mat[i];
        }
        return *this;
    }



    mat2 operator+( const mat2 & m1, const mat2 & m2 ){
        return mat2( m1.mat[0] + m2.mat[0], m1.mat[1] + m2.mat[1],
                     m1.mat[2] + m2.mat[2], m1.mat[3] + m2.mat[3] );
    }

    mat2 operator-( const mat2 & m1, const mat2 & m2 ){
        return mat2( m1.mat[0] - m2.mat[0], m1.mat[1] - m2.mat[1],
                     m1.mat[2] - m2.mat[2], m1.mat[3] - m2.mat[3] );
    }

    mat2 operator*( const mat2 & m1, const mat2 & m2 ){
        /*
        |	0	1	| 	*	|	a	b	|		|	0*a+1*c		0*b+1*d	|
        |	2	3	|		|	c	d	|	=	|	2*a+3*c		0*b+1*d	|
        */

        return mat2( m1.mat[0] * m2.mat[0] + m1.mat[1] * m2.mat[2],	//m11
                     m1.mat[0] * m2.mat[1] + m1.mat[1] * m2.mat[3],	//m12
                     m1.mat[2] * m2.mat[0] + m1.mat[3] * m2.mat[2],	//m21
                     m1.mat[2] * m2.mat[1] + m1.mat[3] * m2.mat[3] );	//m22
    }

    mat2 operator*( const mat2 & m, const float alpha ){
        return mat2( m.mat[0] * alpha, m.mat[1] * alpha,
                     m.mat[2] * alpha, m.mat[3] * alpha );
    }

    mat2 operator*( const float alpha, const mat2 & m ){
        return mat2( m.mat[0] * alpha, m.mat[1] * alpha,
                     m.mat[2] * alpha, m.mat[3] * alpha );
    }

    vec2 operator*( const mat2 & m, const vec2 & v ){
        vec2 ret = vec2( 0 );
        ret.x = m.mat[0] * v.x + m.mat[1] * v.y;
        ret.y = m.mat[2] * v.x + m.mat[3] * v.y;
        return ret;
    }

    const mat2 mat2::transpose(){
        return mat2( mat[0], mat[2],
                     mat[1], mat[3] );
    }

    const float mat2::determinant(){

        return mat[0] * mat[3] - mat[1] * mat[2];
    }

    const mat2 mat2::inverse(){
        return ( 1 / this->determinant() ) * mat2( mat[3], -mat[1], -mat[2], mat[0] );
    }

    std::ostream & operator<<( std::ostream & out, const mat2 & m ){
        out << "| " << m.mat[0] << "  " << m.mat[1] << " |" << std::endl;
        out << "| " << m.mat[2] << "  " << m.mat[3] << " |" << std::endl;

        return out;
    }

    std::istream & operator>>( std::istream & in, mat2 & m ){
        in >> m.mat[0] >> m.mat[1] >> m.mat[2]
            >> m.mat[3] >> m.mat[4] >> m.mat[5]
            >> m.mat[6] >> m.mat[7] >> m.mat[8];
        return in;
    }


    // MAT 3 /////////////////////////////////////
    mat3::mat3( const float k ) // diagonal is same value
    {
        mat[0] = mat[4] = mat[8] = k;
    }
    mat3::mat3( const float m11, const float m12, const float m13,
                const float m21, const float m22, const float m23,
                const float m31, const float m32, const float m33 ){
        mat[0] = m11;	mat[1] = m12;	mat[2] = m13;
        mat[3] = m21;	mat[4] = m22;	mat[5] = m23;
        mat[6] = m31;	mat[7] = m32;	mat[8] = m33;
    }
    mat3::mat3( const mat3& m ){
        memcpy( mat, m.mat, sizeof( m.mat ) );
    }
    mat3::mat3( const mat4& m ){

        mat[0] = m.mat[0];	mat[1] = m.mat[1];	mat[2] = m.mat[2];
        mat[3] = m.mat[4];	mat[4] = m.mat[5];	mat[5] = m.mat[6];
        mat[6] = m.mat[8];	mat[7] = m.mat[9];	mat[8] = m.mat[10];
    }

    const float * mat3::data(){
        float* data = new float[9]();
        data[0] = mat[0]; data[1] = mat[3]; data[2] = mat[6];
        data[3] = mat[1]; data[4] = mat[4]; data[5] = mat[7];
        data[6] = mat[2]; data[7] = mat[8]; data[9] = mat[8];
        return data;
    }

    void mat3::clean(){
        for( int i = 0; i < 9; i++ ){
            if( fcmp( mat[i], 0 ) ){
                mat[i] = 0;
            }
        }
    }

    mat3 mat3::operator=( const mat3 & m ){
        for( int i = 0; i < 9; i++ ){
            mat[i] = m.mat[i];
        }
        return *this;
    }

    mat3 mat3::operator+=( const mat3 & m ){

        for( int i = 0; i < 9; i++ ){
            mat[i] += m.mat[i];
        }
        return *this;
    }

    mat3 mat3::operator-=( const mat3 & m ){
        for( int i = 0; i < 9; i++ ){
            mat[i] -= m.mat[i];
        }
        return *this;
    }



    bool operator==( const mat3 & m1, const mat3 & m2 ){
        for( int i = 0; i < 9; i++ ){
            if( !fcmp( m1.mat[i], m2.mat[i] ) ){
                return false;
            }
        }
        return true;
    }

    mat3 operator+( const mat3 & m1, const mat3 & m2 ){
        return mat3( m1.mat[0] + m2.mat[0], m1.mat[1] + m2.mat[1], m1.mat[2] + m2.mat[2],
                     m1.mat[3] + m2.mat[3], m1.mat[4] + m2.mat[4], m1.mat[5] + m2.mat[5],
                     m1.mat[6] + m2.mat[6], m1.mat[7] + m2.mat[7], m1.mat[8] + m2.mat[8] );
    }

    mat3 operator-( const mat3 & m1, const mat3 & m2 ){
        return mat3( m1.mat[0] - m2.mat[0], m1.mat[1] - m2.mat[1], m1.mat[2] - m2.mat[2],
                     m1.mat[3] - m2.mat[3], m1.mat[4] - m2.mat[4], m1.mat[5] - m2.mat[5],
                     m1.mat[6] - m2.mat[6], m1.mat[7] - m2.mat[7], m1.mat[8] - m2.mat[8] );
    }

    mat3 operator*( const mat3 & m1, const mat3 & m2 ){
        /*
        |	0	1	2	| 		|	a	b	c	|		|	0*a+1*d+2*g		0*b+1*e+2*h		0*c+1*f+2*i		|
        |	3	4	5	|	*	|	d	e	f	|	=	|	3*a+4*d+5*g		3*b+4*e+5*h		3*c+4*f+5*i		|
        |	6	7	8	|		|	g	h	i	|		|	6*a+7*d+8*g		6*b+7*e+8*h		6*c+7*f+8*i		|
        */
        return mat3( m1.mat[0] * m2.mat[0] + m1.mat[1] * m2.mat[3] + m1.mat[2] * m2.mat[6],	//m11
                     m1.mat[0] * m2.mat[1] + m1.mat[1] * m2.mat[4] + m1.mat[2] * m2.mat[7],	//m12
                     m1.mat[0] * m2.mat[2] + m1.mat[1] * m2.mat[5] + m1.mat[2] * m2.mat[8],	//m13
                     m1.mat[3] * m2.mat[0] + m1.mat[4] * m2.mat[3] + m1.mat[5] * m2.mat[6],	//m21
                     m1.mat[3] * m2.mat[1] + m1.mat[4] * m2.mat[4] + m1.mat[5] * m2.mat[7],	//m22 
                     m1.mat[3] * m2.mat[2] + m1.mat[4] * m2.mat[5] + m1.mat[5] * m2.mat[8],	//m23
                     m1.mat[6] * m2.mat[0] + m1.mat[7] * m2.mat[3] + m1.mat[8] * m2.mat[6],	//m31
                     m1.mat[6] * m2.mat[1] + m1.mat[7] * m2.mat[4] + m1.mat[8] * m2.mat[7],	//m32 
                     m1.mat[6] * m2.mat[2] + m1.mat[7] * m2.mat[5] + m1.mat[8] * m2.mat[8] );	//m33
    }

    mat3 operator*( const mat3 & m, const float alpha ){
        return mat3( m.mat[0] * alpha, m.mat[1] * alpha, m.mat[2] * alpha,
                     m.mat[3] * alpha, m.mat[4] * alpha, m.mat[5] * alpha,
                     m.mat[6] * alpha, m.mat[7] * alpha, m.mat[8] * alpha );
    }

    mat3 operator*( const float alpha, const mat3 & m ){
        return mat3( m.mat[0] * alpha, m.mat[1] * alpha, m.mat[2] * alpha,
                     m.mat[3] * alpha, m.mat[4] * alpha, m.mat[5] * alpha,
                     m.mat[6] * alpha, m.mat[7] * alpha, m.mat[8] * alpha );
    }

    vec3 operator*( const mat3 & m, const vec3 & v ){
        vec3 ret = vec3( 0 );
        ret.x = m.mat[0] * v.x + m.mat[1] * v.y + m.mat[2] * v.z;
        ret.y = m.mat[3] * v.x + m.mat[4] * v.y + m.mat[5] * v.z;
        ret.z = m.mat[6] * v.x + m.mat[7] * v.y + m.mat[8] * v.z;
        return ret;
    }

    const mat3 mat3::transpose(){	/*
        |	0	1	2	|T 		|	0	3	6	|
        |	3	4	5	|	=	|	1	4	7	|
        |	6	7	8	|		|	2	5	8	|
        */

        // [0,1,2,3,4,5,6,7,8]T = [0,3,6,1,4,7,2,5,8]

        return mat3( mat[0], mat[3], mat[6],
                     mat[1], mat[4], mat[7],
                     mat[2], mat[5], mat[8] );
    }

    const float mat3::determinant(){
        /*
        |	0,m11	1,m12	2,m13	|
        |	3,m21	4,m22	5,m23	|
        |	6,m31	7,m32	8,m33	|
        */
        return mat[0] * ( mat[4] * mat[8] - mat[5] * mat[7] )
            - mat[1] * ( mat[3] * mat[8] - mat[5] * mat[6] )
            + mat[3] * ( mat[3] * mat[7] - mat[4] * mat[6] );
    }

    const mat3 inverse( mat3& m ){
        // first det != 0
        float det = m.determinant();
        if( fcmp( 0, det ) ){
            throw std::overflow_error( "Divide by zero" );
        }

        // second transpose
        mat3 t = m.transpose();
        mat3 co_f = mat3( t.mat[4] * t.mat[8] - t.mat[5] * t.mat[7], -( t.mat[3] * t.mat[8] - t.mat[5] * t.mat[6] ), t.mat[3] * t.mat[7] - t.mat[4] * t.mat[6]
                          , -( t.mat[1] * t.mat[8] - t.mat[2] * t.mat[7] ), t.mat[0] * t.mat[8] - t.mat[2] * t.mat[6], -( t.mat[0] * t.mat[7] - t.mat[1] * t.mat[6] )
                          , t.mat[1] * t.mat[5] - t.mat[2] * t.mat[4], -( t.mat[0] * t.mat[5] - t.mat[2] * t.mat[3] ), t.mat[0] * t.mat[4] - t.mat[1] * t.mat[3] );
        // third and fourth , determinant of minor matrices, create co-factor matrix and adjugate
        return ( 1 / det )*co_f;
    }

    std::ostream & operator<<( std::ostream & out, const mat3 & m ){
        out << "| " << m.mat[0] << "  " << m.mat[1] << "  " << m.mat[2] << " |" << std::endl;
        out << "| " << m.mat[3] << "  " << m.mat[4] << "  " << m.mat[5] << " |" << std::endl;
        out << "| " << m.mat[6] << "  " << m.mat[7] << "  " << m.mat[8] << " |" << std::endl;

        return out;
    }

    std::istream & operator>>( std::istream & in, mat3 & m ){
        in >> m.mat[0] >> m.mat[1] >> m.mat[2]
            >> m.mat[3] >> m.mat[4] >> m.mat[5]
            >> m.mat[6] >> m.mat[7] >> m.mat[8];
        return in;
    }




    // MAT 4 /////////////////////////////////////

    mat4::mat4(){
    }
    mat4::mat4( const float k ){
        mat[0] = k; mat[5] = k; mat[10] = k; mat[15] = k;
    }

    mat4::mat4( const float m11, const float m12, const float m13, const float m14,
                const float m21, const float m22, const float m23, const float m24,
                const float m31, const float m32, const float m33, const float m34,
                const float m41, const float m42, const float m43, const float m44 ){
        mat[0] = m11;	mat[1] = m12;	mat[2] = m13;	mat[3] = m14;
        mat[4] = m21;	mat[5] = m22;	mat[6] = m23;	mat[7] = m24;
        mat[8] = m31;	mat[9] = m32;	mat[10] = m33;	mat[11] = m34;
        mat[12] = m41;	mat[13] = m42;	mat[14] = m43;	mat[15] = m44;
    }

    mat4::mat4( const mat3 & m, const float w ){
        mat[0] = m.mat[0];	mat[1] = m.mat[1];	mat[2] = m.mat[2];
        mat[4] = m.mat[3];	mat[5] = m.mat[4];	mat[6] = m.mat[5];
        mat[8] = m.mat[6];	mat[9] = m.mat[7];	mat[10] = m.mat[8];
        mat[15] = w;
    }

    mat4::mat4( const mat4 & m ){
        memcpy( mat, m.mat, sizeof( m.mat ) );
    }

    const float * mat4::data() const{
        float* data = new float[16]();
        data[0] = mat[0];	data[1] = mat[4];	data[2] = mat[8];	data[3] = mat[12];
        data[4] = mat[1];	data[5] = mat[5];	data[6] = mat[9];	data[7] = mat[13];
        data[8] = mat[2];	data[9] = mat[6];	data[10] = mat[10];	data[11] = mat[14];
        data[12] = mat[3];	data[13] = mat[7];	data[14] = mat[11];	data[15] = mat[15];
        return data;
    }

    void mat4::clean(){
        for( int i = 0; i < 16; i++ ){
            if( fcmp( mat[i], 0 ) ){
                mat[i] = 0;
            }
        }
    }

    mat4 mat4::operator=( const mat4 & m ){
        for( int i = 0; i < 16; i++ ){
            mat[i] = m.mat[i];
        }
        return *this;
    }

    mat4 mat4::operator+=( const mat4 & m ){
        for( int i = 0; i < 16; i++ ){
            this->mat[i] += m.mat[i];
        }
        return *this;
    }
    mat4 mat4::operator-=( const mat4 & m ){
        for( int i = 0; i < 16; i++ ){
            this->mat[i] -= m.mat[i];
        }
        return *this;
    }


    mat4 operator+( const mat4 & m1, const mat4 & m2 ){
        mat4 m = mat4( 1 );
        for( int i = 0; i < 16; i++ ){
            m.mat[i] = m1.mat[i] + m2.mat[i];
        }
        return m;
    }

    mat4 operator-( const mat4 & m1, const mat4 & m2 ){
        mat4 m = mat4( 1 );
        for( int i = 0; i < 16; i++ ){
            m.mat[i] = m1.mat[i] - m2.mat[i];
        }
        return m;
    }

    mat4 operator*( const mat4 & m1, const mat4 & m2 ){
        /*
        |	0	1	2	3	| |	0	1	2	3	| |	0*0 +1*4 + 2*8 +3*12	0*1 +1*5 + 2*9 +3*13 	0*2 +1*6 +2 *10 +3*14	0*3 +1*7 +2 *11 +3*15	|
        |	4	5	6	7	|*|	4	5	6	7	|=|	4*0 +1*4 + 2*6 +7*12	4*1 +5*5 + 6*9 +7*13 	4*2 +5*6 +6 *10 +7*14	4*3 +5*7 +6 *11 +7*15	|
        |	8	9	10	11	| |	8	9	10	11	| |	8*0 +9*4 +10*8+11*12	8*1 +9*5 +10*9+11*13	8*2 +9*6 +10*10+11*14	8*3 +9*7 +10*11+11*15	|
        |	12	13	14	15	| |	12	13	14	15	| |	12*0+13*4+14*8+15*12	12*1+13*5+14*9+15*13	12*2+13*6+14*10+15*14	12*3+13*7+14*11+15*15	|
        */
        mat4 m = mat4( 2 );

        for( int i = 0; i < 16; i++ ){
            for( int j = 0; j < 13; j += 4 ){
                for( int k = 0; k < 4; i++, k++ ){
                    m.mat[i] = m1.mat[j] * m2.mat[k] + m1.mat[j + 1] * m2.mat[k + 4] + m1.mat[j + 2] * m2.mat[k + 8] + m1.mat[j + 3] * m2.mat[k + 12];
                    /*std::cout << "m.mat[i =" << i << "] = " << m.mat[i] << " = "
                        << "(m1.mat[j =" << j << "] = " << m1.mat[j] << ") * " << "(m2.mat[k =" << k << "] = " << m2.mat[k] << ") + "
                        << "(m1.mat[j +1 =" << j + 1 << "] = " << m1.mat[j + 1] << ") * " << "(m2.mat[k+4 =" << k + 4 << "] = " << m2.mat[k + 4] << ") + "
                        << "(m1.mat[j +2 =" << j + 2 << "] = " << m1.mat[j + 2] << ") * " << "(m2.mat[k+8 =" << k + 8 << "] = " << m2.mat[k + 8] << ") + "
                        << "(m1.mat[j +3 =" << j + 3 << "] = " << m1.mat[j + 3] << ") * " << "(m2.mat[k+12 =" << k + 12 << "] = " << m2.mat[k + 12] << ")" << std::endl;
                    */
                }
            }
        }

        return m;
    }

    mat4 operator*( const mat4 & m, const float alpha ){
        mat4 r = mat4( 1 );
        for( int i = 0; i < 16; i++ ){
            r.mat[i] = m.mat[i] * alpha;
        }
        return r;
    }

    mat4 operator*( const float alpha, const mat4 & m ){
        mat4 r = mat4( 1 );
        for( int i = 0; i < 16; i++ ){
            r.mat[i] = m.mat[i] * alpha;
        }
        return r;
    }

    vec4 operator*( const mat4 & m, const vec4 & v ){
        vec4 ret = vec4( 0 );
        ret.x = m.mat[0] * v.x + m.mat[1] * v.y + m.mat[2] * v.z + m.mat[3] * v.w;
        ret.y = m.mat[4] * v.x + m.mat[5] * v.y + m.mat[6] * v.z + m.mat[7] * v.w;
        ret.z = m.mat[8] * v.x + m.mat[9] * v.y + m.mat[10] * v.z + m.mat[11] * v.w;
        ret.w = m.mat[12] * v.x + m.mat[13] * v.y + m.mat[14] * v.z + m.mat[15] * v.w;
        return ret;

    }

    mat4 inverse( const mat4 & m ){
        mat4 res = mat4( 1 );
        res.mat[0] = m.mat[6] * m.mat[11] * m.mat[13] - m.mat[7] * m.mat[10] * m.mat[13] + m.mat[7] * m.mat[9] * m.mat[14] - m.mat[5] * m.mat[11] * m.mat[14] - m.mat[6] * m.mat[9] * m.mat[15] + m.mat[5] * m.mat[10] * m.mat[15];
        res.mat[1] = m.mat[3] * m.mat[10] * m.mat[13] - m.mat[2] * m.mat[11] * m.mat[13] - m.mat[3] * m.mat[9] * m.mat[14] + m.mat[1] * m.mat[11] * m.mat[14] + m.mat[2] * m.mat[9] * m.mat[15] - m.mat[1] * m.mat[10] * m.mat[15];
        res.mat[2] = m.mat[2] * m.mat[7] * m.mat[13] - m.mat[3] * m.mat[6] * m.mat[13] + m.mat[3] * m.mat[5] * m.mat[14] - m.mat[1] * m.mat[7] * m.mat[14] - m.mat[2] * m.mat[5] * m.mat[15] + m.mat[1] * m.mat[6] * m.mat[15];
        res.mat[3] = m.mat[3] * m.mat[6] * m.mat[9] - m.mat[2] * m.mat[7] * m.mat[9] - m.mat[3] * m.mat[5] * m.mat[10] + m.mat[1] * m.mat[7] * m.mat[10] + m.mat[2] * m.mat[5] * m.mat[11] - m.mat[1] * m.mat[6] * m.mat[11];
        res.mat[4] = m.mat[7] * m.mat[10] * m.mat[12] - m.mat[6] * m.mat[11] * m.mat[12] - m.mat[7] * m.mat[8] * m.mat[14] + m.mat[4] * m.mat[11] * m.mat[14] + m.mat[6] * m.mat[8] * m.mat[15] - m.mat[4] * m.mat[10] * m.mat[15];
        res.mat[5] = m.mat[2] * m.mat[11] * m.mat[12] - m.mat[3] * m.mat[10] * m.mat[12] + m.mat[3] * m.mat[8] * m.mat[14] - m.mat[0] * m.mat[11] * m.mat[14] - m.mat[2] * m.mat[8] * m.mat[15] + m.mat[0] * m.mat[10] * m.mat[15];
        res.mat[6] = m.mat[3] * m.mat[6] * m.mat[12] - m.mat[2] * m.mat[7] * m.mat[12] - m.mat[3] * m.mat[4] * m.mat[14] + m.mat[0] * m.mat[7] * m.mat[14] + m.mat[2] * m.mat[4] * m.mat[15] - m.mat[0] * m.mat[6] * m.mat[15];
        res.mat[7] = m.mat[2] * m.mat[7] * m.mat[8] - m.mat[3] * m.mat[6] * m.mat[8] + m.mat[3] * m.mat[4] * m.mat[10] - m.mat[0] * m.mat[7] * m.mat[10] - m.mat[2] * m.mat[4] * m.mat[11] + m.mat[0] * m.mat[6] * m.mat[11];
        res.mat[8] = m.mat[5] * m.mat[11] * m.mat[12] - m.mat[7] * m.mat[9] * m.mat[12] + m.mat[7] * m.mat[8] * m.mat[13] - m.mat[4] * m.mat[11] * m.mat[13] - m.mat[5] * m.mat[8] * m.mat[15] + m.mat[4] * m.mat[9] * m.mat[15];
        res.mat[9] = m.mat[3] * m.mat[9] * m.mat[12] - m.mat[1] * m.mat[11] * m.mat[12] - m.mat[3] * m.mat[8] * m.mat[13] + m.mat[0] * m.mat[11] * m.mat[13] + m.mat[1] * m.mat[8] * m.mat[15] - m.mat[0] * m.mat[9] * m.mat[15];
        res.mat[10] = m.mat[1] * m.mat[7] * m.mat[12] - m.mat[3] * m.mat[5] * m.mat[12] + m.mat[3] * m.mat[4] * m.mat[13] - m.mat[0] * m.mat[7] * m.mat[13] - m.mat[1] * m.mat[4] * m.mat[15] + m.mat[0] * m.mat[5] * m.mat[15];
        res.mat[11] = m.mat[3] * m.mat[5] * m.mat[8] - m.mat[1] * m.mat[7] * m.mat[8] - m.mat[3] * m.mat[4] * m.mat[9] + m.mat[0] * m.mat[7] * m.mat[9] + m.mat[1] * m.mat[4] * m.mat[11] - m.mat[0] * m.mat[5] * m.mat[11];
        res.mat[12] = m.mat[6] * m.mat[9] * m.mat[12] - m.mat[5] * m.mat[10] * m.mat[12] - m.mat[6] * m.mat[8] * m.mat[13] + m.mat[4] * m.mat[10] * m.mat[13] + m.mat[5] * m.mat[8] * m.mat[14] - m.mat[4] * m.mat[9] * m.mat[14];
        res.mat[13] = m.mat[1] * m.mat[10] * m.mat[12] - m.mat[2] * m.mat[9] * m.mat[12] + m.mat[2] * m.mat[8] * m.mat[13] - m.mat[0] * m.mat[10] * m.mat[13] - m.mat[1] * m.mat[8] * m.mat[14] + m.mat[0] * m.mat[9] * m.mat[14];
        res.mat[14] = m.mat[2] * m.mat[5] * m.mat[12] - m.mat[1] * m.mat[6] * m.mat[12] - m.mat[2] * m.mat[4] * m.mat[13] + m.mat[0] * m.mat[6] * m.mat[13] + m.mat[1] * m.mat[4] * m.mat[14] - m.mat[0] * m.mat[5] * m.mat[14];
        res.mat[15] = m.mat[1] * m.mat[6] * m.mat[8] - m.mat[2] * m.mat[5] * m.mat[8] + m.mat[2] * m.mat[4] * m.mat[9] - m.mat[0] * m.mat[6] * m.mat[9] - m.mat[1] * m.mat[4] * m.mat[10] + m.mat[0] * m.mat[5] * m.mat[10];

        res = res * ( 1 / determinant( res ) );
        return res;
    }

    float determinant( const mat4 & m ){
        float value;
        value =
            m.mat[3] * m.mat[6] * m.mat[9] * m.mat[12] - m.mat[2] * m.mat[7] * m.mat[9] * m.mat[12] - m.mat[3] * m.mat[5] * m.mat[10] * m.mat[12] + m.mat[1] * m.mat[7] * m.mat[10] * m.mat[12] +
            m.mat[2] * m.mat[5] * m.mat[11] * m.mat[12] - m.mat[1] * m.mat[6] * m.mat[11] * m.mat[12] - m.mat[3] * m.mat[6] * m.mat[8] * m.mat[13] + m.mat[2] * m.mat[7] * m.mat[8] * m.mat[13] +
            m.mat[3] * m.mat[4] * m.mat[10] * m.mat[13] - m.mat[0] * m.mat[7] * m.mat[10] * m.mat[13] - m.mat[2] * m.mat[4] * m.mat[11] * m.mat[13] + m.mat[0] * m.mat[6] * m.mat[11] * m.mat[13] +
            m.mat[3] * m.mat[5] * m.mat[8] * m.mat[14] - m.mat[1] * m.mat[7] * m.mat[8] * m.mat[14] - m.mat[3] * m.mat[4] * m.mat[9] * m.mat[14] + m.mat[0] * m.mat[7] * m.mat[9] * m.mat[14] +
            m.mat[1] * m.mat[4] * m.mat[11] * m.mat[14] - m.mat[0] * m.mat[5] * m.mat[11] * m.mat[14] - m.mat[2] * m.mat[5] * m.mat[8] * m.mat[15] + m.mat[1] * m.mat[6] * m.mat[8] * m.mat[15] +
            m.mat[2] * m.mat[4] * m.mat[9] * m.mat[15] - m.mat[0] * m.mat[6] * m.mat[9] * m.mat[15] - m.mat[1] * m.mat[4] * m.mat[10] * m.mat[15] + m.mat[0] * m.mat[5] * m.mat[10] * m.mat[15];
        return value;
    }

    std::ostream & operator<<( std::ostream & out, const mat4 & m ){
        out << std::endl;
        out << "| " << m.mat[0] << "  " << m.mat[1] << "  " << m.mat[2] << "  " << m.mat[3] << " |" << std::endl;
        out << "| " << m.mat[4] << "  " << m.mat[5] << "  " << m.mat[6] << "  " << m.mat[7] << " |" << std::endl;
        out << "| " << m.mat[8] << "  " << m.mat[9] << "  " << m.mat[10] << "  " << m.mat[11] << " |" << std::endl;
        out << "| " << m.mat[12] << "  " << m.mat[13] << "  " << m.mat[14] << "  " << m.mat[15] << " |" << std::endl;
        return out;
    }

    std::istream & operator>>( std::istream & in, mat4 & m ){
        in >> m.mat[0] >> m.mat[1] >> m.mat[2] >> m.mat[3]
            >> m.mat[4] >> m.mat[5] >> m.mat[6] >> m.mat[7]
            >> m.mat[8] >> m.mat[9] >> m.mat[10] >> m.mat[11]
            >> m.mat[12] >> m.mat[13] >> m.mat[14] >> m.mat[15];
        return in;
    }


    // MATRIX FACTORY 2 ///////////////////////////

    const mat2 MatrixFactory::createIdentityMatrix2(){
        return mat2( 1 );
    }

    const mat2 MatrixFactory::createScaleMatrix2( const float sx, const float sy ){
        mat2 ret = mat2( 1 );
        ret.mat[0] = sx;
        ret.mat[3] = sy;
        return mat2( 1 );
    }

    // MATRIX FACTORY 3 ///////////////////////////

    const mat3 MatrixFactory::createIdentityMatrix3(){
        return mat3( 1 );
    }
    const mat3 MatrixFactory::createRotationMatrix3( const float angle, const vec3 & v ){
        float rad = angle * DEGREES_TO_RADIANS;
        vec3 a = ( 1 / v.length() ) * v;
        mat3 A = engine::MatrixFactory::createDualMatrix( a );
        mat3 A2 = engine::MatrixFactory::createSquareDualMatrix( a );
        mat3 R = engine::MatrixFactory::createIdentityMatrix3() + sin( rad ) * A + ( 1 - cos( rad ) ) * A2;
        return R;
    }

    const mat3 MatrixFactory::createDualMatrix( const vec3& v ){
        return mat3( 0, -v.z, v.y,
                     v.z, 0, -v.x,
                     -v.y, v.x, 0 );
    }

    const mat3 MatrixFactory::createSquareDualMatrix( const vec3 & v ){
        return mat3( ( -v.z*v.z ) - v.y*v.y, v.x*v.y, v.x * v.z,
                     v.x*v.y, ( -v.z*v.z ) - v.x*v.x, v.y * v.z,
                     v.x*v.z, v.y * v.z, ( -v.y*v.y ) - v.x*v.x );
    }

    const mat3 MatrixFactory::createScaleMatrix3( const float x, const float y, const float z ){
        mat3 ret = mat3( 1 );
        ret.mat[0] = x;
        ret.mat[4] = y;
        ret.mat[8] = z;
        return ret;
    }

    const mat3 MatrixFactory::transformMatrix4to3( const mat4 & m ){
        return mat3( m );
    }

    const mat3 MatrixFactory::createNormalMatrix( const mat4 & m ){
        mat3 M = engine::MatrixFactory::transformMatrix4to3( m );
        mat3 Q = inverse( M );
        Q = Q.transpose();
        return Q;
    }


    // MATRIX FACTORY 4 ///////////////////////////


    const mat4 MatrixFactory::createIdentityMatrix4(){
        return mat4( 1 );
    }

    const mat4 MatrixFactory::createRotationMatrix4( const float angle, const vec4 & v ){
        return MatrixFactory::transformMatrix3to4( MatrixFactory::createRotationMatrix3( angle, vec3( v ) ) );
    }

    const mat4 MatrixFactory::createTranslationMatrix( const float x, const float y, const float z ){
        mat4 res = MatrixFactory::createIdentityMatrix4();
        res.mat[3] = x;
        res.mat[7] = y;
        res.mat[11] = z;

        return res;
    }

    const mat4 MatrixFactory::createTranslationMatrix( const vec3& v ){
        mat4 res = MatrixFactory::createIdentityMatrix4();
        res.mat[3] = v.x;
        res.mat[7] = v.y;
        res.mat[11] = v.z;

        return res;
    }

    const mat4 MatrixFactory::createScaleMatrix4( const float x, const float y, const float z ){
        return  MatrixFactory::transformMatrix3to4( MatrixFactory::createScaleMatrix3( x, y, z ) );
    }

    const mat4 MatrixFactory::transformMatrix3to4( const mat3 & m ){
        return  mat4( m );
    }

    const mat4 MatrixFactory::createLookAt( const vec3 eye, const vec3 v, const vec3 u, const vec3 s ){
        mat4 res = mat4( s.x, s.y, s.z, -s.dot( eye ),
                         u.x, u.y, u.z, -u.dot( eye ),
                         -v.x, -v.y, -v.z, v.dot( eye ),
                         0, 0, 0, 1 );
        res.clean();
        return res;
    }

    const mat4 MatrixFactory::createLookAt( const vec3 eye, const vec3 center, const vec3 up ){
        vec3 v = center - eye;
        v = v * ( 1 / v.length() ); // normalize
        vec3 s = v.cross( up );
        s = s * ( 1 / s.length() );  // normalize
        vec3 u = s.cross( v ); //is a unit vector only because s and v are perpendicular to each other and are unit vectors |s x v| = |s||v|*sin(angle)

        mat4 res = mat4( s.x, s.y, s.z, -s.dot( eye ),
                         u.x, u.y, u.z, -u.dot( eye ),
                         -v.x, -v.y, -v.z, v.dot( eye ),
                         0, 0, 0, 1 );
        res.clean();
        return res;
    }

    const mat4 MatrixFactory::createOrtographicProjectionMatrix( const float left, const float right, const float bottom, const float top, const float near, const float far ){
        mat4 res = mat4( 2 / ( right - left ), 0, 0, ( left + right ) / ( left - right ),
                         0, 2 / ( top - bottom ), 0, ( top + bottom ) / ( bottom - top ),
                         0, 0, -2 / ( far - near ), ( far + near ) / ( near - far ),
                         0, 0, 0, 1 );
        res.clean();
        return res;
    }

    const mat4 MatrixFactory::createPerspectiveProjectionMatrix( const float fovY, const float aspect, const float near, const float far ){
        const float PI = 3.14159265f;
        float angle = fovY / 2 * ( PI / 180 );
        float d = 1 / tan( angle );

        mat4 res = mat4( d / aspect, 0, 0, 0,
                         0, d, 0, 0,
                         0, 0, ( near + far ) / ( near - far ), ( 2 * far * near ) / ( near - far ),
                         0, 0, -1, 0 );
        res.clean();
        return res;
    }

}
