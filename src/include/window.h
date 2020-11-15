#pragma once

#include "pkg/shade/include/shade.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <atomic>
#include <functional>
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

namespace logl {

struct WindowEvent {
  char key_value; int key; int scancode;
  int action;
  int mods;
};

class window {
  GLFWwindow* window_;
  std::atomic<bool> resizeable_;
  std::vector<std::function<void(WindowEvent)>> keydown_events_;
  const std::unordered_map<
    int,
    std::string
  >  glfw_events_map_{
    {GLFW_PRESS, "keydown"},
  };

public:
  window() : window_(nullptr), resizeable_(false) {}
  window(GLFWwindow* w) : window_(w) {}
  ~window() {}

  static window* inst() {
    static window* w = new window();
    return w;
  }

  bool init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, resizeable_);
    return true;
  }

  void clean() {
    glfwTerminate();
  }

  bool init(std::function<GLFWwindow*()> cb) {
    init();
    setWindow(cb());
    return (window_ != nullptr);
  }

  void setWindow(GLFWwindow* w) {
    window_ = w;
  }

  const GLFWwindow* getWindow() {
    return window_;
  }

  const GLFWwindow* getWindow() const {
    return getWindow();
  }

  GLFWwindow* mutableWindow() {
    return window_;
  }

  void addEventListener(
    const std::string& ev, 
    std::function<void(WindowEvent)>&& cb
  ) {
    if (ev == "keydown")
      keydown_events_.push_back(std::move(cb));
  }

  void removeEventListener(
    const std::string& ev,
    std::function<void(WindowEvent)> cb
  ) {
    if (ev == "keydown") {
    }
  }

  void handleKeyPress(
    GLFWwindow* w,
    int key, int scancode, int action, int mods
  ) {
    try {
      // Ignore for handling key presses
      // of only modifiers
      if (
        key == GLFW_KEY_LEFT_SHIFT
        || key == GLFW_KEY_RIGHT_SHIFT
        || key == GLFW_KEY_LEFT_ALT
        || key == GLFW_KEY_RIGHT_ALT
        || key == GLFW_KEY_LEFT_CONTROL
        || key == GLFW_KEY_RIGHT_CONTROL
        || action == GLFW_RELEASE
      )
        return;

      char kv = static_cast<char>(key);
      if (kv >= 'A' && kv <= 'Z' && !(mods & GLFW_MOD_SHIFT)) {
        const static unsigned int diff = 'a' - 'A';
        kv = kv + diff;
      }

      WindowEvent e{kv, key, scancode, action, mods};
      for (auto& c : keydown_events_) { 
        c(e);
      }
    } catch (std::exception /* e */) {
      // No callbacks exist
    }
  }

  void start() {
    int width, height;
    // Define the viewport dimensions
    glfwGetFramebufferSize(logl::window::inst()->mutableWindow(), &width, &height);  
    glViewport(0, 0, width, height);

    // Create top level handler for events
    glfwSetKeyCallback(window_, [](GLFWwindow* w, int k, int s, int a, int m) {
      logl::window::inst()->handleKeyPress(w, k, s, a, m);
    });

    shade::Shader vrtx(shade::Type::VRTX, "shaders/basic_vertex.glsl");
    shade::Shader fgmt(shade::Type::FRAG, "shaders/simple_color.glsl");
    shade::Program program;
    program.attach(vrtx);
    program.attach(fgmt);
    program.link();

    GLfloat vertices[] = {
      0.5f, 0.5f, 0.0f, // Top Right
      0.5f, -0.5f, 0.0f, // Bottom Right
      -0.5f, -0.5f, 0.0f, // Bottom Left
      -0.5f, 0.5f, 0.0f // Top Left
    };

    GLuint indices[] = { // Note that we start from 0!
      0, 1, 3, // First Triangle
      1, 2, 3 // Second Triangle
    };

    // For a triangle
    // GLfloat verticies[] = {
    //   -0.5f, -0.5f, 0.0f,
    //   0.0f, 0.5f, 0.0f,
    //   0.5f, -0.5f, 0.0f,
    // };

    GLuint el_buf_obj_id;
    GLuint vertex_buffer_obj_id, vertex_array_obj_id;

    glGenVertexArrays(1, &vertex_array_obj_id);
    glGenBuffers(1, &vertex_buffer_obj_id);
    glGenBuffers(1, &el_buf_obj_id);

    // Bind vertex array object first
    glBindVertexArray(vertex_array_obj_id);

    // Bind the verticies buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Bind the indicies buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, el_buf_obj_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // For triangles
    // glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj_id);
    // glBufferData(
    //   GL_ARRAY_BUFFER,
    //   sizeof(verticies),
    //   verticies,
    //   GL_STATIC_DRAW
    // );
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Unbinds the GL_ARRAY_BUFFER
    // and unbinds the vertex array
    // from the current gl context
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
 
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Game loop
    while (!glfwWindowShouldClose(logl::window::inst()->mutableWindow())) {
      // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
      glfwPollEvents();

      glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      // sets shader to use
      glUseProgram(program.id());
      // binds the vao, which is bound to the vbo for creating a triangle
      glBindVertexArray(vertex_array_obj_id);
      // draws currently bound vbo via the vao
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      // glDrawArrays(GL_TRIANGLES, 0, 3);
      // unbinds the vao
      glBindVertexArray(0);

      // Swap the screen buffers
      glfwSwapBuffers(logl::window::inst()->mutableWindow());
    }
  }
};

} // end namespace logl
