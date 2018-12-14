#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Vector.h"
#include "Matrix.h"
#include "ShaderProgram.h"

#include "GL/glew.h"

#define VERTICES 0
#define TEXCOORDS 1
#define NORMALS 2
#define TANGENTS 3
#define BI_TANGENTS 4

namespace engine{

    class Mesh{
        
        std::vector <vec3> Vertices;
        std::vector <vec2> Texcoords;
        std::vector <vec3> Normals;
        std::vector <vec3> Tangents;
        std::vector <vec3> BiTangents;
        
        std::vector <unsigned int> vertexIdx, texcoordIdx, normalIdx;

        public:
        static constexpr const char* const TABLE = "TABLE";
        static constexpr const char* const GROUND = "GROUND";
        static constexpr const char* const TRIANGLE = "TRIANGLE";
        static constexpr const char* const SQUARE = "SQUARE";
        static constexpr const char* const PARALLELOGRAM = "PARALLELOGRAM";
        static constexpr const char* const DUCK = "DUCK";
        static constexpr const char* const QUAD = "QUAD";
        static constexpr const char* const CUBE = "CUBE";
        static constexpr const char* const SPHERE = "SPHERE";

        bool TexcoordsLoaded, NormalsLoaded;
        GLuint VaoId;
        Mesh();
        Mesh( std::vector<vec3>vertices,
            std::vector <vec2> texcoords,
            std::vector <vec3> normals,
            bool tcLoaded, bool nLoaded );
        
        void createBufferObjects();
        void calculateTangentSpace();
        void destroyBufferObjects();
        void draw();

    };

}