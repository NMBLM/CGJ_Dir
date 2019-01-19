#include <iostream>
#include <sstream>
#include <string>
#include <map>


#include "vector.h"
#include "Matrix.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "KeyBuffer.h"
#include "Mesh.h"
#include "MeshLoader.h"
#include "Scene.h"
#include "Catalog.h"
#include "Texture.h"
#include "Particle.h"
#include  "TextureInfo.h"
#include "LightSource.h"

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "FrameBuffer.h"
#include <random>

#define CAPTION "CyberNoodles"
#define NR_POINT_LIGHTS 11
#define MODES_NUM 12

using namespace engine;

int WinX = 1280, WinY = 720;
int lastWinX = WinX, lastWinY = WinY;
int WindowHandle = 0;
unsigned int FrameCount = 0;

const GLuint UBO_BP = 0;

const float PI = 3.14159265f;

FixedCamera* camera;
Camera* freeCamera;

MeshLoader meshLoader;

PointLight pointLights[NR_POINT_LIGHTS];
Scene* scene;

Scene* deferredScene;

ParticleSystemTransform* particlesOne;
ParticleSystemTransform* deferredParticles;

float lastFrame = 0.0f;
float delta = 0.0f;
int lastMouseY = WinX / 2;
int lastMouseX = WinY / 2;
float k = 0.0f;

bool freecam = false;
bool deferred = true;

float exposure = 1.0f;
float gamma = 2.2f;

int mode = 0;

mat4 projectionMatrix = MatrixFactory::createPerspectiveProjectionMatrix( 30, ( float )WinX / ( float )WinY, 1, 30 );
mat4 otherProjectionMatrix = MatrixFactory::createOrtographicProjectionMatrix( -2, 2, -2, 2, 1, 30 );

unsigned int hdrFBO;
unsigned int colorBuffers[2];
unsigned int colorBuffer;

unsigned int pingpongFBO[2];
unsigned int pingpongColorbuffers[2];

unsigned int rboDepth;

unsigned int reflectionBuffer;

vec4 reflectionPlane;

unsigned int ssaoFBO, ssaoBlurFBO;
unsigned int ssaoColorBuffer, ssaoColorBufferBlur;

unsigned int noiseTexture;
std::vector<vec3> ssaoKernel;

unsigned int gBuffer, gDepthBuffer;
unsigned int gPosition, gNormal, gAlbedoSpec, gBright;

unsigned int quadVAO = 0;
unsigned int quadVBO;

vec4 YY = vec4( 0, 1, 0, 1 );
vec4 ZZ = vec4( 0, 0, 1, 1 );

//CATALOGS
Catalog<Texture*>* TextureCat = Catalog<Texture*>::instance();
Catalog<ShaderProgram*> *ShaderProgramCat = Catalog<ShaderProgram*>::instance();

const int MAX_SAMPLES = 128;
int occlusionSamples = 64;
