//
// Created by Piotr Białas on 02/11/2021.
//

#include "PhongMaterial.h"
#include "Engine/utils.h"

namespace xe {

    GLuint PhongMaterial::color_uniform_buffer_ = 0u;
    GLuint PhongMaterial::shader_ = 0u;
    GLint  PhongMaterial::uniform_map_Kd_location_ = 0;

    void PhongMaterial::bind() {
        check_and_use_program(program());
        GLuint use_map_Kd = 0;
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, color_uniform_buffer_);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &color_);
        if(texture_ > 0) {
            glUniform1i(uniform_map_Kd_location_, texture_unit_);
            glActiveTexture(GL_TEXTURE0 + texture_unit_);
            glBindTexture(GL_TEXTURE_2D, texture_);
            use_map_Kd = 1;
        }

        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(GLuint), &use_map_Kd);
        glBindBuffer(GL_UNIFORM_BUFFER, 0u);
    }

    void PhongMaterial::unbind() {

    }

    void PhongMaterial::init() {


        auto program = xe::utils::create_program(
                {{GL_VERTEX_SHADER,   std::string(PROJECT_DIR) + "/shaders/phong_vs.glsl"},
                 {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/phong_fs.glsl"}});
        if (!program) {
            std::cerr << "Invalid program" << std::endl;
            exit(-1);
        }

        shader_ = program;

        glGenBuffers(1, &color_uniform_buffer_);

        glBindBuffer(GL_UNIFORM_BUFFER, color_uniform_buffer_);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) + sizeof(GLuint), nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0u);

        uniform_map_Kd_location_ = glGetUniformLocation(shader_, "map_Kd");
        if (uniform_map_Kd_location_ == -1) {
          std::cerr<<"Cannot get uniform map_kD location\n";
        }
#if __APPLE__
        auto u_modifiers_index = glGetUniformBlockIndex(program, "Color");
        if (u_modifiers_index == -1) {
            std::cerr << "Cannot find Color uniform block in program" << std::endl;
        } else {
            glUniformBlockBinding(program, u_modifiers_index, 0);
        }
#endif

#if __APPLE__
        auto u_transformations_index = glGetUniformBlockIndex(program, "Transformations");
        if (u_transformations_index == -1) {
            std::cerr << "Cannot find Transformations uniform block in program" << std::endl;
        } else {
            glUniformBlockBinding(program, u_transformations_index, 1);
        }
#endif


    }
}