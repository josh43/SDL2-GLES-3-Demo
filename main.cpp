//
//  main.cpp
//  ___PROJECTNAME___
//
//  Created by   on 8/14/15.
//  Use however you would like, if you want the demo to draw a triangle
//  simply uncomment //#define TRIANGLE_DEMO
//
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#include "SDL.h"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "Shader.h"

//#define TRIANGLE_DEMO


static const GLfloat cube_vertices[] = {
    // front
    -1.0, -1.0,  1.0,
    1.0, -1.0,  1.0,
    1.0,  1.0,  1.0,
    -1.0,  1.0,  1.0,
    // back
    -1.0, -1.0, -1.0,
    1.0, -1.0, -1.0,
    1.0,  1.0, -1.0,
    -1.0,  1.0, -1.0,
};
static const GLushort cube_elements[] = {
    // front
    0, 1, 2,
    2, 3, 0,
    // top
    3, 2, 6,
    6, 7, 3,
    // back
    7, 6, 5,
    5, 4, 7,
    // bottom
    4, 5, 1,
    1, 0, 4,
    // left
    4, 0, 3,
    3, 7, 4,
    // right
    1, 5, 6,
    6, 2, 1,
};
static const GLfloat triangle[] = {
    -.50f, .50f, 0.0f,
    0.0f, .50f, 0.0f,
    0.0f,  0.0f, 0.0f,
};
void update(void * window){
    
}
void cubeDrawer(SDL_Window * window, SDL_DisplayMode &display){
    SDL_Window * win = ((SDL_Window *)window);
    
    glm::mat4 Model = glm::mat4();
    glm::mat4 View = glm::lookAt(glm::vec3(3.0f,0.0f,3.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));
    glm::mat4 Projection = glm::perspective(65.0f, (float)display.w/(float)display.h, .01f, 1000.0f);
    
    glm::mat4 ModelView = Projection * View;
    
    GLuint vao,vbo, ibo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);
    
    Shader cubeShader("cubeShader");
    
    GLuint mvpLocation = glGetUniformLocation(cubeShader.getProgramID(), "MVP");
    
    glm::mat4 rotatingMVP;
    
    while(true){
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       
        Model = glm::rotate(Model, .05f, glm::vec3(1.0f,0.0f,1.0f));
        rotatingMVP = ModelView * Model;
        
        glUniformMatrix4fv(mvpLocation,1,GL_FALSE,&rotatingMVP[0][0]);
        // Oddly enough If I do not include these next 2 lines
        // it will not even clear the color of the backscreen!
        SDL_Event e;
        SDL_PollEvent(&e);
        
        
        cubeShader.bind(); // glUseProgram(my_shader_id);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, sizeof(cube_elements)/sizeof(cube_elements[0]),GL_UNSIGNED_SHORT, 0);
        
        glFlush();
        SDL_GL_SwapWindow(window);
    }

    
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
}
void triangleDrawer(SDL_Window  * window,SDL_DisplayMode & display){
     Shader tShader("triangleShader");
    GLuint vao, vbo;
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle),triangle,GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    

    
    //SDL_iPhoneSetAnimationCallback(window, 10, update, nullptr);
    while(true){
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Oddly enough If I do not include these next 2 lines
        // it will not even clear the color of the backscreen!
        SDL_Event e;
        SDL_PollEvent(&e);
        
        
        tShader.bind(); // glUseProgram(my_shader_id);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glFlush();
        SDL_GL_SwapWindow(window);
    }
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}
int
main(int argc, char *argv[])
{
    
    SDL_Window *window = nullptr;
    
    if (SDL_Init(SDL_INIT_VIDEO) !=0) {
        printf("Could not initialize SDL\n");
        return 1;
    }
    if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES)!=0){
        std::cout << "ERROR loading context profile mask\n";
    }
    if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3)!=0){
        std::cout <<"ERROR setting context major version\n";
    }
    if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0)!=0){
        std::cout <<"ERROR setting context minor version\n";
    }
    if(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) != 0){
        std::cout << " ERrror \n setting double buffer";
    } // I have tried without the dubble buffer and nothing changes
    if(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE   , 24) != 0){
        std::cout << " ERrror \n setting depth buffer";
    }
    
    
    SDL_DisplayMode display;
    SDL_GetDesktopDisplayMode(0, &display);
    
  
    window = SDL_CreateWindow(NULL, 0, 0, display.w, display.h, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);
    assert(window != nullptr);
    
    SDL_GLContext  glContext =  nullptr;
    glContext = SDL_GL_CreateContext(window);
    
    assert(glContext != nullptr);
    glViewport(0,0,display.w,display.h);// If you dont set this than nothing will draw
    // VERY IMPORTANT ^^^

    
    
    
#ifdef TRIANGLE_DEMO
    triangleDrawer(window, display);
#else
    cubeDrawer(window,display);
    
#endif

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);

    return 1;
}