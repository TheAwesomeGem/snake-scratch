// Copyright (c) Topaz Centuallas 2022.

#include <cstdio>
#include "renderer.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"


namespace Game {
    void Renderer::init() {
        glGenVertexArrays(1, &drawable);
        glBindVertexArray(drawable);

        GLuint buffer_id;
        glGenBuffers(1, &buffer_id);

        float vertex_data[]{
                -1.0F, 1.0F, //       BOTTOM LEFT
                1.0F, 1.0F, //    BOTTOM RIGHT
                -1.0F, -1.0F,  //  TOP LEFT
                1.0, -1.0F    //  TOP RIGHT
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
                                            uniform mat4 transform;
                                            uniform vec4 color;
                                            out vec4 vertexColor;
                                            void main()
                                            {
                                                gl_Position = transform * vec4(aPos, 1.0);
                                                vertexColor = color;
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
                                            in vec4 vertexColor;

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

    void Renderer::render_rectangle(float x, float y, float width, float height, Color color) const {
        glBindVertexArray(drawable);
        glUseProgram(program);

        float canonical_x = ((x / this->viewport_width) * 2.0F) - 1.0F;
        float canonical_y = ((y / this->viewport_height) * 2.0F) - 1.0F;
        float canonical_width = width / this->viewport_width;
        float canonical_height = height / this->viewport_height;

        glm::mat4 transform = glm::mat4{1.0F};
        transform = glm::translate(transform, glm::vec3(canonical_x + canonical_width, canonical_y + canonical_height, 0.0F));
        // TODO: Rotate here.
        transform = glm::scale(transform, glm::vec3(canonical_width, canonical_height, 1.0F));

        glUniformMatrix4fv(glGetUniformLocation(program, "transform"), 1, GL_FALSE, glm::value_ptr(transform));
        glUniform4f(glGetUniformLocation(program, "color"), color.r, color.g, color.b, color.a);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}
