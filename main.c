/*
*   file OpenGL learning file
*   author Samuel Luptak
*/
#include <stdbool.h>
#include <stdio.h>

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
const char *fs = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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
    float verticies[] = {
         0.5f,  0.5f, 0.0f, // top right
         0.5f, -0.5f, 0.0f, // bottom right
        -0.5f,  0.5f, 0.0f, // top left
        -0.5f, -0.5f, 0.0f, // bottom left
    };
    unsigned int indicies[] = {
        0, 1, 2, // first tringle
        1, 3, 2, // second tringle
    };
    unsigned int VAO; // Vertex array object
    glGenVertexArrays(1, &VAO); 
    glBindVertexArray(VAO);

    unsigned int VBO; // Vertex buffer object
    glGenBuffers(1, &VBO); // Generate the buffer object and store it in the VBO variable
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Select the VBO buffer to be GL_ARRAY_BUFFER type
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW); // Send data to the GPU

    unsigned int EBO; // Element buffer object
    glGenBuffers(1, &EBO); // Generate the buffer object and store it in the EBO variable
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Select the EBO buffer to be GL_ELEMENT_ARRAY_BUFFER type
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW); // Send data to the GPU

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // Setting the attribute pointer
    glEnableVertexAttribArray(0); // Enabling the attribute pointer

    // As we are not going to modify VAO, VBO or EBO we can safely unbind them (as not to unwantedly change them)
    // Warning, the EBO is not changed after this, but it must remain bound
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned int VS; // Vertex shader
    VS = glCreateShader(GL_VERTEX_SHADER); // Creation of vertex shader object
    glShaderSource(VS, 1, &vs, NULL); // Initializing vertex shader object
    glCompileShader(VS); // Run-time compilation of the vertex shader

    unsigned int FS; // Fragment shader
    FS = glCreateShader(GL_FRAGMENT_SHADER); // Creation of fragment shader object
    glShaderSource(FS, 1, &fs, NULL); // Initializing fragment shader object
    glCompileShader(FS); // Run-time compilation of the fragment shader

    unsigned int SP; // Final shader program that integrates other compiled shaders
    SP = glCreateProgram(); // Creation of the shader program
    glAttachShader(SP, VS); // Attaching shader programs
    glAttachShader(SP, FS);
    glLinkProgram(SP); // Linking attached shaders


    glDeleteShader(VS); // Deletion of shaders that will no longer be used
    glDeleteShader(FS);

    int noAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &noAttributes);
    printf("max Attribs in vertex shader: %d\n", noAttributes);

    // Main render loop
    while(!glfwWindowShouldClose(window)) {
        // input
        processInput(window);

        glUseProgram(SP); // Using the shader program (every rendering call will use these shaders)

        glBindVertexArray(VAO); // We do not need to bind it every time, but its a good practice

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swapping buffers (double buffer trick)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Freeing arrays
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(SP);

    // Terminating glfw
    glfwTerminate();
    return 0;
}
