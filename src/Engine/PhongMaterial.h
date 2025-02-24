//
// Created by pbialas on 30.11.22.
//

#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>

#include "Application/utils.h"

#include "Material.h"

namespace xe {
    class PhongMaterial : public Material {
    public:
        PhongMaterial(const glm::vec4 color) : color_(color), texture_(0), texture_unit_(0) {}
        PhongMaterial(const glm::vec4 color, const GLuint texture, const GLuint texture_unit) : color_(color), texture_(texture), texture_unit_(texture_unit) {}

        void bind() override;
        void unbind() override;

        static void init();

        static GLuint program() { return shader_; }

        static GLint uniform_map_Kd_location_;

        GLuint texture() const { return texture_; }
        GLuint texture_unit() const { return texture_unit_; }
        void set_texture(GLuint texture) { texture_ = texture; }
        void set_texture_unit(GLuint texture_unit) { texture_unit_ = texture_unit; }

    private:
        static GLuint shader_;
        static GLuint color_uniform_buffer_;
        GLuint use_map_Kd;
        GLuint texture_;
        GLuint texture_unit_;

        glm::vec4 color_;

    };
}

