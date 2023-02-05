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

float vertices[] = {
        -0.5f,  0.0f, -0.5f,
        -0.5f, 0.0f, 0.5f,
        0.5f, 0.0f, 0.5f,
        0.5f, 0.0f, -0.5f,
        0.0f, 1.0f, 0.0f,
};

unsigned int _pyramidBase[] = {
    3, 1, 0,
    3, 2, 1
};

unsigned int _pyramidRedWall[] = {
    0, 1, 4
};

unsigned int _pyramidCyanWall[] = {
    3, 0, 4
};

unsigned int _pyramidGreenWall[] = {
    1, 2, 4
};

unsigned int _pyramidBlueWall[] = {
    2, 3, 4
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

    xe::ColorMaterial *pyramidBaseColor = new xe::ColorMaterial(
        getModifiedColorVec(
            glm::vec4(1.0, 1.0, 0.0, 0.0), 
            colorModificator, 
            strengthModificator
        )
    );

    xe::Mesh *pyramidBase = new xe::Mesh;
    pyramidBase->allocate_vertex_buffer(sizeof(vertices)*sizeof(float), GL_STATIC_DRAW);
    pyramidBase->load_vertices(0, sizeof(vertices), vertices);
    pyramidBase->vertex_attrib_pointer(0, 3, GL_FLOAT, 3 * sizeof(float), (0));
    pyramidBase->allocate_index_buffer(sizeof(_pyramidBase) * sizeof(float), GL_STATIC_DRAW);
    pyramidBase->load_indices(0, sizeof(_pyramidBase), _pyramidBase);
    pyramidBase->add_submesh(0, 6, pyramidBaseColor);
    add_submesh(pyramidBase);

    xe::ColorMaterial *colorRed = new xe::ColorMaterial(
        getModifiedColorVec(
            glm::vec4(1.0, 0.0, 0.0, 0.0),
            colorModificator, 
            strengthModificator
        )
    );

    xe::Mesh *pyramidRedWall = new xe::Mesh;
    pyramidRedWall->allocate_vertex_buffer(sizeof(vertices)*sizeof(float), GL_STATIC_DRAW);
    pyramidRedWall->load_vertices(0, sizeof(vertices), vertices);
    pyramidRedWall->vertex_attrib_pointer(0, 3, GL_FLOAT, 3 * sizeof(float), (0));
    pyramidRedWall->allocate_index_buffer(sizeof(_pyramidRedWall) * sizeof(float), GL_STATIC_DRAW);
    pyramidRedWall->load_indices(0, sizeof(_pyramidRedWall), _pyramidRedWall);
    pyramidRedWall->add_submesh(0, 3, colorRed);
    add_submesh(pyramidRedWall);

    xe::ColorMaterial *colorCyan = new xe::ColorMaterial(
        getModifiedColorVec(
            glm::vec4(0.0, 1.0, 1.0, 0.0),
            colorModificator, 
            strengthModificator
        )
    );

    xe::Mesh *pyramidCyanWall = new xe::Mesh;
    pyramidCyanWall->allocate_vertex_buffer(sizeof(vertices)*sizeof(float), GL_STATIC_DRAW);
    pyramidCyanWall->load_vertices(0, sizeof(vertices), vertices);
    pyramidCyanWall->vertex_attrib_pointer(0, 3, GL_FLOAT, 3 * sizeof(float), (0));
    pyramidCyanWall->allocate_index_buffer(sizeof(_pyramidCyanWall) * sizeof(float), GL_STATIC_DRAW);
    pyramidCyanWall->load_indices(0, sizeof(_pyramidCyanWall), _pyramidCyanWall);
    pyramidCyanWall->add_submesh(0, 3, colorCyan);
    add_submesh(pyramidCyanWall);

    xe::ColorMaterial *colorGreen = new xe::ColorMaterial(
        getModifiedColorVec(
            glm::vec4(0.0, 1.0, 0.0, 0.0),
            colorModificator,
            strengthModificator
        )
    );

    xe::Mesh *pyramidGreenWall = new xe::Mesh;
    pyramidGreenWall->allocate_vertex_buffer(sizeof(vertices)*sizeof(float), GL_STATIC_DRAW);
    pyramidGreenWall->load_vertices(0, sizeof(vertices), vertices);
    pyramidGreenWall->vertex_attrib_pointer(0, 3, GL_FLOAT, 3 * sizeof(float), (0));
    pyramidGreenWall->allocate_index_buffer(sizeof(_pyramidGreenWall) * sizeof(float), GL_STATIC_DRAW);
    pyramidGreenWall->load_indices(0, sizeof(_pyramidGreenWall), _pyramidGreenWall);
    pyramidGreenWall->add_submesh(0, 3, colorGreen);
    add_submesh(pyramidGreenWall);

    xe::ColorMaterial *colorBlue = new xe::ColorMaterial(
        getModifiedColorVec(
            glm::vec4(0.0, 0.0, 1.0, 0.0),
            colorModificator,
            strengthModificator
        )
    );

    xe::Mesh *pyramidBlueWall = new xe::Mesh;
    pyramidBlueWall->allocate_vertex_buffer(sizeof(vertices)*sizeof(float), GL_STATIC_DRAW);
    pyramidBlueWall->load_vertices(0, sizeof(vertices), vertices);
    pyramidBlueWall->vertex_attrib_pointer(0, 3, GL_FLOAT, 3 * sizeof(float), (0));
    pyramidBlueWall->allocate_index_buffer(sizeof(_pyramidBlueWall) * sizeof(float), GL_STATIC_DRAW);
    pyramidBlueWall->load_indices(0, sizeof(_pyramidBlueWall), _pyramidBlueWall);
    pyramidBlueWall->add_submesh(0, 3, colorBlue);
    add_submesh(pyramidBlueWall);

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
