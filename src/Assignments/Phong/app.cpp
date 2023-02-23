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

float vertices[] = {
        -0.5f,  0.0f, -0.5f, 0.5000f, 0.8090f, //0
        -0.5f, 0.0f, 0.5f, 0.1910f, 0.5000f, //1
        0.5f, 0.0f, 0.5f, 0.5000f, 0.1910f, //2
        0.5f, 0.0f, -0.5f, 0.8090f, 0.5000f, //3
        0.0f, 1.0f, 0.0f, 0.0000f, 1.0000f, //4
        0.0f, 1.0f, 0.0f, 0.0000f, 0.0000f, //5
        0.0f, 1.0f, 0.0f, 1.0000f, 0.0000f, //6
        0.0f, 1.0f, 0.0f, 1.0000f, 1.0000f, //7
};

unsigned int indices[] = {
    3, 1, 0,
    3, 2, 1,
    0, 1, 4,
    3, 0, 7,
    1, 2, 5,
    2, 3, 6
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

    // GLuint texture;

    // stbi_set_flip_vertically_on_load(true);
    // GLint width, height, channels;
    // auto texture_file = std::string(PROJECT_DIR) + "/Models/multicolor.png";
    // auto img = stbi_load(texture_file.c_str(), &width, &height, &channels, 0);
    // if (!img) {
    //     std::cerr<<"Could not read image from file `"<<texture_file<<"'\n";
    // } else {
    //     std::cout<<"Loaded a "<<width<<"x"<<height<<" texture with "<<channels<<" channels\n";
        
    //     glGenTextures(1, &texture);
    //     glBindTexture(GL_TEXTURE_2D, texture);

    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);

    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // }
    // stbi_image_free(img);

    xe::ColorMaterial::init();
    // xe::PhongMaterial::init();
    set_camera(new xe::Camera);
    set_controler(new xe::CameraControler(camera()));

    auto [w, h] = frame_buffer_size();
    camera_->perspective(glm::pi<float>() / 4.0, (float)w / h, 0.1, 100.0f);
    camera_->look_at(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    M_ = glm::mat4(1.0f);

    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);

    float strengthModificator = 0.7;
    glm::vec3 colorModificator = glm::vec3(0.9, 0.5, 0.9);

    glGenBuffers(1, &v_buffer_handle_uniforms_move);
    glBindBuffer(GL_UNIFORM_BUFFER, v_buffer_handle_uniforms_move);
    glBufferData(GL_UNIFORM_BUFFER, 16 * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // xe::ColorMaterial *pyramidTexture = new xe::ColorMaterial(
    //     getModifiedColorVec(
    //         glm::vec4(1.0, 0.0, 0.5, 0.0), 
    //         colorModificator, 
    //         strengthModificator
    //     )
    // );

    // pyramid = new xe::Mesh;
    // pyramid->allocate_vertex_buffer(sizeof(vertices)*sizeof(float), GL_STATIC_DRAW);
    // pyramid->load_vertices(0, sizeof(vertices), vertices);
    // pyramid->vertex_attrib_pointer(0, 3, GL_FLOAT, 5 * sizeof(float), (0));
    // pyramid->vertex_attrib_pointer(1, 2, GL_FLOAT, 5 * sizeof(float), 3*sizeof(float));
    // pyramid->allocate_index_buffer(sizeof(indices) * sizeof(float), GL_STATIC_DRAW);
    // pyramid->load_indices(0, sizeof(indices), indices);
    // pyramid->add_submesh(0, 18, pyramidTexture);

    pyramid = xe::load_mesh_from_obj(std::string(ROOT_DIR) + "/Models/square.obj",
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
