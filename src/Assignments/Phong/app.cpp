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
#include "Engine/Material.h"
#include "Engine/ColorMaterial.h"
#include "Engine/PhongMaterial.h"
#include "Application/utils.h"
#include "Engine/mesh_loader.h"
#include "Engine/Light.h"

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

    // xe::ColorMaterial::init();
    xe::PhongMaterial::init();
    set_camera(new xe::Camera);
    set_controler(new xe::CameraControler(camera()));

    auto [w, h] = frame_buffer_size();
    camera_->perspective(glm::pi<float>() / 4.0, (float)w / h, 0.1, 100.0f);
    camera_->look_at(glm::vec3(0.0f, 1.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    M_ = glm::mat4(1.0f);

    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);

    float strengthModificator = 0.7;
    glm::vec3 colorModificator = glm::vec3(0.9, 0.5, 0.9);

    glGenBuffers(1, &v_buffer_handle_uniforms_move);
    glBindBuffer(GL_UNIFORM_BUFFER, v_buffer_handle_uniforms_move);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) + sizeof(glm::mat4) + sizeof(glm::mat3), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glGenBuffers(1, &v_buffer_handle_unifroms_light);
    glBindBuffer(GL_UNIFORM_BUFFER, v_buffer_handle_unifroms_light);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec3) + sizeof(GLuint) + 1*(sizeof(glm::vec3) + sizeof(float) + sizeof(glm::vec3) + sizeof(float)), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    add_light(xe::PointLight{{0.0, 0.0, 1.0}, {1.0, 1.0, 1.0}, 2.0, 0.1});

    add_ambient(glm::vec3(1.0f, 0.0f, 0.0f));

    pyramid = xe::load_mesh_from_obj(std::string(ROOT_DIR) + "/Models/square.obj",
                                          std::string(ROOT_DIR) + "/Models");

    add_submesh(pyramid);

    glBindBufferBase(GL_UNIFORM_BUFFER, 1, v_buffer_handle_uniforms_move);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, v_buffer_handle_unifroms_light);
    
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glViewport(0, 0, w, h);

    glUseProgram(program);
}

void SimpleShapeApplication::frame()
{
    auto VM = camera_->view() * M_;
    auto R = glm::mat3(VM);
    auto N = glm::mat3(glm::cross(R[1], R[2]), glm::cross(R[2], R[0]), glm::cross(R[0], R[1]));
    auto PVM = camera_->projection() * camera_->view() * M_;
    glBindBuffer(GL_UNIFORM_BUFFER, v_buffer_handle_uniforms_move);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &VM);
    glBufferSubData(GL_UNIFORM_BUFFER, 2*sizeof(glm::mat4), sizeof(N[0]), &N[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 2*sizeof(glm::mat4) + sizeof(N[0]), sizeof(N[1]), &N[1]);
    glBufferSubData(GL_UNIFORM_BUFFER, 2*sizeof(glm::mat4) + sizeof(N[0]) + sizeof(N[1]), sizeof(N[2]), &N[2]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    auto no_lights = 1;
    glBindBuffer(GL_UNIFORM_BUFFER, v_buffer_handle_unifroms_light);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), &ambient_);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec3), sizeof(GLuint), &no_lights);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec3) + sizeof(GLuint), sizeof(glm::vec3), &p_lights_[0].position_in_vs);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec3) + sizeof(GLuint) + sizeof(glm::vec3), sizeof(glm::vec3), &p_lights_[0].color);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec3) + sizeof(GLuint) + sizeof(glm::vec3), sizeof(float), &p_lights_[0].radius);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec3) + sizeof(GLuint) + sizeof(glm::vec3) + sizeof(float) + sizeof(glm::vec3), sizeof(float) ,&p_lights_[0].intensity);
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
