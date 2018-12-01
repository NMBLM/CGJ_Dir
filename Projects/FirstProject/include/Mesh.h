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

namespace engine{

    class Mesh{


        std::vector <vec3> Vertices;
        std::vector <vec2> Texcoords;
        std::vector <vec3> Normals;
        std::vector <unsigned int> vertexIdx, texcoordIdx, normalIdx;

        public:
        bool TexcoordsLoaded, NormalsLoaded;
        GLuint VaoId;
        Mesh();
        Mesh( std::vector<vec3>vertices,
            std::vector <vec2> texcoords,
            std::vector <vec3> normals,
            bool tcLoaded, bool nLoaded );
        void createBufferObjects();
        void destroyBufferObjects();
        void draw();

    };

}