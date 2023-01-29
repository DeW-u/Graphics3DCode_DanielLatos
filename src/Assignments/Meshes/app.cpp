//
// Created by pbialas on 25.09.2020.
//

#include "app.h"

#include <iostream>
#include <vector>
#include <tuple>
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Engine/camera.h"
#include "Engine/camera_controler.h"

#include "Application/utils.h"

float vertices[] = {
        -0.5f,  0.0f, -0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 0.0f,
        0.5f,  0.0f, -0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.0f, -0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 1.0f,
        0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f,  0.0f, -0.5f, 1.0f, 1.0f, 0.0f,
        -0.5f,  0.0f, -0.5f, 1.0f, 1.0f, 0.0f,
};

unsigned int triangles[] = {
    15, 2, 1,
    15, 14, 2,
    4, 5, 6,
    7, 8, 9,
    3, 0, 10,
    11, 12, 13
};

void SimpleShapeApplication::init()
{
    // A utility function that reads the shader sources, compiles them and creates the program object
    // As everything in OpenGL we reference program by an integer "handle".
    auto program = xe::utils::create_program(
        {{GL_VERTEX_SHADER, std::string(PROJECT_DIR) + "/shaders/base_vs.glsl"},
         {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/base_fs.glsl"}});

    if (!program)
    {
        std::cerr << "Invalid program" << std::endl;
        exit(-1);
    }

    set_camera(new xe::Camera);
    set_controler(new xe::CameraControler(camera()));

    auto [w, h] = frame_buffer_size();
    camera_->perspective(glm::pi<float>() / 4.0, (float)w / h, 0.1, 100.0f);
    camera_->look_at(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    M_ = glm::mat4(1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    float strength = 0.7;
    float color[3] = {0.9, 0.5, 0.9};

    GLuint v_buffer_handle_uniforms;
    glGenBuffers(1, &v_buffer_handle_uniforms);

    glBindBuffer(GL_UNIFORM_BUFFER, v_buffer_handle_uniforms);

    glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float), &strength);
    glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(float), 3 * sizeof(float), color);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glGenBuffers(1, &v_buffer_handle_uniforms_move);
    glBindBuffer(GL_UNIFORM_BUFFER, v_buffer_handle_uniforms_move);
    glBufferData(GL_UNIFORM_BUFFER, 16 * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    pyramid = new xe::Mesh;
    pyramid->allocate_vertex_buffer(sizeof(vertices) * sizeof(float), GL_STATIC_DRAW);
    pyramid->load_vertices(0, sizeof(vertices), vertices);
    pyramid->vertex_attrib_pointer(0, 3, GL_FLOAT, 6 * sizeof(float), (0));
    pyramid->vertex_attrib_pointer(1, 3, GL_FLOAT, 6 * sizeof(float), (3 * sizeof(float)));

    pyramid->allocate_index_buffer(sizeof(triangles) * sizeof(float), GL_STATIC_DRAW);
    pyramid->load_indices(0, sizeof(triangles), triangles);

    pyramid->add_submesh(0, 18);
    add_submesh(pyramid);

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, v_buffer_handle_uniforms);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, v_buffer_handle_uniforms_move);

    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);

    glViewport(0, 0, w, h);

    glUseProgram(program);
}

void SimpleShapeApplication::frame()
{
    auto PVM = camera_->projection() * camera_->view() * M_;
    glBindBuffer(GL_UNIFORM_BUFFER, v_buffer_handle_uniforms_move);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, 16 * sizeof(GLfloat), &PVM[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    for (auto m : meshes_)
    {
        m->draw();
    }
}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h)
{
    Application::framebuffer_resize_callback(w, h);
    glViewport(0, 0, w, h);
    camera_->set_aspect((float)w / h);
}
