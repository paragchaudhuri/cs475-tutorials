#include "gl_framework.hpp"

#include <vector>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

extern GLfloat xrot,yrot,zrot,c_xrot,c_yrot,c_zrot;
extern bool enable_perspective;
extern bool save_frame;

namespace csX75
{
  //! Initialize GL State
  void initGL(void)
  {
    //Set framebuffer clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //Set depth buffer furthest depth
    glClearDepth(1.0);
    //Set depth test to less-than
    glDepthFunc(GL_LESS);
    //Enable depth testing
    glEnable(GL_DEPTH_TEST);
  }
  
  //!GLFW Error Callback
  void error_callback(int error, const char* description)
  {
    std::cerr<<description<<std::endl;
  }
  
  //!GLFW framebuffer resize callback
  void framebuffer_size_callback(GLFWwindow* window, int width, int height)
  {
    //!Resize the viewport to fit the window size - draw to entire window
    glViewport(0, 0, width, height);
  }
  
  //!GLFW keyboard callback
  void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
  {
    //!Close the window if the ESC key was pressed
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
    else if (key == GLFW_KEY_LEFT )
      yrot -= 1.0;
    else if (key == GLFW_KEY_RIGHT )
      yrot += 1.0;
    else if (key == GLFW_KEY_UP )
      xrot -= 1.0;
    else if (key == GLFW_KEY_DOWN )
      xrot += 1.0;
    else if (key == GLFW_KEY_PAGE_UP )
      zrot -= 1.0;
    else if (key == GLFW_KEY_PAGE_DOWN)
      zrot += 1.0;
    else if (key == GLFW_KEY_A  )
      c_yrot -= 1.0;
    else if (key == GLFW_KEY_D  )
      c_yrot += 1.0;
    else if (key == GLFW_KEY_W  )
      c_xrot -= 1.0;
    else if (key == GLFW_KEY_S  )
      c_xrot += 1.0;        
    else if (key == GLFW_KEY_Q  )
      c_zrot -= 1.0;
    else if (key == GLFW_KEY_E  )
      c_zrot += 1.0;  
    else if (key == GLFW_KEY_Z)
      save_frame = true;
  }

  int save_fb_toimage(GLFWwindow* window)
  {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    GLsizei num_channels = 4;
    GLsizei ch_width = num_channels * width;
    GLsizei bufferSize = ch_width * height;

    std::vector<char> buffer(bufferSize);
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glReadBuffer(GL_BACK);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer.data());

    stbi_flip_vertically_on_write(true);
    int num_bytes_written = stbi_write_jpg("saved_frame.jpg", width, height, num_channels, buffer.data(), 100);

    return num_bytes_written;

  }
};  
  


