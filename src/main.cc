#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.hh"

void process_input(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Initialize the GLFW window
    GLFWwindow* window = glfwCreateWindow(1280, 720, "OpenGL Playground", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create the GLFW window!\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Tell GLFW to call this function on every window resize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialize GLAD
    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD!\n";
        return -1;
    }

    // Shaders
    Shader shader("assets/shaders/vertex.vert", "assets/shaders/fragment.frag");

    // Vertex data
    float rectangle_vertices[] = {
        -0.5, -0.5, 0.0,   0.0, 170.0/255, 1.0,
         0.5, -0.5, 0.0,   11.0/255, 214.0/255, 11.0/255,
         0.0,  0.5, 0.0,   230.0/255, 28.0/255, 28.0/255
    };

    // unsigned int rectangle_indexes[] = {
    //     0, 1, 2,
    // 0, 3, 2,
    // };

     unsigned int VBO, VAO;
     glGenVertexArrays(1, &VAO);
     glGenBuffers(1, &VBO);
     //glGenBuffers(1, &EBO);

    // Binding magic
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_vertices), rectangle_vertices, GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangle_indexes), rectangle_indexes, GL_STATIC_DRAW);

    // Link vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Polygon mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBindVertexArray(VAO);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Input
        process_input(window);

        // RENDERING COMMANDS
        glClearColor(18.0/255, 19.0/255, 20.0/255, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        // Triangle
        // float time = glfwGetTime();
        // float red_value = (std::sin(time) / 2.0) + 0.5;
        // int vertex_color_location = glGetUniformLocation(shader_program, "meow_color");

        shader.use();
        //glUniform4f(vertex_color_location, red_value, 0.0, 0.0, 1.0);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap frame buffers, check & call events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}