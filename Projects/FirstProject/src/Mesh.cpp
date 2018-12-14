#include "Mesh.h"
using namespace engine;


Mesh::Mesh() {

}

Mesh::Mesh(std::vector<vec3> vertices,
           std::vector<vec2> texcoords,
           std::vector<vec3> normals,
           bool tcLoaded, bool nLoaded) {

    this->Vertices = std::vector<vec3>(vertices);
    this->Texcoords = std::vector<vec2>(texcoords);
    this->Normals = std::vector<vec3>(normals);
    this->TexcoordsLoaded = tcLoaded;
    this->NormalsLoaded = nLoaded;
    if (tcLoaded && nLoaded) {
        calculateTangentSpace();
    }

    createBufferObjects();
}


void engine::Mesh::calculateTangentSpace() {
    //TODO calculate tangents

    for (int i = 0; i < Vertices.size(); i += 3) {

        // Shortcuts for vertices
        vec3 & v0 = Vertices[i + 0];
        vec3 & v1 = Vertices[i + 1];
        vec3 & v2 = Vertices[i + 2];

        // Shortcuts for UVs
        vec2 & uv0 = Texcoords[i + 0];
        vec2 & uv1 = Texcoords[i + 1];
        vec2 & uv2 = Texcoords[i + 2];

        // Edges of the triangle : position delta
        vec3 deltaPos1 = v1 - v0;
        vec3 deltaPos2 = v2 - v0;

        // UV delta
        vec2 deltaUV1 = uv1 - uv0;
        vec2 deltaUV2 = uv2 - uv0;

        float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
        vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;
        vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x)*r;

        Tangents.push_back(tangent);
        Tangents.push_back(tangent);
        Tangents.push_back(tangent);

        // Same thing for bitangents
        BiTangents.push_back(bitangent);
        BiTangents.push_back(bitangent);
        BiTangents.push_back(bitangent);

    }
}

/////////////////////////////////////////////////////////////////////// VAOs & VBOs

void Mesh::createBufferObjects() {
    GLuint VboVertices, VboTexcoords, VboNormals, VboTangents, VboBiTangents;

    glGenVertexArrays(1, &VaoId);
    glBindVertexArray(VaoId);
    {
        glGenBuffers(1, &VboVertices);
        glBindBuffer(GL_ARRAY_BUFFER, VboVertices);
        glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(vec3), &Vertices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(VERTICES);
        glVertexAttribPointer(VERTICES, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);

        if (TexcoordsLoaded) {
            glGenBuffers(1, &VboTexcoords);
            glBindBuffer(GL_ARRAY_BUFFER, VboTexcoords);
            glBufferData(GL_ARRAY_BUFFER, Texcoords.size() * sizeof(vec2), &Texcoords[0], GL_STATIC_DRAW);
            glEnableVertexAttribArray(TEXCOORDS);
            glVertexAttribPointer(TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), 0);
        }
        if (NormalsLoaded) {
            glGenBuffers(1, &VboNormals);
            glBindBuffer(GL_ARRAY_BUFFER, VboNormals);
            glBufferData(GL_ARRAY_BUFFER, Normals.size() * sizeof(vec3), &Normals[0], GL_STATIC_DRAW);
            glEnableVertexAttribArray(NORMALS);
            glVertexAttribPointer(NORMALS, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);
        }

        glGenBuffers(1, &VboTangents);
        glBindBuffer(GL_ARRAY_BUFFER, VboTangents);
        glBufferData(GL_ARRAY_BUFFER, Tangents.size() * sizeof(vec3), &Tangents[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(TANGENTS);
        glVertexAttribPointer(TANGENTS, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);

        glGenBuffers(1, &VboBiTangents);
        glBindBuffer(GL_ARRAY_BUFFER, VboBiTangents);
        glBufferData(GL_ARRAY_BUFFER, BiTangents.size() * sizeof(vec3), &BiTangents[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(BI_TANGENTS);
        glVertexAttribPointer(BI_TANGENTS, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);
        
    }
    //TODO adde tangents do buffer
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &VboVertices);
    glDeleteBuffers(1, &VboTexcoords);
    glDeleteBuffers(1, &VboNormals);
}

void Mesh::destroyBufferObjects() {
    glBindVertexArray(VaoId);
    glDisableVertexAttribArray(VERTICES);
    glDisableVertexAttribArray(TEXCOORDS);
    glDisableVertexAttribArray(NORMALS);
    glDeleteVertexArrays(1, &VaoId);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void engine::Mesh::draw() {
    glBindVertexArray(VaoId);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)Vertices.size());
    glBindVertexArray(0);
}

