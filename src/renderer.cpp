// Copyright (c) Topaz Centuallas 2022.

#include <cstdio>
#include "renderer.h"
#include "glm/glm.hpp"


namespace Game {
    void Renderer::init() {
        glGenVertexArrays(1, &drawable);
        glBindVertexArray(drawable);

        GLuint buffer_id;
        glGenBuffers(1, &buffer_id);

        float vertex_data[]{
                -0.25F, 0.25F, //       BOTTOM LEFT
                0.25F, 0.25F, //    BOTTOM RIGHT
                -0.25, -0.25F,  //  TOP LEFT
                0.25, -0.25F    //  TOP RIGHT
        };
        glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, &vertex_data, GL_STATIC_DRAW);

        GLuint index_buffer_id;
        glGenBuffers(1, &index_buffer_id);

        int indices_data[]{
                0, 1, 3,
                0, 3, 2
        };
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 6, &indices_data, GL_STATIC_DRAW);

        GLuint position_attrib_index = 0;
        glEnableVertexAttribArray(position_attrib_index);
        glVertexAttribPointer(position_attrib_index, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindVertexArray(0);

        glDisableVertexAttribArray(position_attrib_index);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        GLuint vertex_shader;
        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        const char* vertex_shader_src = R"(#version 460 core
                                            layout (location = 0) in vec3 aPos;
                                            out vec4 vertexColor;
                                            void main()
                                            {
                                                gl_Position = vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor
                                                vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // set the output variable to a dark-red color
                                            })";
        glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
        glCompileShader(vertex_shader);

        GLint vertex_compiled;
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &vertex_compiled);
        if (vertex_compiled != GL_TRUE) {
            GLsizei log_length = 0;
            GLchar message[1024];
            glGetShaderInfoLog(vertex_shader, 1024, &log_length, message);
            printf("Error: Cannot compile vertex shader: %s", message);
        }

        GLuint fragment_shader;
        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        const char* fragment_shader_src = R"(#version 460 core
                                            out vec4 FragColor;
                                            in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)

                                            void main()
                                            {
                                                FragColor = vertexColor;
                                            })";
        glShaderSource(fragment_shader, 1, &fragment_shader_src, NULL);
        glCompileShader(fragment_shader);

        GLint fragment_compiled;
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &fragment_compiled);
        if (fragment_compiled != GL_TRUE) {
            GLsizei log_length = 0;
            GLchar message[1024];
            glGetShaderInfoLog(fragment_shader, 1024, &log_length, message);
            printf("Error: Cannot compile fragment shader: %s", message);
        }

        program = glCreateProgram();
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
        glLinkProgram(program);

        GLint program_linked;
        glGetProgramiv(program, GL_LINK_STATUS, &program_linked);
        if (program_linked != GL_TRUE) {
            GLsizei log_length = 0;
            GLchar message[1024];
            glGetProgramInfoLog(program, 1024, &log_length, message);
            printf("Error: Cannot link shaders to program: %s", message);
        }

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
    }

    void Renderer::deinit() {
        glBindVertexArray(0);
        glDeleteProgram(program);
        program = 0;
    }

    void Renderer::render_rectangle(float x, float y, float width, float height) {
        glBindVertexArray(drawable);
        glUseProgram(program);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}
