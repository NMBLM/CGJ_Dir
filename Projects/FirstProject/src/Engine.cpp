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
#include  "TextureInfo.h"
#include "LightSource.h"

#include "GL/glew.h"
#include "GL/freeglut.h"

#define CAPTION "CyberNoodles"
#define NR_NEON_LIGHTS 8
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
SceneNode  *table;
SceneNode  *quad;
ParticleSystemTransform* particlesOne;

float lastFrame = 0.0f;
float delta = 0.0f;
int lastMouseY = WinX / 2;
int lastMouseX = WinY / 2;
float k = 0.0f;
bool freecam = false;

mat4 projectionMatrix = MatrixFactory::createPerspectiveProjectionMatrix( 30, ( float )WinX / ( float )WinY, 1, 30 );
mat4 otherProjectionMatrix = MatrixFactory::createOrtographicProjectionMatrix( -2, 2, -2, 2, 1, 30 );

unsigned int hdrFBO;
unsigned int colorBuffers[2];
unsigned int colorBuffer;

unsigned int pingpongFBO[2];
unsigned int pingpongColorbuffers[2];

unsigned int rboDepth;

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
            particlesOne->camera = camera;

        } else{
            scene->setCamera( freeCamera );
            particlesOne->camera = freeCamera;

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
        particlesOne->position += vec3( -1.0f * delta, 0.0f, 0.0f );
    }
    if( KeyBuffer::instance()->isSpecialKeyDown( GLUT_KEY_UP ) ){
        table->updateModel( MatrixFactory::createTranslationMatrix( 0.0f, 0.0f, -1.0f * delta ) );
        particlesOne->position += vec3( 0.0f, 0.0f, -1.0f * delta );
    }
    if( KeyBuffer::instance()->isSpecialKeyDown( GLUT_KEY_RIGHT ) ){
        table->updateModel( MatrixFactory::createTranslationMatrix( 1.0f * delta, 0.0f, 0.0f ) );
        particlesOne->position += vec3( 1.0f * delta, 0.0f, 0.0f );

    }
    if( KeyBuffer::instance()->isSpecialKeyDown( GLUT_KEY_DOWN ) ){
        table->updateModel( MatrixFactory::createTranslationMatrix( 0.0f, 0.0f, 1.0f * delta ) );
        particlesOne->position += vec3( 0.0f, 0.0f, 1.0f * delta );
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
    dfault = new ShaderProgram();//1
    dfault->attachShader( GL_VERTEX_SHADER, "vertex", "Shaders/default_vs.glsl" );
    dfault->attachShader( GL_FRAGMENT_SHADER, "fragment", "Shaders/default_fs.glsl" );

    dfault->bindAttribLocation( VERTICES, "inPosition" );
    dfault->link();

    dfault->detachShader( "vertex" );
    dfault->detachShader( "fragment" );

    shaderProgramManager->insert( "default", dfault );
    checkOpenGLError( "ERROR: Could not create default shaders." );


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
    prog = new ShaderProgram(); //2
    prog->attachShader( GL_GEOMETRY_SHADER, "geometry", "Shaders/tfb_billboard_gs.glsl" );
    prog->attachShader( GL_VERTEX_SHADER, "vertex", "Shaders/tfb_billboard_vs.glsl" );
    prog->attachShader( GL_FRAGMENT_SHADER, "fragment", "Shaders/tfb_billboard_fs.glsl" );

    prog->link();

    prog->detachShader( "geometry" );
    prog->detachShader( "vertex" );
    prog->detachShader( "fragment" );

    shaderProgramManager->insert( "TFBDraw", prog );
    prog = new ShaderProgram(); //3
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

    // mapping
    prog = new ShaderProgram();//4
    prog->attachShader( GL_VERTEX_SHADER, "vertex", "Shaders/mapping_vs.glsl" );
    prog->attachShader( GL_FRAGMENT_SHADER, "fragment", "Shaders/mapping_fs.glsl" );

    prog->bindAttribLocation( VERTICES, "inPosition" );
    prog->bindAttribLocation( TEXCOORDS, "inTexcoord" );
    prog->bindAttribLocation( NORMALS, "inNormal" );
    prog->bindAttribLocation( TANGENTS, "inTangent" );
    prog->bindAttribLocation( BI_TANGENTS, "inBiTangent" );

    prog->link();

    prog->detachShader( "vertex" );
    prog->detachShader( "fragment" );

    shaderProgramManager->insert( "Mapping", prog );

    prog = new ShaderProgram();//5
    prog->attachShader( GL_VERTEX_SHADER, "vertex", "Shaders/glow_mapping_vs.glsl" );
    prog->attachShader( GL_FRAGMENT_SHADER, "fragment", "Shaders/glow_mapping_fs.glsl" );

    prog->bindAttribLocation( VERTICES, "inPosition" );
    prog->bindAttribLocation( TEXCOORDS, "inTexcoord" );
    prog->bindAttribLocation( NORMALS, "inNormal" );
    prog->bindAttribLocation( TANGENTS, "inTangent" );
    prog->bindAttribLocation( BI_TANGENTS, "inBiTangent" );

    prog->link();

    prog->detachShader( "vertex" );
    prog->detachShader( "fragment" );

    shaderProgramManager->insert( "GlowMapping", prog );

    //Glow
    prog = new ShaderProgram();//6
    prog->attachShader( GL_VERTEX_SHADER, "vertex", "Shaders/glow_vs.glsl" );
    prog->attachShader( GL_FRAGMENT_SHADER, "fragment", "Shaders/glow_fs.glsl" );

    prog->bindAttribLocation( VERTICES, "Position" );
    prog->bindAttribLocation( TEXCOORDS, "Texcoord" );
    prog->bindAttribLocation( NORMALS, "Normal" );

    prog->link();

    prog->detachShader( "vertex" );
    prog->detachShader( "fragment" );

    shaderProgramManager->insert( "GlowOne", prog );

    //HDR
    prog = new ShaderProgram();//7
    prog->attachShader( GL_VERTEX_SHADER, "vertex", "Shaders/hdr_vs.glsl" );
    prog->attachShader( GL_FRAGMENT_SHADER, "fragment", "Shaders/hdr_fs.glsl" );

    prog->bindAttribLocation( VERTICES, "Position" );
    prog->bindAttribLocation( TEXCOORDS, "Texcoord" );
    prog->bindAttribLocation( NORMALS, "Normal" );

    prog->link();

    prog->detachShader( "vertex" );
    prog->detachShader( "fragment" );

    prog->use();
    prog->stop();
    shaderProgramManager->insert( "HDR", prog );

    //LightBox
    prog = new ShaderProgram();//8
    prog->attachShader( GL_VERTEX_SHADER, "vertex", "Shaders/lightbox_vs.glsl" );
    prog->attachShader( GL_FRAGMENT_SHADER, "fragment", "Shaders/lightbox_fs.glsl" );

    prog->bindAttribLocation( VERTICES, "Position" );
    prog->bindAttribLocation( TEXCOORDS, "Texcoord" );
    prog->bindAttribLocation( NORMALS, "Normal" );

    prog->link();

    prog->detachShader( "vertex" );
    prog->detachShader( "fragment" );

    shaderProgramManager->insert( "LightBox", prog );
    //Blur Bloom(hdr aswell)
    prog = new ShaderProgram();//9
    prog->attachShader( GL_VERTEX_SHADER, "vertex", "Shaders/blur_vs.glsl" );
    prog->attachShader( GL_FRAGMENT_SHADER, "fragment", "Shaders/blur_fs.glsl" );

    prog->bindAttribLocation( VERTICES, "Position" );
    prog->bindAttribLocation( TEXCOORDS, "Texcoord" );
    prog->bindAttribLocation( NORMALS, "Normal" );

    prog->link();
    prog->detachShader( "vertex" );
    prog->detachShader( "fragment" );

    shaderProgramManager->insert( "Blur", prog );

    prog = new ShaderProgram();//10
    prog->attachShader( GL_VERTEX_SHADER, "vertex", "Shaders/bloom_vs.glsl" );
    //prog->attachShader( GL_FRAGMENT_SHADER, "fragment", "Shaders/bloom_fs.glsl" );
    prog->attachShader( GL_FRAGMENT_SHADER, "fragment", "Shaders/bloom_cube_fs.glsl" );

    prog->bindAttribLocation( VERTICES, "Position" );
    prog->bindAttribLocation( TEXCOORDS, "Texcoord" );
    prog->bindAttribLocation( NORMALS, "Normal" );

    prog->link();

    prog->detachShader( "vertex" );
    prog->detachShader( "fragment" );

    shaderProgramManager->insert( "Bloom", prog );

    prog = new ShaderProgram();//10
    prog->attachShader( GL_VERTEX_SHADER, "vertex", "Shaders/bloom_final_vs.glsl" );
    prog->attachShader( GL_FRAGMENT_SHADER, "fragment", "Shaders/bloom_final_fs.glsl" );

    prog->bindAttribLocation( VERTICES, "Position" );
    prog->bindAttribLocation( TEXCOORDS, "Texcoord" );
    prog->bindAttribLocation( NORMALS, "Normal" );

    prog->link();

    prog->detachShader( "vertex" );
    prog->detachShader( "fragment" );

    shaderProgramManager->insert( "BloomFinal", prog );




    checkOpenGLError( "ERROR: Could not create shaders." );

}

void destroyShaderProgram(){
    Catalog<ShaderProgram*> *shaderProgramManager = Catalog<ShaderProgram*>::instance();

    glUseProgram( 0 );
    glDeleteProgram( shaderProgramManager->get( "default" )->id );

    checkOpenGLError( "ERROR: Could not destroy shaders." );
}

void destroyMeshes(){
    Catalog<ShaderProgram*> *shaderProgramManager = Catalog<ShaderProgram*>::instance();
    //TODO kill

}

void destroyTextures(){
    Catalog<ShaderProgram*> *shaderProgramManager = Catalog<ShaderProgram*>::instance();
    //TODO kill

}

unsigned int quadVAO = 0;
unsigned int quadVBO;

void renderQuad(){
    if( quadVAO == 0 ){
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays( 1, &quadVAO );
        glGenBuffers( 1, &quadVBO );
        glBindVertexArray( quadVAO );
        glBindBuffer( GL_ARRAY_BUFFER, quadVBO );
        glBufferData( GL_ARRAY_BUFFER, sizeof( quadVertices ), &quadVertices, GL_STATIC_DRAW );
        glEnableVertexAttribArray( 0 );
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), ( void* )0 );
        glEnableVertexAttribArray( 1 );
        glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), ( void* )( 3 * sizeof( float ) ) );
    }
    glBindVertexArray( quadVAO );
    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
    glBindVertexArray( 0 );
}
/////////////////////////////////////////////////////////////////////// SCENE

void drawScene(){
    Catalog<ShaderProgram*> *shaderProgramManager = Catalog<ShaderProgram*>::instance();
    // 1. render scene into floating point framebuffer
    // -----------------------------------------------
    glBindFramebuffer( GL_FRAMEBUFFER, hdrFBO );
    glClearColor( 0, 0, 0, 0 ); // make the background black and not the default grey
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    scene->draw();
    particlesOne->draw();
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    /**/
    // 2. blur bright fragments with two-pass Gaussian Blur
    // --------------------------------------------------
    bool horizontal = true, first_iteration = true;
    unsigned int amount = 10;
    ShaderProgram* blur = shaderProgramManager->get( "Blur" );
    blur->use();
    glActiveTexture( GL_TEXTURE0 );
    blur->addUniform( "image", 0 ); // 3 because GL_texture3
    for( unsigned int i = 0; i < amount; i++ ){
        glBindFramebuffer( GL_FRAMEBUFFER, pingpongFBO[horizontal] );
        blur->addUniform( "horizontal", horizontal );
        glBindTexture( GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongColorbuffers[!horizontal] );  // bind texture of other framebuffer (or scene if first iteration)
        renderQuad();
        horizontal = !horizontal;
        if( first_iteration )
            first_iteration = false;
    }
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    blur->stop();
    /**/
    // 3. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
    // --------------------------------------------------------------------------------------------------------------------------
    ShaderProgram* bloomFinal = shaderProgramManager->get( "BloomFinal" );
    bloomFinal->use();
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, colorBuffers[0] );
    bloomFinal->addUniform( "scene", 0 ); // 3 because GL_TEXTURE3
    glActiveTexture( GL_TEXTURE1 );
    glBindTexture( GL_TEXTURE_2D, pingpongColorbuffers[!horizontal] );
    bloomFinal->addUniform( "bloomBlur", 1 ); // 4 because GL_TEXTURE4

    renderQuad();

    bloomFinal->stop();
    /**/
    checkOpenGLError( "ERROR: Could not draw scene." );
}

/////////////////////////////////////////////////////////////////////// CALLBACKS

void cleanup(){
    destroyShaderProgram();
    destroyMeshes();
    destroyTextures();
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
    particlesOne->update( delta );
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
    //meshManager->insert(Mesh::TRIANGLE, meshLoader.createMesh(std::string("Mesh/Triangle.obj")));
    //meshManager->insert(Mesh::SQUARE, meshLoader.createMesh(std::string("Mesh/Square.obj")));
    //meshManager->insert(Mesh::PARALLELOGRAM, meshLoader.createMesh(std::string("Mesh/Parallelogram.obj")));
    //meshManager->insert(Mesh::TABLE, meshLoader.createMesh(std::string("Mesh/Table.obj")));
    meshManager->insert( Mesh::CUBE, meshLoader.createMesh( std::string( "Mesh/Cube.obj" ) ) );
    //meshManager->insert(Mesh::QUAD, meshLoader.createMesh(std::string("Mesh/Quad.obj")));
    meshManager->insert( Mesh::SPHERE, meshLoader.createMesh( std::string( "Mesh/Sphere.obj" ) ) );
}

void loadTextures(){

    Catalog<Texture*>* textureCatalog = Catalog<Texture*>::instance();
    //textureCatalog->insert(Texture::WOOD, new Texture("Textures/wood.jpg"));
    //textureCatalog->insert( Texture::DEFAULT, new Texture( "Textures/errorTexture.jpg" ) );
    //textureCatalog->insert( Texture::NOODLE_TEXTURE, new Texture( "Textures/noodle_texture.jpg" ) );
    //textureCatalog->insert( Texture::NOODLE_MAP_NORMAL, new Texture( "Textures/noodle_normal_map.jpg" ) );
    //textureCatalog->insert( Texture::NOODLE_MAP_SPECULAR, new Texture( "Textures/noodle_specular_map.jpg" ) );

    textureCatalog->insert( Texture::WORLD_CUBE, new TextureCube( "Textures/cubemap/world_", ".png" ) );
    textureCatalog->insert( Texture::BEACH_BOX, new TextureCube( "Textures/skybox/beach_", ".jpg" ) );

    //textureCatalog->insert( Texture::NOODLE_MAP_SPECULAR, new Texture( "Textures/noodle_specular_map.jpg" ) );
    //textureCatalog->insert(Texture::NOODLE_MAP_DISPLACEMENT, new Texture("Textures/noodle_displacement_map.jpg"));
    //textureCatalog->insert(Texture::NOODLE_MAP_AO, new Texture("Textures/noodle_ao_map.jpg"));
}

vec4 YY = vec4( 0, 1, 0, 1 );

void createSceneMapping(){
    Catalog<Mesh*>* meshManager = Catalog<Mesh*>::instance();
    Catalog<ShaderProgram*> *shaderProgramManager = Catalog<ShaderProgram*>::instance();
    ShaderProgram* dfault = shaderProgramManager->get( "default" );

    scene = new Scene( dfault, camera );
    /** /
    quad = new SceneNode( meshManager->get( Mesh::SPHERE ), shaderProgramManager->get( "GlowOne" ),
        MatrixFactory::createTranslationMatrix( 0.0f, 0.5f, 0.0f ) * MatrixFactory::createScaleMatrix4( 0.2f, 0.2f, 0.2f ) );

    //scene->addNode( quad );
    /**/
    //GL_TEXTURE0 is used for hdr
    //GL_TEXTURE1 is used for saving brightnessvalues and blurring
    TextureInfo* noodleTextureInfo = new TextureInfo( Texture::BEACH_BOX, "noodleTex", GL_TEXTURE2, 2 );
    TextureInfo* noodleNormalInfo = new TextureInfo( Texture::BEACH_BOX, "noodleNormal", GL_TEXTURE3, 3 );
    TextureInfo* noodleSpecularInfo = new TextureInfo( Texture::BEACH_BOX, "noodleSpec", GL_TEXTURE4, 4 );


    quad = new SceneNode( meshManager->get( Mesh::CUBE ), shaderProgramManager->get( "Bloom" ),
        MatrixFactory::createScaleMatrix4( -8.0f, -8.0f, -8.0f ) );
    quad->addTexture( noodleTextureInfo );
    quad->addTexture( noodleNormalInfo );
    quad->addTexture( noodleSpecularInfo );
    scene->addNode( quad );
    /**/
}



void createParticleSystem(){
    Catalog<ShaderProgram*> *shaderProgramManager = Catalog<ShaderProgram*>::instance();

    particlesOne = new ParticleSystemTransform( shaderProgramManager->get( "TFBDraw" ),
        shaderProgramManager->get( "TFBUpdate" ), camera, vec3( 0.0f, 0.0f, 0.0f ) );
    particlesOne->InitParticleSystem();
    checkOpenGLError( "ERROR: Could not create ParticleSystemTwo." );

}

void setupLight(){
    float scale = 0.05f;
    mat4 boxScale = MatrixFactory::createScaleMatrix4( scale, scale, scale );
    Catalog<Mesh*>* meshManager = Catalog<Mesh*>::instance();
    Catalog<ShaderProgram*> *shaderProgramManager = Catalog<ShaderProgram*>::instance();
    SceneNode* lights = new SceneNode( nullptr, shaderProgramManager->get( "default" ) );
    SceneNode* temp;
    vec3 pos;
    vec3 color;
    int i = 0;
    scale = 1.5f;
    /**/
    //Cyan
    pos = vec3( -3.0f / scale, 0.4f, 4.0f / scale );
    color = vec3( 0.0f, 50.0f, 50.0f );
    pointLights[i] = PointLight( pos, 20.0f, 10.0f, 10.5f,
        color, color, vec3( 0.1f, 0.1f, 0.1f ) );
    //vec3( 0.0f, 0.0f, 0.0f ), vec3( 0.0f, 0.0f, 0.0f ), vec3( 0.0f, 0.0f, 0.0f ) );
    temp = new SceneNode( meshManager->get( Mesh::SPHERE ), shaderProgramManager->get( "LightBox" ),
        MatrixFactory::createTranslationMatrix( pos ) * boxScale );
    temp->setColor( vec4( color, 1.0f ) );
    lights->addNode( temp );
    i++;

    //Magenta
    pos = vec3( 4.0f / scale, 0.4f, 4.0f / scale );
    color = vec3( 50.0f, .0f, 50.0f );
    pointLights[i] = PointLight( pos, 20.0f, 10.0f, 10.5f,
        color, color, vec3( 0.1f, 0.1f, 0.1f ) );
    //vec3( 0.0f, 0.0f, 0.0f ), vec3( 0.0f, 0.0f, 0.0f ), vec3( 0.0f, 0.0f, 0.0f ) );

    temp = new SceneNode( meshManager->get( Mesh::SPHERE ), shaderProgramManager->get( "LightBox" ),
        MatrixFactory::createTranslationMatrix( pos ) * boxScale );
    temp->setColor( vec4( color, 1.0f ) );
    lights->addNode( temp );
    i++;

    //Yellow
    pos = vec3( 0.0f / scale, 0.4f, -5.0f / scale );
    color = vec3( 50.0f, 50.0f, 0.0f );
    pointLights[i] = PointLight( pos, 20.0f, 10.0f, 10.5f,
        color, color, vec3( 0.1f, 0.1f, 0.1f ) );
    //vec3( 0.0f, 0.0f, 0.0f ), vec3( 0.0f, 0.0f, 0.0f ), vec3( 0.0f, 0.0f, 0.0f ) );

    temp = new SceneNode( meshManager->get( Mesh::SPHERE ), shaderProgramManager->get( "LightBox" ),
        MatrixFactory::createTranslationMatrix( pos )* boxScale );
    temp->setColor( vec4( color, 1.0f ) );
    lights->addNode( temp );
    i++;

    /**/
    float endX = 0.3f;
    float beginX = -0.3f;
    float offset = ( endX - beginX ) / ( NR_NEON_LIGHTS - 1 );
    pos = vec3( beginX, 0.5f, 0.0f );
    vec3 dropoff = vec3( 0.0f, 5.0f, 100.0f );

    //vec3 ambient = vec3( 0.5f, 0.0f, 0.5f );
    //vec3 diffuse = vec3( 0.5f, 0.0f, 0.5f );
    //vec3 ambient = vec3( 0.0f, 0.0f, 0.0f );
    //vec3 diffuse = vec3( 0.0f, 0.0f, 0.0f );
    //vec3 ambient = vec3( 0.3f, 0.0f, 0.0f );
    //vec3 diffuse = vec3( 0.3f, 0.0f, 0.0f );
    color = vec3( 5.0f, 0.0f, 0.0f );
    vec3 ambient = color;
    vec3 diffuse = color;
    vec3 specular = vec3( 0.1f, 0.1f, 0.1f );

    for( int j = i; j < NR_NEON_LIGHTS + 3; j++, i++ ){
        pointLights[j] = PointLight( pos, dropoff, ambient, diffuse, specular );
        temp = new SceneNode( meshManager->get( Mesh::SPHERE ), shaderProgramManager->get( "LightBox" ),
            MatrixFactory::createTranslationMatrix( pos ) * boxScale );
        pos.x += offset;
        temp->setColor( vec4( color, 1.0f ) );
        lights->addNode( temp );
    }
    /**/
    scene->addNode( lights );
}

void activateLights(){
    Catalog<ShaderProgram*> *shaderProgramManager = Catalog<ShaderProgram*>::instance();
    ShaderProgram* shader = shaderProgramManager->get( "TFBDraw" );

    shader->use();
    for( int i = 0; i < NR_POINT_LIGHTS; i++ ){
        pointLights[i].addItself( shader, i );
    }
    shader->stop();

    shader = shaderProgramManager->get( "GlowOne" );

    shader->use();
    for( int i = 0; i < NR_POINT_LIGHTS; i++ ){
        pointLights[i].addItself( shader, i );
    }
    shader->stop();

    shader = shaderProgramManager->get( "GlowMapping" );

    shader->use();
    for( int i = 0; i < NR_POINT_LIGHTS; i++ ){
        pointLights[i].addItself( shader, i );
    }
    shader->stop();

    shader = shaderProgramManager->get( "Bloom" );

    shader->use();
    for( int i = 0; i < NR_POINT_LIGHTS; i++ ){
        pointLights[i].addItself( shader, i );
    }
    shader->stop();



}


void setupHDR(){
    // configure floating point framebuffer
    // ------------------------------------
    glGenFramebuffers( 1, &hdrFBO );
    glBindFramebuffer( GL_FRAMEBUFFER, hdrFBO );
    /** /
    // create floating point color buffer
    glGenTextures( 1, &colorBuffer );
    glBindTexture( GL_TEXTURE_2D, colorBuffer );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA16F, WinX, WinY, 0, GL_RGBA, GL_FLOAT, NULL );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

    // attach buffers
    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0 );
    /**/
    glGenTextures( 2, colorBuffers );
    for( unsigned int i = 0; i < 2; i++ ){
        glBindTexture( GL_TEXTURE_2D, colorBuffers[i] );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA16F, WinX, WinY, 0, GL_RGBA, GL_FLOAT, NULL );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

        // attach buffers
        glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0 );
    }
    // create depth buffer (renderbuffer)
    glGenRenderbuffers( 1, &rboDepth );
    glBindRenderbuffer( GL_RENDERBUFFER, rboDepth );
    glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WinX, WinY );

    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers( 2, attachments );

    glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth );
    if( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );

    // ping-pong-framebuffer for blurring

    glGenFramebuffers( 2, pingpongFBO );
    glGenTextures( 2, pingpongColorbuffers );
    for( unsigned int i = 0; i < 2; i++ ){
        glBindFramebuffer( GL_FRAMEBUFFER, pingpongFBO[i] );
        glBindTexture( GL_TEXTURE_2D, pingpongColorbuffers[i] );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB16F, WinX, WinY, 0, GL_RGB, GL_FLOAT, NULL );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0 );
        // also check if framebuffers are complete (no need for depth buffer)
        if( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
            std::cout << "Framebuffer not complete!" << std::endl;
    }
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );



}

void init( int argc, char* argv[] ){
    setupGLUT( argc, argv );
    setupGLEW();
    setupOpenGL();

    setupCamera();
    setupCallbacks();


    loadMeshes();
    loadTextures();

    setupHDR();
    createShaderProgram();

    createSceneMapping();
    createParticleSystem();

    setupLight();
    activateLights();
}

int main( int argc, char* argv[] ){

    init( argc, argv );
    glutMainLoop();
    exit( EXIT_SUCCESS );

}

///////////////////////////////////////////////////////////////////////
