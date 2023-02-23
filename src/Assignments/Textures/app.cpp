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
#include "Application/utils.h"
#include "Engine/mesh_loader.h"

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

    xe::ColorMaterial::init();
    set_camera(new xe::Camera);
    set_controler(new xe::CameraControler(camera()));

    auto [w, h] = frame_buffer_size();
    camera_->perspective(glm::pi<float>() / 4.0, (float)w / h, 0.1, 100.0f);
    camera_->look_at(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    M_ = glm::mat4(1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    float strengthModificator = 0.7;
    glm::vec3 colorModificator = glm::vec3(0.9, 0.5, 0.9);

    glGenBuffers(1, &v_buffer_handle_uniforms_move);
    glBindBuffer(GL_UNIFORM_BUFFER, v_buffer_handle_uniforms_move);
    glBufferData(GL_UNIFORM_BUFFER, 16 * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    pyramid = xe::load_mesh_from_obj(std::string(ROOT_DIR) + "/Models/pyramid.obj",
                                          std::string(ROOT_DIR) + "/Models");

    add_submesh(pyramid);

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
