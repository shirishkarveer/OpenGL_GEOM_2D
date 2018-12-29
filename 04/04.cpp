#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


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

static void vertexAttribs() {
    // Our vertices. Three consecutive floats give a 3D vertex; Three
    // consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles,
    // and 12*3 vertices
    static const GLfloat vertexData[] = {
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
         1.0f, 1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
         1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
         1.0f,-1.0f,-1.0f,
         1.0f, 1.0f,-1.0f,
         1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
         1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
         1.0f,-1.0f, 1.0f,
         1.0f, 1.0f, 1.0f,
         1.0f,-1.0f,-1.0f,
         1.0f, 1.0f,-1.0f,
         1.0f,-1.0f,-1.0f,
         1.0f, 1.0f, 1.0f,
         1.0f,-1.0f, 1.0f,
         1.0f, 1.0f, 1.0f,
         1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
         1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
         1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
         1.0f,-1.0f, 1.0f
    };
    // Make the VBO and add it to the VAO.
    GLuint vertexVBOID;
    glGenBuffers(1, &vertexVBOID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBOID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData,
        GL_STATIC_DRAW);
    // 1st attribute buffer: vertices
    const GLuint vertexVAAID = 0;
    glEnableVertexAttribArray(vertexVAAID);
    glVertexAttribPointer(
        vertexVAAID,  // attribute. No particular reason for 0, but must match
                      // the layout in the shader.
        3,            // size
        GL_FLOAT,     // type
        GL_FALSE,     // normalized?
        0,            // stride
        NULL          // array buffer offset
    );
}

static void colourAttribs() {
    // One color for each vertex. They were generated randomly.
    static const GLfloat colourData[] = {
        0.583f,  0.771f,  0.014f,
        0.609f,  0.115f,  0.436f,
        0.327f,  0.483f,  0.844f,
        0.822f,  0.569f,  0.201f,
        0.435f,  0.602f,  0.223f,
        0.310f,  0.747f,  0.185f,
        0.597f,  0.770f,  0.761f,
        0.559f,  0.436f,  0.730f,
        0.359f,  0.583f,  0.152f,
        0.483f,  0.596f,  0.789f,
        0.559f,  0.861f,  0.639f,
        0.195f,  0.548f,  0.859f,
        0.014f,  0.184f,  0.576f,
        0.771f,  0.328f,  0.970f,
        0.406f,  0.615f,  0.116f,
        0.676f,  0.977f,  0.133f,
        0.971f,  0.572f,  0.833f,
        0.140f,  0.616f,  0.489f,
        0.997f,  0.513f,  0.064f,
        0.945f,  0.719f,  0.592f,
        0.543f,  0.021f,  0.978f,
        0.279f,  0.317f,  0.505f,
        0.167f,  0.620f,  0.077f,
        0.347f,  0.857f,  0.137f,
        0.055f,  0.953f,  0.042f,
        0.714f,  0.505f,  0.345f,
        0.783f,  0.290f,  0.734f,
        0.722f,  0.645f,  0.174f,
        0.302f,  0.455f,  0.848f,
        0.225f,  0.587f,  0.040f,
        0.517f,  0.713f,  0.338f,
        0.053f,  0.959f,  0.120f,
        0.393f,  0.621f,  0.362f,
        0.673f,  0.211f,  0.457f,
        0.820f,  0.883f,  0.371f,
        0.982f,  0.099f,  0.879f
    };
    GLuint colourVBOID;
    glGenBuffers(1, &colourVBOID);
    glBindBuffer(GL_ARRAY_BUFFER, colourVBOID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colourData), colourData,
        GL_STATIC_DRAW);
    // 2nd attribute buffer: colors
    const GLuint colourVAAID = 1;
    glEnableVertexAttribArray(colourVAAID);
    glVertexAttribPointer(
        colourVAAID,  // attribute. No particular reason for 1, but must match
                      // the layout in the shader.
        3,            // size
        GL_FLOAT,     // type
        GL_FALSE,     // normalized?
        0,            // stride
        NULL          // array buffer offset
    );
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
        "Tutorial 04 - Colored Cube", NULL, NULL);
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

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it's closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Make the VAO.
    GLuint vaoID;
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);
    vertexAttribs();
    colourAttribs();
    // The VAO is ready.

    // Create and compile our GLSL program from the shaders
    GLuint programID = loadShaders(
        "transform-vertex.glsl", "color-fragment.glsl");
    // Use our shader.
    glUseProgram(programID);

    // Projection matrix: 45 Field of View, 4:3 ratio, display range: 0.1 unit <-> 100 units
    glm::mat4 projection = glm::perspective(
        glm::radians(45.f), 4.f/3, 0.1f, 100.f);

    // Camera matrix
    glm::mat4 view = glm::lookAt(
        glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
        glm::vec3(0, 0, 0), // and looks at the origin
        glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    // Model matrix: an identity matrix (model will be at the origin)
    glm::mat4 model = glm::mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices
    // Remember, matrix multiplication is the other way around
    glm::mat4 mvp = projection * view * model;

    // Get a handle for our "MVP" uniform
    GLuint matrixID = glGetUniformLocation(programID, "MVP");
    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);

    puts("Initialized.");
}

int main() {
    GLFWwindow *window;
    init(&window);

    do {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw the triangle! 12*3 indices starting at 0 -> 12 triangles
        glDrawArrays(GL_TRIANGLES, 0, 12*3);

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
