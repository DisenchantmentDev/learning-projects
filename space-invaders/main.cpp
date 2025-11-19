#include <cstdio>
#include <GLFW/glfw3.h>

void error_callback(int error, const char* description) {
  fprintf(stderr, "Error: %s\n", description);
}

int main (int argc, char *argv[]) {
  glfwSetErrorCallback(error_callback);

  if(!glfwInit()) {
    return -1;
  }

  window = glfwCreateWindow(640, 480, "Space Invaders", NULL, NULL);
  if(!window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  return 0;
}
