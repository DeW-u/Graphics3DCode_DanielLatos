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


private:
    GLuint vao_;
    GLuint v_buffer_handle_uniforms_move;
    std::vector<xe::Mesh*> meshes_; 

    glm::mat4 M_;
};