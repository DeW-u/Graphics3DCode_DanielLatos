//
// Created by pbialas on 25.09.2020.
//

#include "app.h"

#include <iostream>
#include <vector>
#include <tuple>
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

#include "Application/utils.h"

void SimpleShapeApplication::init() {
    // A utility function that reads the shader sources, compiles them and creates the program object
    // As everything in OpenGL we reference program by an integer "handle".
    auto program = xe::utils::create_program(
            {{GL_VERTEX_SHADER,   std::string(PROJECT_DIR) + "/shaders/base_vs.glsl"},
             {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/base_fs.glsl"}});

    if (!program) {
        std::cerr << "Invalid program" << std::endl;
        exit(-1);
    }

    float vertices[] = {
        -0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, };

    unsigned int triangles[] = {
        0, 1, 2,
        3, 4, 5,
        3, 6, 5 
    };

    float strength=0.7;
    float color[3]={0.9,0.5,0.9};

    GLuint v_buffer_handle_uniforms;
    glGenBuffers(1, &v_buffer_handle_uniforms);
    
    glBindBuffer(GL_UNIFORM_BUFFER, v_buffer_handle_uniforms);

    glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);    

    glBufferSubData(GL_UNIFORM_BUFFER, 0,sizeof(float),&strength);
    glBufferSubData(GL_UNIFORM_BUFFER, 4*sizeof(float),3*sizeof(float),color);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    float theta = 1.0*glm::pi<float>()/6.0f;
    auto cs = std::cos(theta);
    auto ss = std::sin(theta);  
    glm::mat2 rot{cs,ss,-ss,cs};
    glm::vec2 trans{0.0,  -0.25};
    glm::vec2 scale{0.5, 0.5};
    GLuint v_buffer_handle_uniforms_move;
    glGenBuffers(1, &v_buffer_handle_uniforms_move);
    
    glBindBuffer(GL_UNIFORM_BUFFER, v_buffer_handle_uniforms_move);

    glBufferData(GL_UNIFORM_BUFFER, 12 * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);   

    glBufferSubData(GL_UNIFORM_BUFFER, 0, 2 * sizeof(float), &scale);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(float), 2 * sizeof(float), &trans);
    glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(float),4 * sizeof(float), &rot[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 8 * sizeof(float),4 * sizeof(float), &rot[1]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    GLuint v_buffer_handle, ebo;
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &v_buffer_handle);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangles), triangles, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindVertexArray(0);

    glBindBufferBase(GL_UNIFORM_BUFFER,0,v_buffer_handle_uniforms);
    glBindBufferBase(GL_UNIFORM_BUFFER,1,v_buffer_handle_uniforms_move);

    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);

    auto[w, h] = frame_buffer_size();
    glViewport(0, 0, w, h);

    glUseProgram(program);
}

void SimpleShapeApplication::frame() {
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
}
