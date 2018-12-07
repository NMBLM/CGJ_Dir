#include "LightSource.h"

using namespace engine;

PointLight::PointLight(){
    //default light is red light at the center of everything
    //with infinte range
    position = vec3( 0.0f );

    //no fall off
    constant = 1.0f;
    linear = 0.0f;
    quadratic = 0.0f;

    //each value specifies how much it affects of each r,g,b value
    //only red
    ambient = vec3( 1.0f, 0.0f, 0.0f );
    diffuse = vec3( 1.0f, 0.0f, 0.0f );
    specular = vec3( 1.0f, 0.0f, 0.0f );
}

PointLight::PointLight( vec3 pos, float constantDrop, float linearDrop,
    float quadraticDrop, vec3 ambientColor, vec3 diffuseColor, vec3 specularColor ){
    position = pos;

    constant = constantDrop;
    linear = linearDrop;
    quadratic = quadraticDrop;

    ambient = ambientColor;
    diffuse = diffuseColor;
    specular = specularColor;
}

PointLight::PointLight( vec3 pos, vec3 dropoff,
    vec3 ambientColor, vec3 diffuseColor, vec3 specularColor ){
    position = pos;

    constant = dropoff.x;
    linear = dropoff.y;
    quadratic = dropoff.z;

    ambient = ambientColor;
    diffuse = diffuseColor;
    specular = specularColor;
}

void PointLight::addItself( ShaderProgram * shader, unsigned int i ){

    SetLightUniform( shader, "position", i, position );
    SetLightUniform( shader, "constant", i, constant );
    SetLightUniform( shader, "linear", i, linear );
    SetLightUniform( shader, "quadratic", i, quadratic );
    SetLightUniform( shader, "ambient", i, ambient );
    SetLightUniform( shader, "diffuse", i, diffuse );
    SetLightUniform( shader, "specular", i, specular );

}

template<typename T>
inline void PointLight::SetLightUniform( ShaderProgram * shader, const char * propertyName, size_t lightIndex, const T & value ){
    std::ostringstream ss;
    ss << "pointLights[" << lightIndex << "]." << propertyName;
    std::string uniformName = ss.str();
    //std::cout << uniformName << " " << value <<std::endl;
    shader->addUniform( uniformName.c_str(), value );

}