#include <cmath>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

void process_input(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

const char* vertex_shader_source = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;

    out vec4 vertex_color;

    void main() {
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
        vertex_color = vec4(0.0, 0.32, 0.32, 1.0);
    }
)";

const char* fragment_shader_source = R"(
    #version 330 core
    in vec4 vertex_color;
    out vec4 frag_color;

    uniform vec4 meow_color;

    void main() {
       //frag_color = vec4(72.0f/255, 167.0f/255, 240.0f/255, 1.0f);
       //frag_color = vertex_color;
       frag_color = meow_color;
    }
)";

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

    // Compile vertex shader
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    int success;
    char info_log[512];

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (success) {
        std::cout << "Successfully compiled vertex shader!\n";
    } else {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << "\n";
    }

    // Compile fragment shader
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (success) {
        std::cout << "Successfully compiled fragment shader!\n";
    } else {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << "\n";
    }

    // Link shaders
    unsigned int shader_program =  glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (success) {
        std::cout << "Successfully linked shaders!\n";
    } else {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << "\n";
    }

    // Cleanup
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // Vertex data
    float rectangle_vertices[] = {
        -0.5, -0.5, 0.0,
        0.5, -0.5, 0.0,
        0.0, 0.5, 0.0,
    };

    unsigned int rectangle_indexes[] = {
        0, 1, 2,
    0, 3, 2,
    };

     unsigned int VBO, VAO;
     glGenVertexArrays(1, &VAO);
     glGenBuffers(1, &VBO);
     //glGenBuffers(1, &EBO);

    // Binding magic
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_vertices), rectangle_vertices, GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangle_indexes), rectangle_indexes, GL_STATIC_DRAW);

    // Link vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

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
        float time = glfwGetTime();
        float red_value = (std::sin(time) / 2.0) + 0.5;
        int vertex_color_location = glGetUniformLocation(shader_program, "meow_color");

        glUseProgram(shader_program);
        glUniform4f(vertex_color_location, red_value, 0.0, 0.0, 1.0);

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