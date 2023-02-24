//
// Created by pbialas on 05.08.2020.
//

#pragma once

#include <vector>
#include "Application/application.h"
#include "Application/utils.h"
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Engine/camera.h"
#include "Engine/camera_controler.h"
#include "Engine/Mesh.h"
#include "Engine/mesh_loader.h"
#include "Engine/Light.h"

#include "glad/gl.h"

class SimpleShapeApplication : public xe::Application
{
public:
    SimpleShapeApplication(int width, int height, std::string title, bool debug) : Application(width, height, title, debug) {}

    void init() override;

    void frame() override;

    void framebuffer_resize_callback(int w, int h) override;

    xe::Camera *camera_;

    xe::CameraControler *controler_;

    xe::Mesh *pyramid;

    void set_controler(xe::CameraControler *controler) { controler_ = controler; };

    void set_camera(xe::Camera *camera) { camera_ = camera; };

    xe::Camera *camera() const { return camera_; };

    void scroll_callback(double xoffset, double yoffset) override {
         xe::Application::scroll_callback(xoffset, yoffset);   
         camera()->zoom(yoffset / 30.0f);
    }

    void mouse_button_callback(int button, int action, int mods) {
        xe::Application::mouse_button_callback(button, action, mods);
        if (controler_) {
            double x, y;
            glfwGetCursorPos(window_, &x, &y);

            if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
                controler_->LMB_pressed(x, y);

            if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
                controler_->LMB_released(x, y);
        }
    }   

    void cursor_position_callback(double x, double y) {
        xe::Application::cursor_position_callback(x, y);
        if (controler_) {
            controler_->mouse_moved(x, y);
        }
    }

    void add_submesh(xe::Mesh *mesh) {
        meshes_.push_back(mesh);
    }

    glm::vec4 getModifiedColorVec(glm::vec4 colorVec, glm::vec3 colorModificator, float strengthModificator) {
        glm::vec4 colorResult = glm::vec4(
            colorVec[0]*colorModificator[0]*strengthModificator,
            colorVec[1]*colorModificator[1]*strengthModificator,
            colorVec[2]*colorModificator[2]*strengthModificator,
            colorVec[3]
        );

        return colorResult;
    }

    void add_light(const xe::PointLight &p_light) {
        p_lights_.push_back(p_light); 
    }

    void add_ambient(glm::vec3 ambient) {
        ambient_ = ambient;
    }

private:
    GLuint vao_;
    GLuint v_buffer_handle_uniforms_move;
    std::vector<xe::Mesh*> meshes_; 

    glm::mat4 M_;

    GLuint v_buffer_handle_unifroms_light;
    glm::vec3 ambient_;
    std::vector<xe::PointLight> p_lights_;
};