#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <iostream>
#include "shader.hh"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void process_input(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Initialize the GLFW window
    GLFWwindow* window = glfwCreateWindow(720, 720, "OpenGL Playground", NULL, NULL);
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
    // float rectangle_vertices[] = { // Vertex XYZ -- vertex RGB -- tex ST
    //     -0.5, -0.5, 0.0,    1.0, 0.0, 1.0,    0.0, 0.0, // Bottom left
    //      0.5, -0.5, 0.0,    0.0, 0.0, 1.0,    1.0, 0.0, // Bottom right
    //      0.5,  0.5, 0.0,    0.0, 1.0, 0.0,    1.0, 1.0, // Top right
    //     -0.5,  0.5, 0.0,    1.0, 0.0, 0.0,    0.0, 1.0, // Top left
    // };

    // Aligned at top left
    float rectangle_vertices[] = { // Vertex XYZ -- vertex RGB -- tex ST
        -1.0,  0.0, 0.0,    1.0, 0.0, 1.0,    0.0, 0.0, // Bottom left
         0.0,  0.0, 0.0,    0.0, 0.0, 1.0,    1.0, 0.0, // Bottom right
         0.0,  1.0, 0.0,    0.0, 1.0, 0.0,    1.0, 1.0, // Top right
        -1.0,  1.0, 0.0,    1.0, 0.0, 0.0,    0.0, 1.0, // Top left
    };

    unsigned int rectangle_indexes[] = {
        0, 1, 2,
        0, 2, 3
    };

    // Buffers
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_vertices), rectangle_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangle_indexes), rectangle_indexes, GL_STATIC_DRAW);

    // Link vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // Position

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1); // Color

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2); // Texture coord

    // Texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int texture_w, texture_h, texture_channel_count;
    stbi_set_flip_vertically_on_load(true);

    unsigned char* texture_data = stbi_load("assets/textures/musicgato.png", &texture_w, &texture_h, &texture_channel_count, 0);

    if (texture_data) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // ily Claude

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_w, texture_h, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture!\n";
    }

    stbi_image_free(texture_data);

    // Use the shader
    shader.use();

    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Polygon mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        double cursor_x, cursor_y;
        int window_w, window_h;

        // Input
        process_input(window);
        glfwGetCursorPos(window, &cursor_x, &cursor_y);
        glfwGetWindowSize(window, &window_w, &window_h);

        std::cout << "@@@ x = " << cursor_x << ", y = " << cursor_y << "\n";

        // RENDERING COMMANDS
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 transformation(1.0f);

        // Apply the rotation transformation to the `transformation` matrix (identity matrix by default)
        // The rotation is 45 degrees (glm takes in radians), around the Z plane (unit vector)
        // transformation = glm::rotate(transformation, (float)glfwGetTime()*2, glm::vec3(1.0, 1.0, 0.0));
        // transformation = glm::rotate(transformation, (float)glfwGetTime()*4, glm::vec3(0.0, 1.0, 0.0));
        // transformation = glm::rotate(transformation, (float)glfwGetTime()*8, glm::vec3(0.0, 0.0, 1.0));

        //transformation = glm::translate(transformation, glm::vec3(1, -1, 0));

        transformation = glm::translate(transformation, glm::vec3(cursor_x/window_w*2, -(cursor_y/window_h)*2, 0));


        // Rectangle
        glBindTexture(GL_TEXTURE_2D, texture);
        shader.use();

        shader.set_mat4("transformation", transformation);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 3);

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