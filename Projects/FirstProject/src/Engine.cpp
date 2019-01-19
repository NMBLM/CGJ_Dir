#include "Engine.h"

/////////////////////////////////////////////////////////////////////// ERRORS

static std::string errorType(GLenum type) {
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:				return "error";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	return "deprecated behavior";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	return "undefined behavior";
        case GL_DEBUG_TYPE_PORTABILITY:			return "portability issue";
        case GL_DEBUG_TYPE_PERFORMANCE:			return "performance issue";
        case GL_DEBUG_TYPE_MARKER:				return "stream annotation";
        case GL_DEBUG_TYPE_OTHER_ARB:			return "other";
        default:								exit(EXIT_FAILURE);
    }
}

static std::string errorSource(GLenum source) {
    switch (source) {
        case GL_DEBUG_SOURCE_API:				return "API";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		return "window system";
        case GL_DEBUG_SOURCE_SHADER_COMPILER:	return "shader compiler";
        case GL_DEBUG_SOURCE_THIRD_PARTY:		return "third party";
        case GL_DEBUG_SOURCE_APPLICATION:		return "application";
        case GL_DEBUG_SOURCE_OTHER:				return "other";
        default:								exit(EXIT_FAILURE);
    }
}

static std::string errorSeverity(GLenum severity) {
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:			return "high";
        case GL_DEBUG_SEVERITY_MEDIUM:			return "medium";
        case GL_DEBUG_SEVERITY_LOW:				return "low";
        case GL_DEBUG_SEVERITY_NOTIFICATION:	return "notification";
        default:								exit(EXIT_FAILURE);
    }
}

static void error(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                  const GLchar *message, const void *userParam) {
    std::cerr << "ERROR:" << std::endl;
    std::cerr << "  source:     " << errorSource(source) << std::endl;
    std::cerr << "  type:       " << errorType(type) << std::endl;
    std::cerr << "  severity:   " << errorSeverity(severity) << std::endl;
    std::cerr << "  debug call: " << std::endl << message << std::endl << std::endl;
}

void setupErrors() {
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(error, 0);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, GL_TRUE);
    // params: source, type, severity, count, ids, enabled
}

void mouse_wheel_input(int button, int dir, int x, int y) {
    if (!freecam) {
        camera->zoom(dir, delta);
    }
}

void mouse_input(int x, int y) {

    int xoffset = lastMouseX - x;
    int yoffset = lastMouseY - y;
    lastMouseX = x;
    lastMouseY = y;
    float sen = 0.25f;
    if (freecam) {
        freeCamera->cameraLookAround(xoffset * sen, yoffset * sen, delta);
    } else {
        camera->cameraLookAround(xoffset * sen, yoffset * sen, delta);
    }

}

void keyPress(unsigned char key, int x, int y) {
    KeyBuffer::instance()->pressKey(key);
}

void keyRelease(unsigned char key, int x, int y) {
    if (KeyBuffer::instance()->isKeyDown('f') || KeyBuffer::instance()->isKeyDown('F')) {
        if (freecam) {
            scene->setCamera(camera);
            deferredScene->setCamera( camera );
            particlesOne->camera = camera;
            deferredParticles->camera = camera;
        } else {
            scene->setCamera(freeCamera);
            deferredScene->setCamera( freeCamera );
            particlesOne->camera = freeCamera;
            deferredParticles->camera = freeCamera;
        }
        freecam = !freecam;
    }

    if (KeyBuffer::instance()->isKeyDown('b')) deferred = !deferred;
    if (KeyBuffer::instance()->isKeyDown('B')) deferred = !deferred;

    if( KeyBuffer::instance()->isKeyDown( 'm' ) || KeyBuffer::instance()->isKeyDown( 'M' ) ){
        mode = ( mode + 1 ) % MODES_NUM;
    }
    if( KeyBuffer::instance()->isKeyDown( 'n' ) || KeyBuffer::instance()->isKeyDown( 'N' ) ){
        mode = ( mode - 1 ) % MODES_NUM;
        mode = ( mode < 0 ) ? ( MODES_NUM - 1 ) : mode;
    }
    if( KeyBuffer::instance()->isKeyDown( 'm' ) || KeyBuffer::instance()->isKeyDown( 'M' ) 
        || KeyBuffer::instance()->isKeyDown( 'n' ) || KeyBuffer::instance()->isKeyDown( 'N' ) ){
        switch( mode ){
            case 0: std::cout << "Mode " << mode << ": Normal mode with blured occlusion" << std::endl;
                break;
            case 1: std::cout << "Mode " << mode << ": Stored Position in ViewSpace" << std::endl;
                break;
            case 2: std::cout << "Mode " << mode << ": Stored Normals in ViewSpace" << std::endl;
                break;
            case 3: std::cout << "Mode " << mode << ": Basic Object Color" << std::endl;
                break;
            case 4: std::cout << "Mode " << mode << ": Only Brightness Unblurred" << std::endl;
                break;
            case 5: std::cout << "Mode " << mode << ": Only Brightness Blurred" << std::endl;
                break;
            case 6: std::cout << "Mode " << mode << ": Only Occlusion" << std::endl;
                break;
            case 7: std::cout << "Mode " << mode << ": Only Occlusion Blurred" << std::endl;
                break;
            case 8: std::cout << "Mode " << mode << ": Stored Position in WorldSpace" << std::endl;
                break;
            case 9: std::cout << "Mode " << mode << ": Stored Normals in WorldSpace" << std::endl;
                break;
            case 10: std::cout << "Mode " << mode << ": Normal but no occlusion" << std::endl;
                break;
            case 11: std::cout << "Mode " << mode << ": Normal mode but unblurred occlusion" << std::endl;
                break;
        }
    }
    KeyBuffer::instance()->releaseKey(key);
}

void specialKeyPress(int key, int x, int y) {
    KeyBuffer::instance()->pressSpecialKey(key);
}

void specialKeyRelease(int key, int x, int y) {
    KeyBuffer::instance()->releaseSpecialKey(key);
}

void update() {
    //FixedCamera
    if (!freecam) {
        if (KeyBuffer::instance()->isKeyDown('a')) camera->cameraMoveLeft(delta);
        if (KeyBuffer::instance()->isKeyDown('A')) camera->cameraMoveLeft(delta);

        if (KeyBuffer::instance()->isKeyDown('d')) camera->cameraMoveRight(delta);
        if (KeyBuffer::instance()->isKeyDown('D')) camera->cameraMoveRight(delta);

        if (KeyBuffer::instance()->isKeyDown('s')) camera->cameraMoveBack(delta);
        if (KeyBuffer::instance()->isKeyDown('S')) camera->cameraMoveBack(delta);

        if (KeyBuffer::instance()->isKeyDown('w')) camera->cameraMoveForward(delta);
        if (KeyBuffer::instance()->isKeyDown('W')) camera->cameraMoveForward(delta);

        if (KeyBuffer::instance()->isKeyDown('q')) camera->cameraRollLeft(delta);
        if (KeyBuffer::instance()->isKeyDown('Q')) camera->cameraRollLeft(delta);

        if (KeyBuffer::instance()->isKeyDown('e')) camera->cameraRollRight(delta);
        if (KeyBuffer::instance()->isKeyDown('E')) camera->cameraRollRight(delta);

        if( KeyBuffer::instance()->isKeyDown( 'z' ) ) camera->zoom(1, delta );
        if( KeyBuffer::instance()->isKeyDown( 'Z' ) ) camera->zoom(1, delta );

        if( KeyBuffer::instance()->isKeyDown( 'x' ) ) camera->zoom( -1, delta );
        if( KeyBuffer::instance()->isKeyDown( 'X' ) ) camera->zoom( -1, delta );
    }

    //FreeCamera
    if (freecam) {
        if (KeyBuffer::instance()->isKeyDown('a')) freeCamera->cameraMoveLeft(delta);
        if (KeyBuffer::instance()->isKeyDown('A')) freeCamera->cameraMoveLeft(delta);

        if (KeyBuffer::instance()->isKeyDown('d')) freeCamera->cameraMoveRight(delta);
        if (KeyBuffer::instance()->isKeyDown('D')) freeCamera->cameraMoveRight(delta);

        if (KeyBuffer::instance()->isKeyDown('s')) freeCamera->cameraMoveBack(delta);
        if (KeyBuffer::instance()->isKeyDown('S')) freeCamera->cameraMoveBack(delta);

        if (KeyBuffer::instance()->isKeyDown('w')) freeCamera->cameraMoveForward(delta);
        if (KeyBuffer::instance()->isKeyDown('W')) freeCamera->cameraMoveForward(delta);
    }

    float scale = 0.4f;
    if (KeyBuffer::instance()->isKeyDown('k') || KeyBuffer::instance()->isKeyDown('K')) {
        exposure += delta * scale;
        std::cout << "Exposure: " << exposure << std::endl;
    }
    if (KeyBuffer::instance()->isKeyDown('l') || KeyBuffer::instance()->isKeyDown('L')) {
        if (exposure > 1.0f) {
            exposure -= delta * scale * 4;
        } else {
            exposure -= delta * scale / 20;
        }
        exposure = (exposure > 0.0f) ? exposure : 0.0f;
        std::cout << "Exposure: " << exposure << std::endl;
    }
    if (KeyBuffer::instance()->isKeyDown('h') || KeyBuffer::instance()->isKeyDown('H')) {
        gamma += delta * scale;
        std::cout << "Gamma: " << gamma << std::endl;
    }
    if (KeyBuffer::instance()->isKeyDown('j') || KeyBuffer::instance()->isKeyDown('J')) {
        gamma -= delta * scale;
        gamma = (gamma > 0.0f) ? gamma : 0.0f;
        std::cout << "Gamma: " << gamma << std::endl;
    }
}

static bool isOpenGLError() {
    bool isError = false;
    GLenum errCode;
    const GLubyte *errString;
    while ((errCode = glGetError()) != GL_NO_ERROR) {
        isError = true;
        errString = gluErrorString(errCode);
        std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
    }
    return isError;
}

static void checkOpenGLError(std::string error) {
    if (isOpenGLError()) {
        std::cerr << error << std::endl;
        exit(EXIT_FAILURE);
    }
}

/////////////////////////////////////////////////////////////////////// SHADERs

void createShaderProgram() {
    // DEFAULT SHADER
    Catalog<ShaderProgram*> *shaderProgramManager = Catalog<ShaderProgram*>::instance();
    ShaderProgram* dfault;
    ShaderProgram* prog;
    dfault = new ShaderProgram();//1
    dfault->attachShader(GL_VERTEX_SHADER, "vertex", "Shaders/default_vs.glsl");
    dfault->attachShader(GL_FRAGMENT_SHADER, "fragment", "Shaders/default_fs.glsl");

    dfault->bindAttribLocation(VERTICES, "inPosition");
    dfault->link();

    dfault->detachShader("vertex");
    dfault->detachShader("fragment");

    shaderProgramManager->insert("default", dfault);
    checkOpenGLError("ERROR: Could not create default shaders.");


    //PartTransformProgram
    prog = new ShaderProgram(); //2
    prog->attachShader(GL_GEOMETRY_SHADER, "geometry", "Shaders/tfb_billboard_gs.glsl");
    prog->attachShader(GL_VERTEX_SHADER, "vertex", "Shaders/tfb_billboard_vs.glsl");
    prog->attachShader(GL_FRAGMENT_SHADER, "fragment", "Shaders/tfb_billboard_fs.glsl");

    prog->link();

    prog->detachShader("geometry");
    prog->detachShader("vertex");
    prog->detachShader("fragment");

    shaderProgramManager->insert("TFBDraw", prog);
    prog = new ShaderProgram(); //3
    prog->attachShader(GL_GEOMETRY_SHADER, "geometry", "Shaders/tfb_gs.glsl");
    prog->attachShader(GL_VERTEX_SHADER, "vertex", "Shaders/tfb_vs.glsl");
    prog->attachShader(GL_FRAGMENT_SHADER, "fragment", "Shaders/tfb_fs.glsl");

    const GLchar* Varyings[4];
    Varyings[0] = "Position1";
    Varyings[1] = "Velocity1";
    Varyings[2] = "Life1";
    Varyings[3] = "Type1";

    glTransformFeedbackVaryings(prog->id, 4, Varyings, GL_INTERLEAVED_ATTRIBS);
    prog->link();

    prog->detachShader("geometry");
    prog->detachShader("vertex");
    prog->detachShader("fragment");

    shaderProgramManager->insert("TFBUpdate", prog);
    checkOpenGLError("ERROR: Could not create Transform shaders.");

    //SkyBox
    prog = new ShaderProgram();//4
    prog->attachShader(GL_VERTEX_SHADER, "vertex", "Shaders/skybox_vs.glsl");
    prog->attachShader(GL_FRAGMENT_SHADER, "fragment", "Shaders/skybox_fs.glsl");

    prog->bindAttribLocation(VERTICES, "Position");

    prog->link();

    prog->detachShader("vertex");
    prog->detachShader("fragment");

    shaderProgramManager->insert("SkyBox", prog);

    //LightBox
    prog = new ShaderProgram();//8
    prog->attachShader(GL_VERTEX_SHADER, "vertex", "Shaders/lightbox_vs.glsl");
    prog->attachShader(GL_FRAGMENT_SHADER, "fragment", "Shaders/lightbox_fs.glsl");
    prog = new ShaderProgram();//5
    prog->attachShader(GL_VERTEX_SHADER, "vertex", "Shaders/lightbox_vs.glsl");
    prog->attachShader(GL_FRAGMENT_SHADER, "fragment", "Shaders/lightbox_fs.glsl");

    prog->bindAttribLocation(VERTICES, "Position");
    prog->bindAttribLocation(TEXCOORDS, "Texcoord");
    prog->bindAttribLocation(NORMALS, "Normal");

    prog->link();

    prog->detachShader("vertex");
    prog->detachShader("fragment");

    shaderProgramManager->insert("LightBox", prog);

    //Blur Bloom(hdr aswell)
    prog = new ShaderProgram();//6
    prog->attachShader(GL_VERTEX_SHADER, "vertex", "Shaders/blur_vs.glsl");
    prog->attachShader(GL_FRAGMENT_SHADER, "fragment", "Shaders/blur_fs.glsl");

    prog->bindAttribLocation(VERTICES, "Position");
    prog->bindAttribLocation(TEXCOORDS, "Texcoord");
    prog->bindAttribLocation(NORMALS, "Normal");

    prog->link();
    prog->detachShader("vertex");
    prog->detachShader("fragment");

    shaderProgramManager->insert("Blur", prog);


    //
    prog = new ShaderProgram();//7
    prog->attachShader(GL_VERTEX_SHADER, "vertex", "Shaders/bloom_vs.glsl");
    prog->attachShader(GL_FRAGMENT_SHADER, "fragment", "Shaders/bloom_fs.glsl");

    prog->bindAttribLocation( VERTICES, "inPosition" );
    prog->bindAttribLocation( TEXCOORDS, "inTexcoord" );
    prog->bindAttribLocation( NORMALS, "inNormal" );
    prog->bindAttribLocation( TANGENTS, "inTangent" );
    prog->bindAttribLocation( BI_TANGENTS, "inBiTangent" );

    prog->link();

    prog->detachShader("vertex");
    prog->detachShader("fragment");

    shaderProgramManager->insert("Bloom", prog);

    //
    prog = new ShaderProgram();//8
    prog->attachShader(GL_VERTEX_SHADER, "vertex", "Shaders/bloom_final_vs.glsl");
    prog->attachShader(GL_FRAGMENT_SHADER, "fragment", "Shaders/bloom_final_fs.glsl");

    prog->bindAttribLocation(VERTICES, "Position");
    prog->bindAttribLocation(TEXCOORDS, "Texcoord");
    prog->bindAttribLocation(NORMALS, "Normal");

    prog->link();

    prog->detachShader("vertex");
    prog->detachShader("fragment");

    shaderProgramManager->insert("BloomFinal", prog);

    //Refelction shaders
    prog = new ShaderProgram();//9
    prog->attachShader(GL_VERTEX_SHADER, "vertex", "Shaders/reflection_vs.glsl");
    prog->attachShader(GL_FRAGMENT_SHADER, "fragment", "Shaders/reflection_fs.glsl");

    prog->bindAttribLocation(VERTICES, "Position");
    prog->bindAttribLocation(TEXCOORDS, "Texcoord");
    prog->bindAttribLocation(NORMALS, "Normal");

    prog->link();

    prog->detachShader("vertex");
    prog->detachShader("fragment");

    shaderProgramManager->insert("ReflectionShader", prog);


    checkOpenGLError("ERROR: Could not create shaders.");

}

void createDeferredShaderProgram() {
    Catalog<ShaderProgram*> *shaderProgramManager = Catalog<ShaderProgram*>::instance();
    ShaderProgram *temp;


    temp = new ShaderProgram();// Calculates Light
    temp->attachShader(GL_VERTEX_SHADER, "vertex", "Shaders/Deferred/default_deferred_vs.glsl");
    temp->attachShader(GL_FRAGMENT_SHADER, "fragment", "Shaders/Deferred/default_deferred_fs.glsl");

    temp->bindAttribLocation(VERTICES, "Position");
    temp->bindAttribLocation(TEXCOORDS, "Texcoord");
    temp->bindAttribLocation(NORMALS, "Normal");

    temp->link();

    temp->detachShader("vertex");
    temp->detachShader("fragment");

    shaderProgramManager->insert("DeferredSceneRender", temp);

    temp = new ShaderProgram();// Generates the lightboxes that represent where the lights are
    temp->attachShader(GL_VERTEX_SHADER, "vertex", "Shaders/Deferred/lightbox_deferred_vs.glsl");
    temp->attachShader(GL_FRAGMENT_SHADER, "fragment", "Shaders/Deferred/lightbox_deferred_fs.glsl");

    temp->bindAttribLocation(VERTICES, "Position");
    temp->bindAttribLocation(TEXCOORDS, "Texcoord");
    temp->bindAttribLocation(NORMALS, "Normal");

    temp->link();

    temp->detachShader("vertex");
    temp->detachShader("fragment");

    shaderProgramManager->insert("DeferredLightBox", temp);

    temp = new ShaderProgram();// For the noodles
    temp->attachShader(GL_VERTEX_SHADER, "vertex", "Shaders/Deferred/bloom_deferred_vs.glsl");
    temp->attachShader(GL_FRAGMENT_SHADER, "fragment", "Shaders/Deferred/bloom_deferred_fs.glsl");

    temp->bindAttribLocation( VERTICES, "inPosition" );
    temp->bindAttribLocation( TEXCOORDS, "inTexcoord" );
    temp->bindAttribLocation( NORMALS, "inNormal" );
    temp->bindAttribLocation( TANGENTS, "inTangent" );
    temp->bindAttribLocation( BI_TANGENTS, "inBiTangent" );


    temp->link();

    temp->detachShader("vertex");
    temp->detachShader("fragment");

    shaderProgramManager->insert("DeferredBloom", temp);

    temp = new ShaderProgram();// For the skybox
    temp->attachShader( GL_VERTEX_SHADER, "vertex", "Shaders/Deferred/skybox_deferred_vs.glsl" );
    temp->attachShader( GL_FRAGMENT_SHADER, "fragment", "Shaders/Deferred/skybox_deferred_fs.glsl" );

    temp->bindAttribLocation( VERTICES, "inPosition" );
    temp->bindAttribLocation( TEXCOORDS, "inTexcoord" );
    temp->bindAttribLocation( NORMALS, "inNormal" );

    temp->link();

    temp->detachShader( "vertex" );
    temp->detachShader( "fragment" );

    shaderProgramManager->insert( "DeferredSkybox", temp );


    temp = new ShaderProgram();// for the particles
    temp->attachShader(GL_GEOMETRY_SHADER, "geometry", "Shaders/Deferred/tfb_billboard_deferred_gs.glsl");
    temp->attachShader(GL_VERTEX_SHADER, "vertex", "Shaders/Deferred/tfb_billboard_deferred_vs.glsl");
    temp->attachShader(GL_FRAGMENT_SHADER, "fragment", "Shaders/Deferred/tfb_billboard_deferred_fs.glsl");

    temp->link();

    temp->detachShader("geometry");
    temp->detachShader("vertex");
    temp->detachShader("fragment");

    shaderProgramManager->insert("DeferredTFBDraw", temp);

    //
    temp = new ShaderProgram();//8
    temp->attachShader(GL_FRAGMENT_SHADER, "fragment", "Shaders/ssao_fs.glsl");
    temp->attachShader(GL_VERTEX_SHADER, "vertex", "Shaders/ssao_vs.glsl");

    temp->bindAttribLocation(TEXCOORDS, "TexCoord");

    temp->link();

    temp->detachShader("vertex");
    temp->detachShader("fragment");

    shaderProgramManager->insert("SSAO", temp);

    //
    temp = new ShaderProgram();//8
    temp->attachShader(GL_FRAGMENT_SHADER, "fragment", "Shaders/ssao_test_fs.glsl");
    temp->attachShader(GL_VERTEX_SHADER, "vertex", "Shaders/Deferred/default_deferred_vs.glsl");

    temp->bindAttribLocation(TEXCOORDS, "TexCoord");

    temp->link();

    temp->detachShader("vertex");
    temp->detachShader("fragment");

    shaderProgramManager->insert("SSAO_test", temp);

    //
    temp = new ShaderProgram();//8
    temp->attachShader(GL_FRAGMENT_SHADER, "fragment", "Shaders/ssao_blur_fs.glsl");
    temp->attachShader(GL_VERTEX_SHADER, "vertex", "Shaders/Deferred/default_deferred_vs.glsl");

    temp->bindAttribLocation(TEXCOORDS, "TexCoord");

    temp->link();

    temp->detachShader("vertex");
    temp->detachShader("fragment");

    shaderProgramManager->insert("SSAO_BLUR", temp);
}

void destroyShaderProgram() {
    Catalog<ShaderProgram*> *shaderProgramManager = Catalog<ShaderProgram*>::instance();

    glUseProgram(0);
    glDeleteProgram(shaderProgramManager->get("default")->id);

    checkOpenGLError("ERROR: Could not destroy shaders.");
}

void destroyMeshes() {
    Catalog<ShaderProgram*> *shaderProgramManager = Catalog<ShaderProgram*>::instance();
    //TODO kill

}

void destroyTextures() {
    Catalog<ShaderProgram*> *shaderProgramManager = Catalog<ShaderProgram*>::instance();
    //TODO kill

}

/////////////////////////////////////////////////////////////////////// DEFERRED

void createGBuffer() {
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    // - position color buffer
    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WinX, WinY, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

    // - normal color buffer
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WinX, WinY, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

    // - color + specular color buffer
    glGenTextures(1, &gAlbedoSpec);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WinX, WinY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);

    glGenTextures( 1, &gBright );
    glBindTexture( GL_TEXTURE_2D, gBright );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA16F, WinX, WinY, 0, GL_RGBA, GL_FLOAT, NULL );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gBright, 0 );

    glGenRenderbuffers(1, &gDepthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, gDepthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WinX, WinY);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, gDepthBuffer);


    // - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
    unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
    glDrawBuffers( 4, attachments );


    // - Finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/////////////////////////////////////////////////////////////////////// SCENE

void renderQuad() {
    if (quadVAO == 0) {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void renderBasicScene() {
    Catalog<ShaderProgram*> *shaderProgramManager = Catalog<ShaderProgram*>::instance();
    // 1. render scene into floating point framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    glClearColor(0, 0, 0, 0); // make the background black and not the default grey
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene->draw();
    particlesOne->draw();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void blurBrightScene() {
    Catalog<ShaderProgram*> *shaderProgramManager = Catalog<ShaderProgram*>::instance();
    // 2. blur bright fragments with two-pass Gaussian Blur
    bool horizontal = true, first_iteration = true;
    unsigned int amount = 10;
    ShaderProgram* blur = shaderProgramManager->get("Blur");
    blur->use();
    glActiveTexture(GL_TEXTURE0);
    blur->addUniform("image", 0); // 3 because GL_texture3
    for (unsigned int i = 0; i < amount; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
        blur->addUniform("horizontal", horizontal);
        glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongColorbuffers[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)
        renderQuad();
        horizontal = !horizontal;
        if (first_iteration)
            first_iteration = false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    blur->stop();
}

void drawQuadWithScene() {
    Catalog<ShaderProgram*> *shaderProgramManager = Catalog<ShaderProgram*>::instance();
    // 3. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
    ShaderProgram* bloomFinal = shaderProgramManager->get("BloomFinal");
    bloomFinal->use();
    bloomFinal->addUniform("exposure", exposure);
    bloomFinal->addUniform("gamma", gamma);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
    bloomFinal->addUniform("scene", 0); // 0 because GL_TEXTURE0
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[0]);
    bloomFinal->addUniform("bloomBlur", 1); // 1 because GL_TEXTURE1
    renderQuad();
    bloomFinal->stop();
}

float lerp(float a, float b, float f) {
    return a + f * (b - a);
}

void renderOcclusion() {
    Catalog<ShaderProgram*> *shaderProgramManager = Catalog<ShaderProgram*>::instance();

    ShaderProgram* ssaoShader = shaderProgramManager->get("SSAO");

    // ------------------------
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
    glClear(GL_COLOR_BUFFER_BIT);
    ssaoShader->use();
    // Send kernel + rotation 
    for (unsigned int i = 0; i < 64; ++i) {
        std::string s = "samples[" + std::to_string(i) + +"]";
        ssaoShader->addUniform(s.c_str(), ssaoKernel[i]);
    }
    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glActiveTexture(GL_TEXTURE11);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glActiveTexture(GL_TEXTURE12);
    glBindTexture(GL_TEXTURE_2D, noiseTexture);
    renderQuad();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void drawDeferredScene() {
    Catalog<ShaderProgram*> *shaderProgramManager = Catalog<ShaderProgram*>::instance();
    Catalog<Texture*> *textureManager = Catalog<Texture*>::instance();

    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    glClearColor(0, 0, 0, 0); // make the background black and not the default grey
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    deferredScene->draw();
    deferredParticles->draw();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    ShaderProgram* ssaoShader = shaderProgramManager->get("SSAO");

    // ------------------------
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
    glClear(GL_COLOR_BUFFER_BIT);
    ssaoShader->use();
    // Send kernel + rotation 
    for (unsigned int i = 0; i < 64; ++i) {
        std::string s = "samples[" + std::to_string(i) + +"]";
        ssaoShader->addUniform(s.c_str(), ssaoKernel[i]);
    }

    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    ssaoShader->addUniform("gPosition", 10);
    glActiveTexture(GL_TEXTURE11);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    ssaoShader->addUniform("gNormal", 11);
    glActiveTexture(GL_TEXTURE12);
    glBindTexture(GL_TEXTURE_2D, noiseTexture);
    ssaoShader->addUniform("texNoise", 12);

    renderQuad();

    ssaoShader->stop();
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );

    glBindFramebuffer( GL_FRAMEBUFFER, hdrFBO );
    glClearColor( 0, 0, 0, 0 ); // make the background black and not the default grey
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //SSAO_BLUR
    ShaderProgram* ssaoBlurShader = shaderProgramManager->get("SSAO_BLUR");
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
    glClear(GL_COLOR_BUFFER_BIT);
    ssaoBlurShader->use();
    glActiveTexture(GL_TEXTURE16);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
    ssaoBlurShader->addUniform("ssaoInput", 16);
    renderQuad();
    ssaoBlurShader->stop();
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );


    //LIGHTPASS

    //RENDER QUAD
    ShaderProgram* renderShader = shaderProgramManager->get("DeferredSceneRender");
    renderShader->use();

    glActiveTexture( GL_TEXTURE10 );
    glBindTexture( GL_TEXTURE_2D, gPosition );
    renderShader->addUniform( "gPosition", 10 );
    glActiveTexture( GL_TEXTURE11 );

    glBindTexture( GL_TEXTURE_2D, gNormal );
    renderShader->addUniform( "gNormal", 11 );

    glActiveTexture( GL_TEXTURE12 );
    glBindTexture( GL_TEXTURE_2D, gAlbedoSpec );
    renderShader->addUniform( "gAlbedoSpec", 12 );

    glActiveTexture( GL_TEXTURE13 );
    glBindTexture( GL_TEXTURE_2D, gBright );
    renderShader->addUniform( "gBright", 13 );

    glActiveTexture( GL_TEXTURE14 );
    glBindTexture( GL_TEXTURE_2D, ssaoColorBuffer );
    renderShader->addUniform( "gSSAO", 14 );

    glActiveTexture(GL_TEXTURE15);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
    renderShader->addUniform("gSSAOBlur", 15);

    renderShader->addUniform("mode",(float) mode);

    glBindFramebuffer( GL_FRAMEBUFFER, hdrFBO );
    glClearColor( 0, 0, 0, 0 ); // make the background black and not the default grey
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    renderQuad();
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    renderShader->stop();

    blurBrightScene();

    drawQuadWithScene();



}

void drawScene() {
    // 1. render scene into floating point framebuffer
    /**/
    scene->updateModel(MatrixFactory::createScaleMatrix4(1.0f, -1.0f, 1.0f));
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);
    SCENE_NODE_MANAGER->get(SceneNode::TABLE)->disable();
    //scene->activateReflection( reflectionPlane );
    renderBasicScene();
    // 2. blur bright fragments with two-pass Gaussian Blur
    blurBrightScene();
    // 3. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
    glBindFramebuffer(GL_FRAMEBUFFER, FRAME_BUFFER_MANAGER->get(FrameBuffer::REFLECTION)->getId());
    glCullFace(GL_BACK);
    drawQuadWithScene();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    /**/
    //renderTextures();
    // 1. render scene into floating point framebuffer

    SCENE_NODE_MANAGER->get(SceneNode::TABLE)->enable();
    scene->updateModel(MatrixFactory::createScaleMatrix4(1.0f, -1.0f, 1.0f));
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    //scene->deactivateReflection();
    renderBasicScene();
    // 2. blur bright fragments with two-pass Gaussian Blur
    blurBrightScene();

    drawQuadWithScene();

    checkOpenGLError("ERROR: Could not draw scene.");
}

/////////////////////////////////////////////////////////////////////// CALLBACKS

void cleanup() {
    destroyShaderProgram();
    destroyMeshes();
    destroyTextures();
}

void display() {
    ++FrameCount;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (deferred) {
        drawDeferredScene();
    } else {
        drawScene();
    }
    glutSwapBuffers();
}

void idle() {
    update();
    float currentFrame = (float)glutGet(GLUT_ELAPSED_TIME);
    delta = ((float)currentFrame - (float)lastFrame) / 100;
    lastFrame = (float)currentFrame;
    scene->update(delta);
    particlesOne->update(delta);
    deferredParticles->update(delta);

    glutPostRedisplay();
}

void reshape(int w, int h) {
    WinX = w;
    WinY = h;
    glViewport(0, 0, WinX, WinY);
    if (lastWinX != WinX || lastWinY != WinY) {
        projectionMatrix = MatrixFactory::createPerspectiveProjectionMatrix(30, (float)WinX / (float)WinY, 1, 30);
        camera->ProjectionMatrix(projectionMatrix);
        freeCamera->ProjectionMatrix(projectionMatrix);
        lastWinX = WinX;
        lastWinY = WinY;
    }
}

void timer(int value) {
    std::ostringstream oss;
    oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")";
    std::string s = oss.str();
    glutSetWindow(WindowHandle);
    glutSetWindowTitle(s.c_str());
    FrameCount = 0;
    glutTimerFunc(1000, timer, 0);
}

/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks() {
    glutCloseFunc(cleanup);
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyPress);
    glutKeyboardUpFunc(keyRelease);
    glutSpecialFunc(specialKeyPress);
    glutSpecialUpFunc(specialKeyRelease);
    glutMotionFunc(mouse_input);
    glutMouseWheelFunc(mouse_wheel_input);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);
    setupErrors();
}

void checkOpenGLInfo() {
    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *vendor = glGetString(GL_VENDOR);
    const GLubyte *version = glGetString(GL_VERSION);
    const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    std::cerr << "OpenGL Renderer: " << renderer << " (" << vendor << ")" << std::endl;
    std::cerr << "OpenGL version " << version << std::endl;
    std::cerr << "GLSL version " << glslVersion << std::endl;
}

void setupOpenGL() {
    checkOpenGLInfo();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_TRUE);
    glDepthRange(0.0, 1.0);
    glClearDepth(1.0);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

void setupGLEW() {
    glewExperimental = GL_TRUE;
    GLenum result = glewInit();
    if (result != GLEW_OK) {
        std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
        exit(EXIT_FAILURE);
    }
    GLenum err_code = glGetError();
}

void setupGLUT(int argc, char* argv[]) {
    glutInit(&argc, argv);

    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextFlags(GLUT_DEBUG);

    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

    glutInitWindowSize(WinX, WinY);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    WindowHandle = glutCreateWindow(CAPTION);
    if (WindowHandle < 1) {
        std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
        exit(EXIT_FAILURE);
    }
}

void setupCamera() {
    camera = new FixedCamera(vec3(0, 0, 5), vec3(0, 0, 0), vec3(0, 1, 0));
    freeCamera = new FreeCamera(vec3(0, 0, 5), vec3(0, 0, 0), vec3(0, 1, 0));
    camera->ProjectionMatrix(projectionMatrix);
    freeCamera->ProjectionMatrix(projectionMatrix);
}

void loadMeshes() {
    Catalog<Mesh*>* meshManager = Catalog<Mesh*>::instance();
    meshManager->insert(Mesh::QUAD, meshLoader.createMesh(std::string("Mesh/Quad.obj")));
    meshManager->insert(Mesh::SPHERE, meshLoader.createMesh(std::string("Mesh/Sphere.obj")));
    meshManager->insert(Mesh::CUBE_SKYBOX, meshLoader.createMesh(std::string("Mesh/skybox.obj")));
    meshManager->insert(Mesh::SPHERE_SKYBOX, meshLoader.createMesh(std::string("Mesh/sphereSkybox.obj")));
    meshManager->insert(Mesh::NOODLES, meshLoader.createMesh(std::string("Mesh/Noodles_mesh.obj")));
}

void loadTextures() {

    Catalog<Texture*>* textureCatalog = Catalog<Texture*>::instance();
    textureCatalog->insert( Texture::NOODLE_TEXTURE, new Texture( "Textures/noodle_texture_simple.jpg" ) );
    textureCatalog->insert( Texture::NOODLE_MAP_NORMAL, new Texture( "Textures/noodle_normal.jpg" ) );
    textureCatalog->insert( Texture::NOODLE_MAP_SPECULAR, new Texture( "Textures/noodle_specular.jpg" ) );

    textureCatalog->insert( Texture::TABLE, new Texture( "Textures/wood.jpg" ) );
    textureCatalog->insert( Texture::TABLE_NORMAL, new Texture( "Textures/wood_normal.jpg" ) );
    textureCatalog->insert( Texture::TABLE_SPECULAR, new Texture( "Textures/wood_specular.jpg" ) );

    textureCatalog->insert( Texture::BEACH_BOX, new TextureCube( "Textures/skybox/kitchen/", ".png", true, true ) );

    textureCatalog->insert(Texture::REFLECTION_RENDER_TEXTURE, new RenderTexture(WinX, WinY));

}

void createFrameBuffers() {
    FRAME_BUFFER_MANAGER->insert(FrameBuffer::REFLECTION, new FrameBuffer(Texture::REFLECTION_RENDER_TEXTURE, GL_TEXTURE_2D));
    glGenFramebuffers(1, &ssaoFBO);  glGenFramebuffers(1, &ssaoBlurFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
    // SSAO color buffer
    glGenTextures(1, &ssaoColorBuffer);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, WinX, WinY, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "SSAO Framebuffer not complete!" << std::endl;
    // and blur stage
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
    glGenTextures(1, &ssaoColorBufferBlur);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, WinX, WinY, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "SSAO Blur Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void createDeferredScene() {
    Catalog<SceneNode*>* sceneNodeManager = Catalog<SceneNode*>::instance();
    Catalog<Mesh*>* meshManager = Catalog<Mesh*>::instance();
    Catalog<ShaderProgram*> *shaderProgramManager = Catalog<ShaderProgram*>::instance();
    ShaderProgram* dfault = shaderProgramManager->get("default");

    deferredScene = new Scene(dfault, camera);

    TextureInfo* noodleTextureInfo = new TextureInfo(Texture::NOODLE_TEXTURE, "noodleTex", GL_TEXTURE2, 2);
    TextureInfo* noodleNormalInfo = new TextureInfo(Texture::NOODLE_MAP_NORMAL, "noodleNormal", GL_TEXTURE3, 3);
    TextureInfo* noodleSpecularInfo = new TextureInfo(Texture::NOODLE_MAP_SPECULAR, "noodleSpec", GL_TEXTURE4, 4);

    TextureInfo* skyboxTexture = new TextureInfo( Texture::BEACH_BOX, "skybox", GL_TEXTURE6, 6 );
    SceneNode  *skyboxers = new SceneNode( meshManager->get( Mesh::SPHERE_SKYBOX ), shaderProgramManager->get( "DeferredSkybox" ),
        MatrixFactory::createScaleMatrix4( 7.0f, 7.0f, 7.0f ) );
    skyboxers->addTexture( skyboxTexture );
    deferredScene->addNode( skyboxers );
    sceneNodeManager->insert( "DEFERRED_SKYBOX", skyboxers );

    SceneNode  *noodles = new SceneNode( meshManager->get( Mesh::NOODLES ), shaderProgramManager->get( "DeferredBloom" ),
        MatrixFactory::createTranslationMatrix(vec3(0.0f,-0.1f,0.0f) )*MatrixFactory::createScaleMatrix4( 0.8f, 0.8f, 0.8f ) );
    noodles->addTexture( noodleTextureInfo );
    noodles->addTexture( noodleNormalInfo );
    noodles->addTexture( noodleSpecularInfo );
    deferredScene->addNode( noodles );
    sceneNodeManager->insert( "DEFERRED_NOODLES", noodles );

    TextureInfo* tableTextureInfo = new TextureInfo( Texture::TABLE, "noodleTex", GL_TEXTURE2, 2 );
    TextureInfo* tableNormalInfo = new TextureInfo( Texture::TABLE_NORMAL, "noodleNormal", GL_TEXTURE3, 3 );
    TextureInfo* tableSpecularInfo = new TextureInfo( Texture::TABLE_SPECULAR, "noodleSpec", GL_TEXTURE4, 4 );

    SceneNode  *table = new SceneNode( meshManager->get( Mesh::QUAD ), shaderProgramManager->get( "DeferredBloom" ),
        MatrixFactory::createTranslationMatrix( vec3( 0.0f, -0.1f, 0.0f ) ) * 
        MatrixFactory::createScaleMatrix4( 1.0f, 1.0f, 1.0f ) *
        MatrixFactory::createRotationMatrix4(90 ,ZZ));
    table->addTexture( tableTextureInfo );
    table->addTexture( tableNormalInfo );
    table->addTexture( tableSpecularInfo );
    deferredScene->addNode( table );
    sceneNodeManager->insert( "DEFERRED_TABLE", table );


    deferredParticles = new ParticleSystemTransform( shaderProgramManager->get( "DeferredTFBDraw" ),
        shaderProgramManager->get( "TFBUpdate" ), camera, vec3( 0.0f, 0.0f, 0.0f ) );

    deferredParticles = new ParticleSystemTransform(shaderProgramManager->get("DeferredTFBDraw"),
                                                    shaderProgramManager->get("TFBUpdate"), camera, vec3(0.0f, 0.0f, 0.0f));
    deferredParticles->InitParticleSystem();

    checkOpenGLError("ERROR: Could not create ParticleSystemTwo.");
}

void createSceneMapping() {
    Catalog<SceneNode*>* sceneNodeManager = Catalog<SceneNode*>::instance();
    Catalog<Mesh*>* meshManager = Catalog<Mesh*>::instance();
    Catalog<ShaderProgram*> *shaderProgramManager = Catalog<ShaderProgram*>::instance();
    ShaderProgram* dfault = shaderProgramManager->get("default");

    scene = new Scene(dfault, camera);

    //GL_TEXTURE0 is used for hdr
    //GL_TEXTURE1 is used for saving brightnessvalues and blurring
    /**/
    TextureInfo* noodleTextureInfo = new TextureInfo(Texture::NOODLE_TEXTURE, "noodleTex", GL_TEXTURE2, 2);
    TextureInfo* noodleNormalInfo = new TextureInfo(Texture::NOODLE_MAP_NORMAL, "noodleNormal", GL_TEXTURE3, 3);
    TextureInfo* noodleSpecularInfo = new TextureInfo(Texture::NOODLE_MAP_SPECULAR, "noodleSpec", GL_TEXTURE4, 4);

    SceneNode  *noodles = new SceneNode(meshManager->get(Mesh::SPHERE), shaderProgramManager->get("Bloom"),
                                        MatrixFactory::createScaleMatrix4(0.2f, 0.2f, 0.2f));
    noodles->addTexture(noodleTextureInfo);
    noodles->addTexture(noodleNormalInfo);
    noodles->addTexture(noodleSpecularInfo);
    scene->addNode(noodles);
    sceneNodeManager->insert(SceneNode::NOODLES, noodles);
    /**/
    TextureInfo* reflectionRenderTextureInfo = new TextureInfo(Texture::REFLECTION_RENDER_TEXTURE, "reflection", GL_TEXTURE5, 5);
    SceneNode  *table = new SceneNode(meshManager->get(Mesh::QUAD), shaderProgramManager->get("ReflectionShader"),
                                      MatrixFactory::createTranslationMatrix(vec3(0.0f, -0.2f, 0.0f)) *
                                      MatrixFactory::createScaleMatrix4(1.0f, 0.0f, 1.0f) *
                                      //MatrixFactory::createRotationMatrix4( 180, YY ) *
                                      MatrixFactory::createRotationMatrix4(90, ZZ));
    table->addTexture(reflectionRenderTextureInfo);
    scene->addNode(table);
    sceneNodeManager->insert(SceneNode::TABLE, table);

    reflectionPlane = vec4(YY, 0);
    /**/
    TextureInfo* skyboxTexture = new TextureInfo(Texture::BEACH_BOX, "skybox", GL_TEXTURE6, 6);

    SceneNode  *skybox = new SceneNode(meshManager->get(Mesh::SPHERE_SKYBOX), shaderProgramManager->get("SkyBox"),
                                       MatrixFactory::createScaleMatrix4(5.0f, 5.0f, 5.0f));
    skybox->addTexture(skyboxTexture);
    //skybox->addTexture( reflectionRenderTextureInfo );
    scene->addNode(skybox);
    sceneNodeManager->insert(SceneNode::SKY_BOX, skybox);

    /**/
}

void createParticleSystem() {
    Catalog<ShaderProgram*> *shaderProgramManager = Catalog<ShaderProgram*>::instance();

    particlesOne = new ParticleSystemTransform(shaderProgramManager->get("TFBDraw"),
                                               shaderProgramManager->get("TFBUpdate"), camera, vec3(0.0f, 0.0f, 0.0f));
    particlesOne->InitParticleSystem();
    checkOpenGLError("ERROR: Could not create ParticleSystemTwo.");

}

void setupLight() {
    float scale = 0.05f;
    mat4 boxScale = MatrixFactory::createScaleMatrix4(scale, scale, scale);
    Catalog<Mesh*>* meshManager = Catalog<Mesh*>::instance();
    Catalog<ShaderProgram*> *shaderProgramManager = Catalog<ShaderProgram*>::instance();
    SceneNode* lights = new SceneNode( nullptr, shaderProgramManager->get( "default" ) );
    SceneNode* deferredLights = new SceneNode( nullptr, shaderProgramManager->get( "default" ) );
    SceneNode* temp, *deferredTemp;

    vec3 pos;
    vec3 color;
    int i = 0;
    float endX = 0.3f;
    float beginX = -0.3f;
    float offset = (endX - beginX) / (NR_POINT_LIGHTS - 1);
    pos = vec3(beginX, 0.7f, 0.0f);
    vec3 dropoff = vec3(10.0f, 5.0f, 50.0f);

    color = vec3( 186.0f, 85.0f, 211.0f )*( 1 / 50.0f );
    //color = vec3( 148.0f, 0.0f, 211.0f)*( 1 / 15.0f );
    vec3 ambient = color;
    vec3 diffuse = color;
    vec3 specular = vec3(0.1f, 0.1f, 0.1f);

    for (int j = i; j < NR_POINT_LIGHTS; j++, i++) {
        pointLights[j] = PointLight(pos, dropoff, ambient, diffuse, specular);
        temp = new SceneNode(meshManager->get(Mesh::SPHERE), shaderProgramManager->get("LightBox"),
                             MatrixFactory::createTranslationMatrix(pos) * boxScale);
        pos.x += offset;
        temp->setColor( vec4( color, 1.0f ) );
        lights->addNode( temp );
        deferredTemp = new SceneNode( meshManager->get( Mesh::SPHERE ), shaderProgramManager->get( "DeferredLightBox" ),
            MatrixFactory::createTranslationMatrix( pos ) * boxScale );
        deferredTemp->setColor( vec4( color, 1.0f ) );
        deferredLights->addNode( deferredTemp );
    }
    /**/
    scene->addNode( lights );
    deferredScene->addNode( deferredLights );
    SCENE_NODE_MANAGER->insert( SceneNode::LIGHTS, lights );
    SCENE_NODE_MANAGER->insert( "DEFERRED_LIGHTS_BOXES", lights );
}

void activateLights() {
    Catalog<ShaderProgram*> *shaderProgramManager = Catalog<ShaderProgram*>::instance();
    ShaderProgram* shader = shaderProgramManager->get("TFBDraw");

    shader->use();
    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        pointLights[i].addItself(shader, i);
    }
    shader->stop();

    shader = shaderProgramManager->get("Bloom");

    shader->use();
    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        pointLights[i].addItself(shader, i);
    }
    shader->stop();

    shader = shaderProgramManager->get("DeferredSceneRender");

    shader->use();
    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        pointLights[i].addItself(shader, i);
    }
    shader->stop();

}

void setupHDR() {
    // configure floating point framebuffer
    glGenFramebuffers(1, &hdrFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    // create floating point color buffer 2 of them
    // This is so shaders can output 2 colors, the basic fragment color
    // and the brightness of that fragment to know if we need to blur or not
    glGenTextures(2, colorBuffers);
    for (unsigned int i = 0; i < 2; i++) {
        glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WinX, WinY, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // attach buffers
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
    }
    // create depth buffer (renderbuffer)
    // for a framebuffer to be complete it needs to have a depth buffer
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WinX, WinY);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);

    // - Finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // ping-pong-framebuffer for blurring
    glGenFramebuffers(2, pingpongFBO);
    glGenTextures(2, pingpongColorbuffers);
    for (unsigned int i = 0; i < 2; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WinX, WinY, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
        // also check if frameBuffers are complete (no need for depth buffer)
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "FrameBuffer not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void setUpSSAO() {

    // generate sample kernel
// ----------------------
    std::uniform_real_distribution<GLfloat> randomFloats(0.0f, 1.0f); // generates random floats between 0.0 and 1.0
    std::default_random_engine generator;
    for (unsigned int i = 0; i < 64; ++i) {
        vec3 sample(randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator));
        sample = normalize(sample);
        sample *= randomFloats(generator);
        float scale = float(i) / 64.0f;

        // scale samples s.t. they're more aligned to center of kernel
        scale = lerp(0.1f, 1.0f, scale * scale);
        sample *= scale;
        ssaoKernel.push_back(sample);
    }

    // generate noise texture
    // ----------------------
    std::vector<vec3> ssaoNoise;
    for (unsigned int i = 0; i < 16; i++) {
        vec3 noise(randomFloats(generator) * 2.0f - 1.0f,
                   randomFloats(generator) * 2.0f - 1.0f,
                   0.0f); // rotate around z-axis (in tangent space)
        ssaoNoise.push_back(noise);
    }
    glGenTextures(1, &noiseTexture);
    glBindTexture(GL_TEXTURE_2D, noiseTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

}

void init(int argc, char* argv[]) {
    setupGLUT(argc, argv);
    setupGLEW();
    setupOpenGL();

    setupCamera();
    setupCallbacks();
    float tBefore, tAfter,tD;
    std::cout << "Loading Textures" << std::endl;
    tBefore = ( float )glutGet( GLUT_ELAPSED_TIME );
    loadTextures();
    tAfter = ( float )glutGet( GLUT_ELAPSED_TIME );
    tD = ( ( float )tAfter - ( float )tBefore ) / 1000.0f;
    std::cout << "Finished Textures time: "<< tD << std::endl;

    std::cout << "Loading Meshes" << std::endl;
    tBefore = ( float )glutGet( GLUT_ELAPSED_TIME );
    loadMeshes();
    tAfter = ( float )glutGet( GLUT_ELAPSED_TIME );
    tD = ( ( float )tAfter - ( float )tBefore ) / 1000.0f;
    std::cout << "Finished Meshes time: " << tD << std::endl;

    setupHDR();
    createShaderProgram();
    createDeferredShaderProgram();
    createFrameBuffers();
    createGBuffer();

    setUpSSAO();
    createSceneMapping();
    createDeferredScene();
    createParticleSystem();

    setupLight();
    activateLights();

    std::cout << "Mode " << mode << ": Normal mode with blured occlusion" << std::endl;
}

int main(int argc, char* argv[]) {

    init(argc, argv);
    glutMainLoop();
    exit(EXIT_SUCCESS);

}
