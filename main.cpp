#include "glew-2.1.0/include/GL/glew.h"
#include "glfw/include/GLFW/glfw3.h"

#include <iostream>

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
const bool kRESIZABLE = false;

// The MAIN function, from here we start the application and run the game loop
int main()
{
  std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
  // Init GLFW
  glfwInit();
  // Set all the required options for GLFW
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, kRESIZABLE);

  // Create a GLFWwindow object that we can use for GLFW's functions
  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);    
  if (window == nullptr) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
  glewExperimental = GL_TRUE;
  // Initialize GLEW to setup the OpenGL Function pointers
  if (glewInit() != GLEW_OK) {
    std::cout << "Failed to initialize GLEW" << std::endl;
    return -1;
  }    

  // Define the viewport dimensions
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);  
  glViewport(0, 0, width, height);

  // Game loop
  while (!glfwWindowShouldClose(window)) {
    // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
    glfwPollEvents();
    // Swap the screen buffers
    glfwSwapBuffers(window);
  }

  // Terminate GLFW, clearing any resources allocated by GLFW.
  glfwTerminate();
  return 0;
}