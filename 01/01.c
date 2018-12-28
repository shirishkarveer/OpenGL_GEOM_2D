#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>

static void glfwErrorCallback(int error, const char *desc) {
    fprintf(stderr, "GLFW error 0x%08X: %s\n", error, desc);
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
        "Tutorial 01", NULL, NULL);
    if (!*window) {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, "
                        "they are not 3.3 compatible. Try the 2.1 version of "
                        "the tutorials.\n");
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(*window);

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(*window, GLFW_STICKY_KEYS, GL_TRUE);
}

int main() {
    GLFWwindow *window;
    init(&window);

    do {
        // Clear the screen. It's not mentioned before Tutorial 02, but it can
        // cause flickering, so it's there nonetheless.
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw nothing, see you in tutorial 2!

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Check if the ESC key was pressed or the window was closed
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             !glfwWindowShouldClose(window));
    
    return 0;
}
