#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "src/include/window.h"

#include <iostream>

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
const bool kRESIZABLE = false;

// The MAIN function, from here we start the application and run the game loop
int main()
{
  std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
  // Init GLFW
  // Set all the required options for GLFW

  bool ok = logl::window::inst()->init([]() -> GLFWwindow* {
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);    
    if (window == nullptr) {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return nullptr;
    }
    return window;
  });

  if (!ok) {
    return -1;
  }

  
  glfwMakeContextCurrent(logl::window::inst()->mutableWindow());

  // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
  glewExperimental = GL_TRUE;
  // Initialize GLEW to setup the OpenGL Function pointers
  if (glewInit() != GLEW_OK) {
    std::cout << "Failed to initialize GLEW" << std::endl;
    return -1;
  }    

  logl::window::inst()->addEventListener(
    "keydown",
    [&](logl::WindowEvent ev) {
      if (ev.key != GLFW_KEY_ESCAPE)
        return;
      std::cout << "Handling escape\n";
      GLFWwindow* w = logl::window::inst()->mutableWindow();
      glfwSetWindowShouldClose(w, GLFW_TRUE);
    }
  );

  logl::window::inst()->addEventListener(
    "keydown",
    [](logl::WindowEvent ev) {
      std::cout << "Key Press: " << ev.key_value << std::endl;
    }
  );

  logl::window::inst()->start();
  logl::window::inst()->clean();
  return 0;
}
