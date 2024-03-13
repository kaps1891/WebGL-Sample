/*

*/

//#include <GL/gl.h>
#include <stdio.h>
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <GLES3/gl3.h>

#include <emscripten.h>
#include <emscripten/html5.h>
#include <emscripten/threading.h>
#include <pthread.h>
#include <emscripten/html5_webgpu.h>
#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <unistd.h> // For sleep function

GLuint program;
GLuint vao;
GLuint vbo;

pthread_t renderingThreadId = 0, intermediate_th_id = 0;
EMSCRIPTEN_WEBGL_CONTEXT_HANDLE _context;
void init() {
    
    EmscriptenWebGLContextAttributes attrs;
    emscripten_webgl_init_context_attributes(&attrs);
    attrs.enableExtensionsByDefault = 1;
    // attrs.explicitSwapControl = true;
    attrs.powerPreference = EM_WEBGL_POWER_PREFERENCE_HIGH_PERFORMANCE;
    attrs.proxyContextToMainThread = EMSCRIPTEN_WEBGL_CONTEXT_PROXY_DISALLOW;
    // attrs.renderViaOffscreenBackBuffer = true;
    attrs.depth = 1;
    attrs.stencil = 1;
    attrs.antialias = 1;
    attrs.majorVersion = 2;
    attrs.minorVersion = 0;
    _context = emscripten_webgl_create_context("#myCanvas", &attrs);
    if (_context > 0)
    {
        printf("\n\nCreation of context succeeded!!!\n\n");
    }
    else
    {
         printf("\n\nCreation of context failed!!!\n\n");
    }

    emscripten_webgl_make_context_current(_context);

    // Clear the screen to green
   // glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    // Create shaders
    const char* vertexShaderSource = "#version 300 es\n"
                                     "precision mediump float;\n"
                                     "layout (location = 0) in vec2 position;\n"
                                     "void main() {\n"
                                     "    gl_Position = vec4(position, 0.0, 1.0);\n"
                                     "}";

    const char* fragmentShaderSource = "#version 300 es\n"
                                       "precision mediump float;\n"
                                       "out vec4 FragColor;\n"
                                       "void main() {\n"
                                       "    FragColor = vec4(1.0, 1.0, 0.0, 1.0);\n"
                                       "}";

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Create shader program
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Create VAO and VBO
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    GLfloat vertices[] = {
        -1.0f, -1.0f,
         1.0f, -1.0f,
         0.0f,  1.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void render() {
    // glClear(GL_COLOR_BUFFER_BIT);

    // glUseProgram(program);
    // glBindVertexArray(vao);

    // glDrawArrays(GL_TRIANGLES, 0, 3);

    // glBindVertexArray(0);
    // glUseProgram(0);
}



void *renderingThreadEntry(void *arg)
{
    init();
    emscripten_set_main_loop(render, 0, 1);
    return nullptr;
}

int main() 
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    emscripten_pthread_attr_settransferredcanvases(&attr, "#myCanvas");
    pthread_create(&renderingThreadId, &attr, renderingThreadEntry, (void *)nullptr);
    return 0;
}

void intermediateThreadEntry(void *arg)
{
    
    // emscripten_webgl_make_context_current(_context);
    printf("Intermediate Thread Entry\n");
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glBindVertexArray(vao);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);
    glUseProgram(0);
}



extern "C" EMSCRIPTEN_KEEPALIVE void myFunction(int argc, char ** argv) {
    printf("MyFunction Called\n");
    emscripten_dispatch_to_thread_async(renderingThreadId, EM_FUNC_SIG_VI,
                                        intermediateThreadEntry, nullptr,
                                        nullptr);
}


