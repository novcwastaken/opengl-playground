#ifndef SHADER_HH
#define SHADER_HH

#include <fstream>
#include <glad/gl.h>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
    public:
        unsigned int program_id;

        Shader(const char* vertex_path, const char* fragment_path) {
            // Read vertex & fragment sources
            std::string vertex_source;
            std::string fragment_source;
            std::ifstream vertex_file;
            std::ifstream fragment_file;

            vertex_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            fragment_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

            try {
                vertex_file.open(vertex_path);
                fragment_file.open(fragment_path);
                std::stringstream vertex_stream, fragment_stream;

                vertex_stream << vertex_file.rdbuf();
                fragment_stream << fragment_file.rdbuf();

                vertex_file.close();
                fragment_file.close();

                vertex_source = vertex_stream.str();
                fragment_source = fragment_stream.str();
            } catch (std::ifstream::failure e) {
                std::cout << "Failure reading shader file!\n";
            }

            const char* vertex_source_c = vertex_source.c_str();
            const char* fragment_source_c = fragment_source.c_str();

            // COMPILE SHADERS
            unsigned int vertex, fragment;
            int success;
            char info_log[512];

            // Vertex
            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vertex_source_c, NULL);
            glCompileShader(vertex);

            glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(vertex, 512, NULL, info_log);
                std::cout << "Failed to compile vertex shader!\n" << info_log << "\n";
            }

            // Fragment
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fragment_source_c, NULL);
            glCompileShader(fragment);

            glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(fragment, 512, NULL, info_log);
                std::cout << "Failed to compile fragment shader!\n" << info_log << "\n";
            }

            // Shader program
            program_id = glCreateProgram();
            glAttachShader(program_id, vertex);
            glAttachShader(program_id, fragment);
            glLinkProgram(program_id);

            glGetProgramiv(program_id, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(program_id, 512, NULL, info_log);
                std::cout << "Failed to link shader program!\n" << info_log << "\n";
            }

            // Cleanup (already compiled)
            glDeleteShader(vertex);
            glDeleteShader(fragment);
        }

        void use() {
            glUseProgram(program_id);
        }

        void set_bool(const std::string &name, bool value) const {
            glUniform1i(glGetUniformLocation(program_id, name.c_str()), (int)value);
        }
        void set_int(const std::string &name, int value) const {
            glUniform1i(glGetUniformLocation(program_id, name.c_str()), value);
        }
        void set_float(const std::string &name, float value) const {
            glUniform1f(glGetUniformLocation(program_id, name.c_str()), value);
        }

};

#endif