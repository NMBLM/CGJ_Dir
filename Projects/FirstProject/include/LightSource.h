#pragma once
#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "Vector.h"
#include "ShaderProgram.h"

namespace engine{
    struct DirectionalLight;
    struct PointLight;
    struct SpotLight;

    struct PointLight{
        vec3 position;

        float constant;
        float linear;
        float quadratic;

        vec3 ambient;
        vec3 diffuse;
        vec3 specular;   

        PointLight();
        PointLight( vec3 pos, float constantDrop, float linearDrop, float quadraticDrop,
            vec3 ambientColor, vec3 diffuseColor, vec3 specularColor );
        PointLight( vec3 pos, vec3 dropoff, vec3 ambientColor, vec3 diffuseColor,
            vec3 specularColor );

        void addItself( ShaderProgram * shader, unsigned int i );

        template <typename T>
        void SetLightUniform( ShaderProgram* shader, const char* propertyName, size_t lightIndex, const T& value );


    };

    struct SpotLight{
        vec4 position;
        vec3 intensities;
        float attenuation;
        float ambientCoefficient;
        float coneAngle;    // new
        vec3 coneDirection; // new
    };



}

#endif