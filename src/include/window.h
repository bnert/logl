#pragma once

#include "glew-2.1.0/include/GL/glew.h"
#include "glfw/include/GLFW/glfw3.h"

#include <atomic>
#include <functional>
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

namespace logl {

struct WindowEvent {
  char key_value;
  int key;
  int scancode;
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
  ~window() {
    glfwTerminate();
  }

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

    GLfloat verticies[] = {
      -0.5f, -0.5f, 0.0f,
      0.0f, 0.5f, 0.0f,
      0.5f, -0.5f, 0.0f,
    };

    GLuint virtual_buffer_obj_id;
    glGenBuffers(1, &virtual_buffer_obj_id);
    glBindBuffer(GL_ARRAY_BUFFER, virtual_buffer_obj_id);
    glBufferData(
      GL_ARRAY_BUFFER,
      sizeof(verticies),
      verticies,
      GL_STATIC_DRAW
    );

    GLuint shader_id = glCreateShader(GL_VERTEX_SHADER);
    std::string sh = "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "void main() {\n"
    "  gl_Position = vec4(position.x, position.y, position.z, 1.0f);\n"
    "}\n";
    const GLchar* shader = sh.data();
    glShaderSource(shader_id, 1, &shader, nullptr);
    glCompileShader(shader_id);
    GLint success;
    GLchar log[512];
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader_id, 512, NULL, log);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
      log << std::endl;
    }

    GLuint fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fg = "#version 330 core\n"
    "out vec4 color;\n"
    "void main() {\n"
    "  color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n";
    const GLchar* fg_shader = fg.data();
    glShaderSource(fragment_id, 1, &fg_shader, nullptr);
    glCompileShader(fragment_id);
    glGetShaderiv(fragment_id, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(fragment_id, 512, NULL, log);
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
      log << std::endl;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, shader_id);
    glAttachShader(program, fragment_id);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(program, 512, nullptr, log);
      std::cout << "PROGRAM LINK ERR: " << log << std::endl;
    } else {
      glUseProgram(program);
    }

    // Game loop
    while (!glfwWindowShouldClose(logl::window::inst()->mutableWindow())) {
      glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);
      // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
      glfwPollEvents();
      // Swap the screen buffers
      glfwSwapBuffers(logl::window::inst()->mutableWindow());
    }
  }
};

} // end namespace logl
