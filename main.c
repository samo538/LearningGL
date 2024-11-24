/*
*   file OpenGL learning file
*   author Samuel Luptak
*/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "include/glad/glad.h"
#include <GLFW/glfw3.h>

#define error(x) fprintf(stderr, x); glfwTerminate(); return -1;

// Vertex shader
const char *vs = "#version 330 core\n" 
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fs1 = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";
const char *fs2 = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
    "}\0";

void processInput(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}

int main(){
    // Initialises glfw
    glfwInit();

    // Sets window parameters before creating it
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Window creation
    GLFWwindow* window = glfwCreateWindow(800, 600, "TestingWindow", NULL, NULL);
    if (window == NULL) {
        error("Window creation failed")
    }

    // Setting the context to the window
    glfwMakeContextCurrent(window);

    // Glad initialisation
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        error("Glad init falied")
    }

    // OpenGL API function call
    glViewport(0,0,800,600);

    // Initializing shaders and data
    float verticies1[] = {
         0.5f,  0.5f, 0.0f, // top right
         0.5f, -0.5f, 0.0f, // bottom right
        -0.5f,  0.5f, 0.0f, // top left
    };
    float verticies2[] = {
         0.5f, -0.5f, 0.0f, // bottom right
        -0.5f,  0.5f, 0.0f, // top left
        -0.5f, -0.5f, 0.0f, // bottom left
    };
    unsigned int *VAO = malloc(sizeof(unsigned int)); // Vertex array object
    unsigned int *VBO = malloc(sizeof(unsigned int)); // Vertex buffer object
    glGenBuffers(2, VBO); // Generate the buffer object and store it in the VBO variable
    glGenVertexArrays(2, VAO); 

    glBindVertexArray(VAO[0]);
    // The first VBO stores the first triangle
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]); // Select the VBO buffer to be GL_ARRAY_BUFFER type
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies1), verticies1, GL_STATIC_DRAW); // Send data to the GPU

    // Setting the attributes pointers to the VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // Setting the attribute pointer
    glEnableVertexAttribArray(0); // Enabling the attribute pointer


    glBindVertexArray(VAO[1]);
    // The second VBO stores the second trinagle
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]); // Select the VBO buffer to be GL_ARRAY_BUFFER type
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies2), verticies2, GL_STATIC_DRAW); // Send data to the GPU

    // Setting the attributes pointers to the VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // Setting the attribute pointer
    glEnableVertexAttribArray(0); // Enabling the attribute pointer

    // As we are not going to modify VAO, VBO or EBO we can safely unbind them (as not to unwantedly change them)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned int VS; // Vertex shader
    VS = glCreateShader(GL_VERTEX_SHADER); // Creation of vertex shader object
    glShaderSource(VS, 1, &vs, NULL); // Initializing vertex shader object
    glCompileShader(VS); // Run-time compilation of the vertex shader

    unsigned int FS1; // Fragment shader
    FS1 = glCreateShader(GL_FRAGMENT_SHADER); // Creation of fragment shader object
    glShaderSource(FS1, 1, &fs1, NULL); // Initializing fragment shader object
    glCompileShader(FS1); // Run-time compilation of the fragment shader

    unsigned int FS2; // Fragment shader
    FS2 = glCreateShader(GL_FRAGMENT_SHADER); // Creation of fragment shader object
    glShaderSource(FS2, 1, &fs2, NULL); // Initializing fragment shader object
    glCompileShader(FS2); // Run-time compilation of the fragment shader

    unsigned int SP1; // Final shader program that integrates other compiled shaders
    SP1 = glCreateProgram(); // Creation of the shader program
    glAttachShader(SP1, VS); // Attaching shader programs
    glAttachShader(SP1, FS1);
    glLinkProgram(SP1); // Linking attached shaders

    unsigned int SP2; // Final shader program that integrates other compiled shaders
    SP2 = glCreateProgram(); // Creation of the shader program
    glAttachShader(SP2, VS); // Attaching shader programs
    glAttachShader(SP2, FS2);
    glLinkProgram(SP2); // Linking attached shaders

    glDeleteShader(VS); // Deletion of shaders that will no longer be used
    glDeleteShader(FS1);
    glDeleteShader(FS2);

    // Main render loop
    while(!glfwWindowShouldClose(window)) {
        // input
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(SP1); // Using the shader program (every rendering call will use these shaders)

        // Drawing the first trinagle
        glBindVertexArray(VAO[0]); // We do not need to bind it every time, but its a good practice
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(SP2); // Using the shader program (every rendering call will use these shaders)
        // Drawing the second trinagle
        glBindVertexArray(VAO[1]); // We do not need to bind it every time, but its a good practice
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swapping buffers (double buffer trick)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Terminating glfw
    glfwTerminate();
    return 0;
}
