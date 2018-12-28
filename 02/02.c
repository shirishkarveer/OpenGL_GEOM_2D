#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


static void glfwErrorCallback(int error, const char *desc) {
    fprintf(stderr, "GLFW error 0x%08X: %s\n", error, desc);
}

static GLuint loadShader(const char *fn, GLenum shaderType) {
    printf("Compiling shader '%s'...\n", fn);

    GLuint shaderID = glCreateShader(shaderType);
    if (!shaderID) {
        fprintf(stderr, "Failed to create shader\n");
        exit(1);
    }

    FILE *f = fopen(fn, "r");
    if (!f) {
        perror("Failed to load shader file");
        exit(1);
    }
    if (fseek(f, 0, SEEK_END)) {
        perror("Failed to get file size");
        exit(1);
    }
    GLint size = ftell(f);
    if (size == -1) {
        perror("Failed to get file size");
        exit(1);
    }
    rewind(f);
    char *source = (char*)malloc(size);
    if (!source) {
        perror("Failed to allocate source memory");
        exit(1);
    }
    if (fread(source, 1, size, f) != size) {
        perror("Failed to read file");
        exit(1);
    }
    if (fclose(f))
        perror("Warning: failed to close source file");

    const GLchar *rosource = source;
    glShaderSource(shaderID, 1, &rosource, &size);
    free(source);

    glCompileShader(shaderID);

    GLint logLength;
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength) {
        GLchar *log = (GLchar*)malloc(logLength);
        if (!log) {
            perror("Couldn't allocate shader compile log");
            exit(1);
        }
        glGetShaderInfoLog(shaderID, logLength, NULL, log);
        printf("Shader compile message: %s\n", log);
        free(log);
    }

    GLint status;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
    if (!status)
        exit(1);

    return shaderID;
}

static GLuint loadShaders(const char *vertex_fn, const char *fragment_fn) {
    // Compile the shaders
    GLuint vertexShaderID = loadShader(vertex_fn, GL_VERTEX_SHADER),
           fragmentShaderID = loadShader(fragment_fn, GL_FRAGMENT_SHADER);

    puts("Linking shader program...");

    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    // Check the program
    GLint logLength;
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        char *log = (char*)malloc(logLength);
        if (!log) {
            perror("Couldn't allocate shader compile log");
            exit(1);
        }
        glGetProgramInfoLog(programID, logLength, NULL, log);
        printf("Shader link message: %s\n", log);
        free(log);
    }

    GLint status;
    glGetProgramiv(programID, GL_LINK_STATUS, &status);
    if (!status)
        exit(1);

    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return programID;
}

static void init(GLFWwindow **window) {
    // Set error callback to see more detailed failure info
    glfwSetErrorCallback(glfwErrorCallback);

    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(1);
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing

    // To ensure compatiblity, check the output of this command:
    // $ glxinfo | grep 'Max core profile version'
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // To make MacOS happy
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // We don't want the old OpenGL
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    *window = glfwCreateWindow(1024, 768,
        "Tutorial 02 - Red triangle", NULL, NULL);
    if (!*window) {
        fputs("Failed to open GLFW window.\n", stderr);
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(*window);

    glewExperimental = true; // Needed in core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        glfwTerminate();
        exit(1);
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(*window, GLFW_STICKY_KEYS, GL_TRUE);

    // Dark blue background
    glClearColor(0.0, 0.0, 0.4, 0.0);

    // Make the VAO.
    GLuint vaoID;
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    // Make the VBO and add it to the VAO.
    GLuint vboID;
    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);

    static const GLfloat vertexBufferData[] = {
        -1.0, -1.0, 0.0,
         1.0, -1.0, 0.0,
         0.0,  1.0, 0.0,
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData),
                 vertexBufferData, GL_STATIC_DRAW);

    // vertex attribute array 0: vertices. No particular reason for 0, but must
    // match the layout in the shader.
    const GLuint vaaID = 0;
    glEnableVertexAttribArray(vaaID);
    glVertexAttribPointer(
        vaaID,
        3,         // number of numbers per vertex
        GL_FLOAT,  // type
        GL_FALSE,  // normalized?
        0,         // stride
        NULL       // array buffer offset
    );
    // The VAO is ready.

    // Create and compile our GLSL program from the shaders.
     GLuint programID = loadShaders(
        "simple-vertex.glsl", "simple-fragment.glsl");
    // Use our shader.
    glUseProgram(programID);

    puts("Initialized.");
}

int main() {
    GLFWwindow *window;
    init(&window);

    do {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the triangle! 3 indices starting at 0 -> 1 triangle.
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Check if the ESC key was pressed or the window was closed
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             !glfwWindowShouldClose(window));

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}
