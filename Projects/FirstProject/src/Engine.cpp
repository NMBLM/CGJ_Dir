#include <iostream>
#include <sstream>
#include <string>
#include <map>


#include "vector.h"
#include "matrix.h"
#include "ShaderProgram.h"
#include "camera.h"
#include "KeyBuffer.h"
#include "Mesh.h"
#include "MeshLoader.h"
#include "Scene.h"
#include "Catalog.h"
#include "Texture.h"
#include "Particle.h"
#include "LightSource.h"

#include "GL/glew.h"
#include "GL/freeglut.h"

#define CAPTION "Hello Modern 3D World"
#define NR_NEON_LIGHTS 5
#define NR_POINT_LIGHTS 3 + NR_NEON_LIGHTS


using namespace engine;

int WinX = 640, WinY = 640;
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
//ParticleSystem* particlesOne;
ParticleSystemTransform* particlesTwo;

float lastFrame = 0.0f;
float delta = 0.0f;
int lastMouseY = WinX / 2;
int lastMouseX = WinY / 2;
float k = 0.0f;
bool freecam = false;

mat4 projectionMatrix = MatrixFactory::createPerspectiveProjectionMatrix( 30, ( float )WinX / ( float )WinY, 1, 30 );
mat4 otherProjectionMatrix = MatrixFactory::createOrtographicProjectionMatrix( -2, 2, -2, 2, 1, 30 );

SceneNode  *table;

/////////////////////////////////////////////////////////////////////// ERRORS

static std::string errorType( GLenum type ){
    switch( type ){
        case GL_DEBUG_TYPE_ERROR:				return "error";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	return "deprecated behavior";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	return "undefined behavior";
        case GL_DEBUG_TYPE_PORTABILITY:			return "portability issue";
        case GL_DEBUG_TYPE_PERFORMANCE:			return "performance issue";
        case GL_DEBUG_TYPE_MARKER:				return "stream annotation";
        case GL_DEBUG_TYPE_OTHER_ARB:			return "other";
        default:								exit( EXIT_FAILURE );
    }
}

static std::string errorSource( GLenum source ){
    switch( source ){
        case GL_DEBUG_SOURCE_API:				return "API";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		return "window system";
        case GL_DEBUG_SOURCE_SHADER_COMPILER:	return "shader compiler";
        case GL_DEBUG_SOURCE_THIRD_PARTY:		return "third party";
        case GL_DEBUG_SOURCE_APPLICATION:		return "application";
        case GL_DEBUG_SOURCE_OTHER:				return "other";
        default:								exit( EXIT_FAILURE );
    }
}

static std::string errorSeverity( GLenum severity ){
    switch( severity ){
        case GL_DEBUG_SEVERITY_HIGH:			return "high";
        case GL_DEBUG_SEVERITY_MEDIUM:			return "medium";
        case GL_DEBUG_SEVERITY_LOW:				return "low";
        case GL_DEBUG_SEVERITY_NOTIFICATION:	return "notification";
        default:								exit( EXIT_FAILURE );
    }
}

static void error( GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
    const GLchar *message, const void *userParam ){
    std::cerr << "ERROR:" << std::endl;
    std::cerr << "  source:     " << errorSource( source ) << std::endl;
    std::cerr << "  type:       " << errorType( type ) << std::endl;
    std::cerr << "  severity:   " << errorSeverity( severity ) << std::endl;
    std::cerr << "  debug call: " << std::endl << message << std::endl << std::endl;
}

void setupErrors(){
    glEnable( GL_DEBUG_OUTPUT );
    glDebugMessageCallback( error, 0 );
    glDebugMessageControl( GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, GL_TRUE );
    // params: source, type, severity, count, ids, enabled
}

void mouse_wheel_input( int button, int dir, int x, int y ){
    if( !freecam ){
        camera->zoom( dir, delta );
    }
}

void mouse_input( int x, int y ){

    int xoffset = lastMouseX - x;
    int yoffset = lastMouseY - y;
    lastMouseX = x;
    lastMouseY = y;
    float sen = 0.25f;
    if( freecam ){
        freeCamera->cameraLookAround( xoffset * sen, yoffset * sen, delta );
    } else{
        camera->cameraLookAround( xoffset * sen, yoffset * sen, delta );
    }

}

void keyPress( unsigned char key, int x, int y ){
    KeyBuffer::instance()->pressKey( key );
}

void keyRelease( unsigned char key, int x, int y ){
    if( KeyBuffer::instance()->isKeyDown( 'g' ) || KeyBuffer::instance()->isKeyDown( 'G' ) ) camera->gimbalLockSwitch();
    if( KeyBuffer::instance()->isKeyDown( 'p' ) || KeyBuffer::instance()->isKeyDown( 'P' ) ){
        mat4 temp = projectionMatrix;
        projectionMatrix = otherProjectionMatrix;
        otherProjectionMatrix = temp;
        camera->ProjectionMatrix( projectionMatrix );
    }

    if( KeyBuffer::instance()->isKeyDown( 'f' ) || KeyBuffer::instance()->isKeyDown( 'F' ) ){
        if( freecam ){
            scene->setCamera( camera );
            //particlesOne->camera = camera;
            particlesTwo->camera = camera;

        } else{
            scene->setCamera( freeCamera );
            //particlesOne->camera = freeCamera;
            particlesTwo->camera = freeCamera;

        }
        freecam = !freecam;
    }
    if( KeyBuffer::instance()->isKeyDown( 'r' ) ) scene->actOnAnimator();
    if( KeyBuffer::instance()->isKeyDown( 'R' ) ) scene->actOnAnimator();
    KeyBuffer::instance()->releaseKey( key );

}

void specialKeyPress( int key, int x, int y ){
    KeyBuffer::instance()->pressSpecialKey( key );
}

void specialKeyRelease( int key, int x, int y ){
    KeyBuffer::instance()->releaseSpecialKey( key );
}

void update(){
    //FixedCamera
    if( !freecam ){
        if( KeyBuffer::instance()->isKeyDown( 'a' ) ) camera->cameraMoveLeft( delta );
        if( KeyBuffer::instance()->isKeyDown( 'A' ) ) camera->cameraMoveLeft( delta );

        if( KeyBuffer::instance()->isKeyDown( 'd' ) ) camera->cameraMoveRight( delta );
        if( KeyBuffer::instance()->isKeyDown( 'D' ) ) camera->cameraMoveRight( delta );

        if( KeyBuffer::instance()->isKeyDown( 's' ) ) camera->cameraMoveBack( delta );
        if( KeyBuffer::instance()->isKeyDown( 'S' ) ) camera->cameraMoveBack( delta );

        if( KeyBuffer::instance()->isKeyDown( 'w' ) ) camera->cameraMoveForward( delta );
        if( KeyBuffer::instance()->isKeyDown( 'W' ) ) camera->cameraMoveForward( delta );

        if( KeyBuffer::instance()->isKeyDown( 'q' ) ) camera->cameraRollLeft( delta );
        if( KeyBuffer::instance()->isKeyDown( 'Q' ) ) camera->cameraRollLeft( delta );

        if( KeyBuffer::instance()->isKeyDown( 'e' ) ) camera->cameraRollRight( delta );
        if( KeyBuffer::instance()->isKeyDown( 'E' ) ) camera->cameraRollRight( delta );
    }

    //FreeCamera
    if( freecam ){
        if( KeyBuffer::instance()->isKeyDown( 'a' ) ) freeCamera->cameraMoveLeft( delta );
        if( KeyBuffer::instance()->isKeyDown( 'A' ) ) freeCamera->cameraMoveLeft( delta );

        if( KeyBuffer::instance()->isKeyDown( 'd' ) ) freeCamera->cameraMoveRight( delta );
        if( KeyBuffer::instance()->isKeyDown( 'D' ) ) freeCamera->cameraMoveRight( delta );

        if( KeyBuffer::instance()->isKeyDown( 's' ) ) freeCamera->cameraMoveBack( delta );
        if( KeyBuffer::instance()->isKeyDown( 'S' ) ) freeCamera->cameraMoveBack( delta );

        if( KeyBuffer::instance()->isKeyDown( 'w' ) ) freeCamera->cameraMoveForward( delta );
        if( KeyBuffer::instance()->isKeyDown( 'W' ) ) freeCamera->cameraMoveForward( delta );
    }

    if( KeyBuffer::instance()->isSpecialKeyDown( GLUT_KEY_LEFT ) ){
        table->updateModel( MatrixFactory::createTranslationMatrix( -1.0f * delta, 0.0f, 0.0f ) );
    }
    if( KeyBuffer::instance()->isSpecialKeyDown( GLUT_KEY_UP ) ){
        table->updateModel( MatrixFactory::createTranslationMatrix( 0.0f, 0.0f, -1.0f * delta ) );
    }
    if( KeyBuffer::instance()->isSpecialKeyDown( GLUT_KEY_RIGHT ) ){
        table->updateModel( MatrixFactory::createTranslationMatrix( 1.0f * delta, 0.0f, 0.0f ) );
    }
    if( KeyBuffer::instance()->isSpecialKeyDown( GLUT_KEY_DOWN ) ){
        table->updateModel( MatrixFactory::createTranslationMatrix( 0.0f, 0.0f, 1.0f * delta ) );
    }

}


static bool isOpenGLError(){
    bool isError = false;
    GLenum errCode;
    const GLubyte *errString;
    while( ( errCode = glGetError() ) != GL_NO_ERROR ){
        isError = true;
        errString = gluErrorString( errCode );
        std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
    }
    return isError;
}

static void checkOpenGLError( std::string error ){
    if( isOpenGLError() ){
        std::cerr << error << std::endl;
        exit( EXIT_FAILURE );
    }
}


/////////////////////////////////////////////////////////////////////// SHADERs

void createShaderProgram(){
    // DEFAULT SHADER
    Catalog<ShaderProgram*> *shaderProgramManager = Catalog<ShaderProgram*>::instance();
    ShaderProgram* dfault;
    ShaderProgram* prog;
    dfault = new ShaderProgram();
    dfault->attachShader( GL_VERTEX_SHADER, "vertex", "Shaders/default_vs.glsl" );
    dfault->attachShader( GL_FRAGMENT_SHADER, "fragment", "Shaders/default_fs.glsl" );

    dfault->bindAttribLocation( VERTICES, "inPosition" );
    dfault->link();

    dfault->detachShader( "vertex" );
    dfault->detachShader( "fragment" );

    shaderProgramManager->insert( "default", dfault );
    checkOpenGLError( "ERROR: Could not create default shaders." );

    // Non default
    prog = new ShaderProgram();
    prog->attachShader( GL_VERTEX_SHADER, "vertex", "Shaders/force_color_vs.glsl" );
    prog->attachShader( GL_FRAGMENT_SHADER, "fragment", "Shaders/force_color_rcv_fs.glsl" );


    prog->bindAttribLocation( VERTICES, "inPosition" );
    prog->bindAttribLocation( TEXCOORDS, "inTexcoord" );
    prog->bindAttribLocation( NORMALS, "inNormal" );

    prog->link();

    prog->detachShader( "vertex" );
    prog->detachShader( "fragment" );

    shaderProgramManager->insert( "ColorProgram", prog );

    prog = new ShaderProgram();
    prog->attachShader( GL_VERTEX_SHADER, "vertex", "Shaders/texShader_color_vs.glsl" );
    prog->attachShader( GL_FRAGMENT_SHADER, "fragment", "Shaders/texShader_color_fs.glsl" );

    prog->bindAttribLocation( VERTICES, "inPosition" );
    prog->bindAttribLocation( TEXCOORDS, "inTexcoord" );
    prog->bindAttribLocation( NORMALS, "inNormal" );

    prog->link();

    prog->detachShader( "vertex" );
    prog->detachShader( "fragment" );

    shaderProgramManager->insert( "ColorTextureProgram", prog );


    //PartTransformProgram
    prog = new ShaderProgram();
    prog->attachShader( GL_GEOMETRY_SHADER, "geometry", "Shaders/tfb_billboard_gs.glsl" );
    prog->attachShader( GL_VERTEX_SHADER, "vertex", "Shaders/tfb_billboard_vs.glsl" );
    prog->attachShader( GL_FRAGMENT_SHADER, "fragment", "Shaders/tfb_billboard_fs.glsl" );

    prog->link();

    prog->detachShader( "geometry" );
    prog->detachShader( "vertex" );
    prog->detachShader( "fragment" );

    shaderProgramManager->insert( "TFBDraw", prog );
    prog = new ShaderProgram();
    prog->attachShader( GL_GEOMETRY_SHADER, "geometry", "Shaders/tfb_gs.glsl" );
    prog->attachShader( GL_VERTEX_SHADER, "vertex", "Shaders/tfb_vs.glsl" );
    prog->attachShader( GL_FRAGMENT_SHADER, "fragment", "Shaders/tfb_fs.glsl" );

    const GLchar* Varyings[4];
    Varyings[0] = "Position1";
    Varyings[1] = "Velocity1";
    Varyings[2] = "Life1";
    Varyings[3] = "Type1";

    glTransformFeedbackVaryings( prog->id, 4, Varyings, GL_INTERLEAVED_ATTRIBS );
    prog->link();

    prog->detachShader( "geometry" );
    prog->detachShader( "vertex" );
    prog->detachShader( "fragment" );

    shaderProgramManager->insert( "TFBUpdate", prog );
    checkOpenGLError( "ERROR: Could not create Transform shaders." );
    checkOpenGLError( "ERROR: Could not create shaders." );

}
void destroyShaderProgram(){
    Catalog<ShaderProgram*> *shaderProgramManager = Catalog<ShaderProgram*>::instance();

    glUseProgram( 0 );
    glDeleteProgram( shaderProgramManager->get( "default" )->id );
    glDeleteProgram( shaderProgramManager->get( "ColorProgram" )->id );

    checkOpenGLError( "ERROR: Could not destroy shaders." );
}

/////////////////////////////////////////////////////////////////////// VAOs & VBOs

void createBufferObjects(){
    checkOpenGLError( "ERROR: Could not create VAOs, VBOs and UBOs." );
}

void destroyBufferObjects(){
    checkOpenGLError( "ERROR: Could not destroy VAOs and VBOs." );
}

/////////////////////////////////////////////////////////////////////// SCENE

void drawScene(){

    //scene->draw();
    particlesTwo->draw();
    checkOpenGLError( "ERROR: Could not draw scene." );
}

/////////////////////////////////////////////////////////////////////// CALLBACKS

void cleanup(){
    destroyShaderProgram();
    destroyBufferObjects();
}

void display(){
    ++FrameCount;
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    drawScene();
    glutSwapBuffers();
}

void idle(){
    update();
    float currentFrame = ( float )glutGet( GLUT_ELAPSED_TIME );
    delta = ( ( float )currentFrame - ( float )lastFrame ) / 100;
    lastFrame = ( float )currentFrame;
    scene->update( delta );
    particlesTwo->update( delta );

    glutPostRedisplay();
}

void reshape( int w, int h ){
    WinX = w;
    WinY = h;
    glViewport( 0, 0, WinX, WinY );
    if( lastWinX != WinX || lastWinY != WinY ){
        projectionMatrix = MatrixFactory::createPerspectiveProjectionMatrix( 30, ( float )WinX / ( float )WinY, 1, 30 );
        camera->ProjectionMatrix( projectionMatrix );
        freeCamera->ProjectionMatrix( projectionMatrix );
        lastWinX = WinX;
        lastWinY = WinY;
    }
}

void timer( int value ){
    std::ostringstream oss;
    oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")";
    std::string s = oss.str();
    glutSetWindow( WindowHandle );
    glutSetWindowTitle( s.c_str() );
    FrameCount = 0;
    glutTimerFunc( 1000, timer, 0 );
}

/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks(){
    glutCloseFunc( cleanup );
    glutDisplayFunc( display );
    glutIdleFunc( idle );
    glutKeyboardFunc( keyPress );
    glutKeyboardUpFunc( keyRelease );
    glutSpecialFunc( specialKeyPress );
    glutSpecialUpFunc( specialKeyRelease );
    glutMotionFunc( mouse_input );
    glutMouseWheelFunc( mouse_wheel_input );
    glutReshapeFunc( reshape );
    glutTimerFunc( 0, timer, 0 );
    setupErrors();
}

void checkOpenGLInfo(){
    const GLubyte *renderer = glGetString( GL_RENDERER );
    const GLubyte *vendor = glGetString( GL_VENDOR );
    const GLubyte *version = glGetString( GL_VERSION );
    const GLubyte *glslVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );
    std::cerr << "OpenGL Renderer: " << renderer << " (" << vendor << ")" << std::endl;
    std::cerr << "OpenGL version " << version << std::endl;
    std::cerr << "GLSL version " << glslVersion << std::endl;
}

void setupOpenGL(){
    checkOpenGLInfo();
    glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glDepthMask( GL_TRUE );
    glDepthRange( 0.0, 1.0 );
    glClearDepth( 1.0 );
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    glFrontFace( GL_CCW );
}

void setupGLEW(){
    glewExperimental = GL_TRUE;
    GLenum result = glewInit();
    if( result != GLEW_OK ){
        std::cerr << "ERROR glewInit: " << glewGetString( result ) << std::endl;
        exit( EXIT_FAILURE );
    }
    GLenum err_code = glGetError();
}

void setupGLUT( int argc, char* argv[] ){
    glutInit( &argc, argv );

    glutInitContextVersion( 3, 3 );
    glutInitContextProfile( GLUT_CORE_PROFILE );
    glutInitContextFlags( GLUT_FORWARD_COMPATIBLE );
    glutInitContextFlags( GLUT_DEBUG );

    glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS );

    glutInitWindowSize( WinX, WinY );
    glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA );
    WindowHandle = glutCreateWindow( CAPTION );
    if( WindowHandle < 1 ){
        std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
        exit( EXIT_FAILURE );
    }
}

void setupCamera(){
    camera = new FixedCamera( vec3( 0, 0, 5 ), vec3( 0, 0, 0 ), vec3( 0, 1, 0 ) );
    freeCamera = new FreeCamera( vec3( 0, 0, 5 ), vec3( 0, 0, 0 ), vec3( 0, 1, 0 ) );
    camera->ProjectionMatrix( projectionMatrix );
    freeCamera->ProjectionMatrix( projectionMatrix );
}

void loadMeshes(){
    Catalog<Mesh*>* meshManager = Catalog<Mesh*>::instance();
    meshManager->insert( "triangle", meshLoader.createMesh( std::string( "Mesh/Triangle.obj" ) ) );
    meshManager->insert( "square", meshLoader.createMesh( std::string( "Mesh/Square.obj" ) ) );
    meshManager->insert( "parallelogram", meshLoader.createMesh( std::string( "Mesh/Parallelogram.obj" ) ) );
    meshManager->insert( "table", meshLoader.createMesh( std::string( "Mesh/Table.obj" ) ) );

}

void loadTextures(){
    Catalog<Texture*>* textureCatalog = Catalog<Texture*>::instance();
    textureCatalog->insert( "wood", new Texture( "Textures/wood.jpg" ) );
    textureCatalog->insert( "error2", new Texture( "Textures/errorTexture3.png" ) );
}

void createScene(){
    vec4 YY = vec4( 0, 1, 0, 1 );
    const mat4 tr1 = MatrixFactory::createTranslationMatrix( -0.2f, 0.0f, -0.8f ) *
        MatrixFactory::createRotationMatrix4( -90.0f, YY );

    const mat4 tr2 = MatrixFactory::createTranslationMatrix( -0.4f, 0.0f, 0.2f );

    const mat4 tr3 = MatrixFactory::createTranslationMatrix( 0.2f, 0.0f, 0.0f ) *
        MatrixFactory::createScaleMatrix4( 0.5f, 1, 0.5f ) *
        MatrixFactory::createRotationMatrix4( 90.0f, YY );

    const mat4 pl45 = MatrixFactory::createTranslationMatrix( 0.2f, 0.0f, -0.4f ) *
        MatrixFactory::createRotationMatrix4( 90.0f, YY );

    const mat4 tr6 = MatrixFactory::createTranslationMatrix( 0.0f, 0.0f, -0.6f ) *
        MatrixFactory::createScaleMatrix4( 0.5f, 1, 0.5f ) *
        MatrixFactory::createRotationMatrix4( 90.0f, YY );

    const mat4 sq78 = MatrixFactory::createTranslationMatrix( 0.0f, 0.0f, 0.34f ) * // '.14f is half the side of the square
        MatrixFactory::createRotationMatrix4( 45.0f, YY ) * //rotate 45 degrees
        MatrixFactory::createTranslationMatrix( -0.2f, 0.0f, 0.0f ); // center in the origin

    const mat4 tr9 = MatrixFactory::createTranslationMatrix( 0.4f * 0.707f, 0.0f, 0.48f ) * // 0.8f * 0.707f / 2  is the center of the hypotenuse to the origin
        MatrixFactory::createScaleMatrix4( 0.707f, 1, 0.707f ) *
        MatrixFactory::createRotationMatrix4( -180.0f, YY );

    const vec4 red = vec4( 1.0f, 0.0f, 0.0f, 1.0f );
    const vec4 green = vec4( 0.0f, 1.0f, 0.0f, 1.0f );
    const vec4 blue = vec4( 0.0f, 0.0f, 1.0f, 1.0f );
    const vec4 cyan = vec4( 0.0f, 1.0f, 1.0f, 1.0f );
    const vec4 magenta = vec4( 1.0f, 0.0f, 1.0f, 1.0f );
    const vec4 yellow = vec4( 1.0f, 1.0f, 0.0f, 1.0f );
    const vec4 white = vec4( 1.0f, 1.0f, 1.0f, 1.0f );
    const vec4 orange = vec4( 1.0f, 0.2f, 0.0f, 1.0f );
    const vec4 purple = vec4( 0.4f, 0.0f, 0.4f, 1.0f );

    SceneNode* tangram;
    SceneNode  *trpc1, *trpc2, *trpc3, *trpc6, *trpc9, *plpc45, *sqpc78;

    Catalog<Mesh*>* meshManager = Catalog<Mesh*>::instance();
    Catalog<ShaderProgram*> *shaderProgramManager = Catalog<ShaderProgram*>::instance();
    ShaderProgram* dfault = shaderProgramManager->get( "default" );
    ShaderProgram* prog = shaderProgramManager->get( "ColorProgram" );

    scene = new Scene( dfault, camera );

    //TABLE SETUP
    table = new SceneNode( meshManager->get( "table" ), shaderProgramManager->get( "ColorTextureProgram" ) );
    table->addTexture( "wood" );
    table->setColor( orange );
    scene->addNode( table );

    tangram = new SceneNode( nullptr, prog, MatrixFactory::createIdentityMatrix4() );
    table->addNode( tangram );

    trpc1 = new SceneNode( meshManager->get( "triangle" ), prog, tr1 );
    trpc1->setColor( red );
    tangram->addNode( trpc1 );

    trpc2 = new SceneNode( meshManager->get( "triangle" ), prog, tr2 );
    trpc2->setColor( green );
    tangram->addNode( trpc2 );

    trpc3 = new SceneNode( meshManager->get( "triangle" ), prog, tr3 );
    trpc3->setColor( blue );
    tangram->addNode( trpc3 );

    trpc6 = new SceneNode( meshManager->get( "triangle" ), prog, tr6 );
    trpc6->setColor( cyan );
    tangram->addNode( trpc6 );

    trpc9 = new SceneNode( meshManager->get( "triangle" ), prog, tr9 );
    trpc9->setColor( magenta );
    tangram->addNode( trpc9 );

    sqpc78 = new SceneNode( meshManager->get( "square" ), shaderProgramManager->get( "ColorTextureProgram" ), sq78 );
    sqpc78->addTexture( "wood" );
    sqpc78->setColor( yellow );
    tangram->addNode( sqpc78 );

    plpc45 = new SceneNode( meshManager->get( "parallelogram" ), shaderProgramManager->get( "ColorTextureProgram" ), pl45 );
    plpc45->addTexture( "wood" );
    plpc45->setColor( white );
    tangram->addNode( plpc45 );

}


void createParticleSystem(){
    Catalog<ShaderProgram*> *shaderProgramManager = Catalog<ShaderProgram*>::instance();
    /**/
    //particlesOne = new ParticleSystem( shaderProgramManager->get( "GeometryProperLightParticleProgram" ),
              //camera, vec3( 0.0f, -0.8f, 0.0f ) );
    //checkOpenGLError( "ERROR: Could not create ParticleSystemOne." );

    particlesTwo = new ParticleSystemTransform( shaderProgramManager->get( "TFBDraw" ),
        shaderProgramManager->get( "TFBUpdate" ), camera, vec3( 0.0f, -0.2f, 0.0f ) );
    particlesTwo->InitParticleSystem();
    checkOpenGLError( "ERROR: Could not create ParticleSystemTwo." );
    /**/

}

void setupLight(){
    pointLights[0] = PointLight( vec3( -0.3f, 0.4f, 0.4f ), 1.0f, 1.0f, 10.5f,
        vec3( 0.0f, 1.0f, 1.0f ), vec3( 0.0f, 1.0f, 1.0f ), vec3( 0.5f, 0.5f, 0.5f ) );

    pointLights[1] = PointLight( vec3( 0.3f, 0.4f, 0.4f ), 1.0f, 1.0f, 10.5f,
        vec3( 1.0f, 0.0f, 1.0f ), vec3( 1.0f, 0.0f, 1.0f ), vec3( 0.5f, 0.5f, 0.5f ) );

    pointLights[2] = PointLight( vec3( 0.0f, 0.4f, -0.5f ), 1.0f, 1.0f, 10.5f,
        vec3( 1.0f, 1.0f, 0.0f ), vec3( 1.0f, 1.0f, 0.0f ), vec3( 0.5f, 0.5f, 0.5f ) );

    float endX = 1.0f;
    float beginX = -1.0f;
    float offset = ( endX - beginX ) / ( NR_NEON_LIGHTS - 1 );
    vec3 pos = vec3( 0.0f, 1.5f, 0.0f );
    vec3 dropoff = vec3( 0.0f, 5.0f, 0.5f );

    //vec3 ambient = vec3( 0.5f, 0.0f, 0.5f );
    //vec3 diffuse = vec3( 0.5f, 0.0f, 0.5f );
    vec3 ambient = vec3( 1.0f, 0.0f, 0.0f );
    vec3 diffuse = vec3( 1.0f, 0.0f, 0.0f );
    vec3 specular = vec3( 0.1f, 0.1f, 0.1f );

    for( int i = 3; i < NR_NEON_LIGHTS + 3; i++ ){
        pointLights[i] = PointLight( pos, dropoff, ambient, diffuse, specular );
        pos.x += offset;
    }

}

void activateLights(){
    Catalog<ShaderProgram*> *shaderProgramManager = Catalog<ShaderProgram*>::instance();
    ShaderProgram* shader = shaderProgramManager->get( "TFBDraw" );

    shader->use();
    for( int i = 0; i < NR_POINT_LIGHTS; i++ ){
        pointLights[i].addItself( shader, i );
    }
    shader->stop();
}

void init( int argc, char* argv[] ){
    setupGLUT( argc, argv );
    setupGLEW();
    setupOpenGL();

    setupCamera();
    setupLight();
    setupCallbacks();


    loadMeshes();
    loadTextures();

    createShaderProgram();
    activateLights();

    createScene();


    createParticleSystem();

    createBufferObjects();

}

int main( int argc, char* argv[] ){

    init( argc, argv );
    glutMainLoop();
    exit( EXIT_SUCCESS );

}

///////////////////////////////////////////////////////////////////////
