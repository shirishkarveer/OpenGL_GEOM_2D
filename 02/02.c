#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


static void glfwCB(int error, const char *desc)
{
    fprintf(stderr, "GLFW error 0x%08X: %s\n", error, desc);
}

static GLuint loadShader(const char *fn, GLenum shaderType)
{
    printf("Compiling shader '%s'...\n", fn);

    GLuint shader = glCreateShader(shaderType);
    if (!shader) {
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
    GLint size[1] = {ftell(f)};
    if (size[0] == -1) {
        perror("Failed to get file size");
        exit(1);
    }
    rewind(f);
    char *source = malloc(size[0]);
    if (!source) {
        perror("Failed to allocate source memory");
        exit(1);
    }
    if (fread(source, 1, size[0], f) != size[0]) {
        perror("Failed to read file");
        exit(1);
    }
    if (fclose(f))
        perror("Warning: failed to close source file");

    const GLchar *rosource = source;
    glShaderSource(shader, 1, &rosource, size);
    free(source);

    glCompileShader(shader);

    GLint logLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength) {
        GLchar *log = malloc(logLength);
        if (!log) {
            perror("Couldn't allocate shader compile log");
            exit(1);
        }
        glGetShaderInfoLog(shader, logLength, NULL, log);
        printf("Shader compile message: %s\n", log);
        free(log);
    }

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status)
        exit(1);

    return shader;
}

static GLuint loadShaders(const char *vertex_fn, const char *fragment_fn)
{
    // Compile the shaders
    GLuint vertexShader = loadShader(vertex_fn, GL_VERTEX_SHADER),
           fragmentShader = loadShader(fragment_fn, GL_FRAGMENT_SHADER);

    puts("Linking shader program...");

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Check the program
    GLint logLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        char *log = malloc(logLength);
        if (!log) {
            perror("Couldn't allocate shader compile log");
            exit(1);
        }
        glGetProgramInfoLog(program, logLength, NULL, log);
        printf("Shader link message: %s\n", log);
        free(log);
    }

    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status)
        exit(1);

    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

int main()
{
    // Set error callback to see more detailed failure info
    glfwSetErrorCallback(glfwCB);

    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing

    // To ensure compatiblity, check the output of this command:
    // $ glxinfo | grep 'Max core profile version'
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // We don't want the old OpenGL
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    GLFWwindow *window = glfwCreateWindow(800, 600, "Tutorial 02 - Red triangle", NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, "
                        "they are not 3.3 compatible. Try the 2.1 version of "
                        "the tutorials.\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = true; // Needed in core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Make the VAO.
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Make the VBO and add it to the VBO.
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    const GLfloat vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         0.0f,  1.0f, 0.0f,
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data),
                 vertex_buffer_data, GL_STATIC_DRAW);

    // vertex attribute array 0: vertices. No particular reason for 0, but must
    // match the layout in the shader.
    GLuint vaa = 0;
    glEnableVertexAttribArray(vaa);
    glVertexAttribPointer(
        vaa,
        3,         // number of numbers per vertex
        GL_FLOAT,  // type
        GL_FALSE,  // normalized?
        0,         // stride
        0          // array buffer offset
    );
    // The VAO is ready.


    // Create and compile our GLSL program from the shaders.
    GLuint program = loadShaders("simple-vertex.glsl", "simple-fragment.glsl");
    // Use our shader.
    glUseProgram(program);

    puts("Initialized.");

    do
    {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the triangle!
        glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Check if the ESC key was pressed or the window was closed
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             !glfwWindowShouldClose(window));

    // Cleanup VBO
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(program);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}
