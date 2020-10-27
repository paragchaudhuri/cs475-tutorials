/*
  CSX75 Tutorial 3

  A program which opens a window and draws the "color cube."

  Use the arrow keys and PgUp,PgDn,
  keys to make the cube move.

  Modified from An Introduction to OpenGL Programming, 
  Ed Angel and Dave Shreiner, SIGGRAPH 2013

  Written by Parag Chaudhuri, 2015
*/


#include "04_camera_viewing.hpp"

GLuint shaderProgram;
GLuint vbo[2], vao[2];

glm::mat4 rotation_matrix;
glm::mat4 c_rotation_matrix;

glm::mat4 model_matrix;
glm::mat4 view_matrix;
glm::mat4 projection_matrix;

glm::mat4 modelviewproject_matrix;
GLuint uModelViewProjectMatrix;

//-----------------------------------------------------------------

//6 faces, 2 triangles/face, 3 vertices/triangle
const int num_vertices = 36;

//Eight vertices in homogenous coordinates
glm::vec4 positions[8] = {
  glm::vec4(-0.5, -0.5, 0.5, 1.0),
  glm::vec4(-0.5, 0.5, 0.5, 1.0),
  glm::vec4(0.5, 0.5, 0.5, 1.0),
  glm::vec4(0.5, -0.5, 0.5, 1.0),
  glm::vec4(-0.5, -0.5, -0.5, 1.0),
  glm::vec4(-0.5, 0.5, -0.5, 1.0),
  glm::vec4(0.5, 0.5, -0.5, 1.0),
  glm::vec4(0.5, -0.5, -0.5, 1.0)
};

//RGBA colors
glm::vec4 colors[8] = {
  glm::vec4(0.0, 0.0, 0.0, 1.0),
  glm::vec4(1.0, 0.0, 0.0, 1.0),
  glm::vec4(1.0, 1.0, 0.0, 1.0),
  glm::vec4(0.0, 1.0, 0.0, 1.0),
  glm::vec4(0.0, 0.0, 1.0, 1.0),
  glm::vec4(1.0, 0.0, 1.0, 1.0),
  glm::vec4(1.0, 1.0, 1.0, 1.0),
  glm::vec4(0.0, 1.0, 1.0, 1.0)
};

int tri_idx=0;
glm::vec4 v_positions[num_vertices];
glm::vec4 v_colors[num_vertices];

// quad generates two triangles for each face and assigns colors to the vertices
void quad(int a, int b, int c, int d)
{
  v_colors[tri_idx] = colors[a]; v_positions[tri_idx] = positions[a]; tri_idx++;
  v_colors[tri_idx] = colors[b]; v_positions[tri_idx] = positions[b]; tri_idx++;
  v_colors[tri_idx] = colors[c]; v_positions[tri_idx] = positions[c]; tri_idx++;
  v_colors[tri_idx] = colors[a]; v_positions[tri_idx] = positions[a]; tri_idx++;
  v_colors[tri_idx] = colors[c]; v_positions[tri_idx] = positions[c]; tri_idx++;
  v_colors[tri_idx] = colors[d]; v_positions[tri_idx] = positions[d]; tri_idx++;
 }

// generate 12 triangles: 36 vertices and 36 colors
void colorcube(void)
{
    quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 );
}



glm::vec4 v_positions2[6] = {
  glm::vec4(1.0, -1.0, 1.0, 1.0),
  glm::vec4(1.0, -1.0, -1.0, 1.0),
  glm::vec4(-1.0, -1.0, -1.0, 1.0),
  glm::vec4(1.0, -1.0, 1.0, 1.0),
  glm::vec4(-1.0, -1.0, -1.0, 1.0),
  glm::vec4(-1.0, -1.0, 1.0, 1.0)
};

glm::vec4 v_colors2[6] = {
  glm::vec4(0.5, 0.5, 0.5, 1.0),
  glm::vec4(0.5, 0.5, 0.5, 1.0),
  glm::vec4(0.5, 0.5, 0.5, 1.0),
  glm::vec4(0.5, 0.5, 0.5, 1.0),
  glm::vec4(0.5, 0.5, 0.5, 1.0),
  glm::vec4(0.5, 0.5, 0.5, 1.0)   
};


//-----------------------------------------------------------------

void initBuffersGL(void)
{

  // Load shaders and use the resulting shader program
  std::string vertex_shader_file("04_vshader.glsl");
  std::string fragment_shader_file("04_fshader.glsl");

  std::vector<GLuint> shaderList;
  shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  shaderProgram = csX75::CreateProgramGL(shaderList);
  glUseProgram( shaderProgram );

  // getting the attributes from the shader program
  GLuint vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
  GLuint vColor = glGetAttribLocation( shaderProgram, "vColor" ); 
  uModelViewMatrix = glGetUniformLocation( shaderProgram, "uModelViewProjectMatrix");

  //Ask GL for two Vertex Attribute Objects (vao) , one for the colorcube and one for the plane.
  glGenVertexArrays (2, vao);
  //Ask GL for two Vertex Buffer Object (vbo)
  glGenBuffers (2, vbo);

  // Color cube ------------------

  //Set 0 as the current array to be used by binding it
  glBindVertexArray (vao[0]);
  //Set 0 as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo[0]);

  colorcube();
  //Copy the points into the current buffer
  glBufferData (GL_ARRAY_BUFFER, sizeof (v_positions) + sizeof(v_colors), NULL, GL_STATIC_DRAW);
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(v_positions), v_positions );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(v_positions), sizeof(v_colors), v_colors );

  // set up vertex array

  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  
  glEnableVertexAttribArray( vColor );
  glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(v_positions)) );


  // Plane -----------------------

  //Set 1 as the current array to be used by binding it
  glBindVertexArray (vao[1]);
  //Set 1 as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo[1]);

  //Copy the points into the current buffer
  glBufferData (GL_ARRAY_BUFFER, sizeof (v_positions2) + sizeof(v_colors2), NULL, GL_STATIC_DRAW);
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(v_positions2), v_positions2 );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(v_positions2), sizeof(v_colors2), v_colors2 );

  // set up vertex array

  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

  glEnableVertexAttribArray( vColor );
  glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(v_positions2)) );

}

void renderGL(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(xrot), glm::vec3(1.0f,0.0f,0.0f));
  rotation_matrix = glm::rotate(rotation_matrix, glm::radians(yrot), glm::vec3(0.0f,1.0f,0.0f));
  rotation_matrix = glm::rotate(rotation_matrix, glm::radians(zrot), glm::vec3(0.0f,0.0f,1.0f));
  model_matrix = rotation_matrix;

  //Creating the lookat and the up vectors for the camera
  c_rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(c_xrot), glm::vec3(1.0f,0.0f,0.0f));
  c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_yrot), glm::vec3(0.0f,1.0f,0.0f));
  c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_zrot), glm::vec3(0.0f,0.0f,1.0f));

  glm::vec4 c_pos = c_rotation_matrix*glm::vec4(c_xpos,c_ypos,c_zpos, 1.0);
  glm::vec4 c_up = c_rotation_matrix*glm::vec4(c_up_x,c_up_y,c_up_z, 1.0);
  
  //Creating the lookat matrix
  view_matrix = glm::lookAt(glm::vec3(c_pos),glm::vec3(0.0),glm::vec3(c_up));

  //creating the projection matrix
  if(enable_perspective)
    projection_matrix = glm::frustum(-1.0, 1.0, -1.0, 1.0, 1.0, 5.0);
    //projection_matrix = glm::perspective(glm::radians(90.0),1.0,0.1,5.0);
  else
    projection_matrix = glm::ortho(-2.0, 2.0, -2.0, 2.0, -5.0, 5.0);

    // Draw cube
  modelviewproject_matrix = projection_matrix*view_matrix*model_matrix;
  glUniformMatrix4fv(uModelViewProjectMatrix, 1, GL_FALSE, glm::value_ptr(modelviewproject_matrix));
  glBindVertexArray (vao[0]);
  glDrawArrays(GL_TRIANGLES, 0, num_vertices);
  
  // Draw plane
  modelviewproject_matrix = view_matrix;
  glUniformMatrix4fv(uModelViewProjectMatrix, 1, GL_FALSE, glm::value_ptr(modelviewproject_matrix));
  glBindVertexArray (vao[1]);
  glDrawArrays(GL_TRIANGLES, 0, 6);

}

int main(int argc, char** argv)
{
  //! The pointer to the GLFW window
  GLFWwindow* window;

  //! Setting up the GLFW Error callback
  glfwSetErrorCallback(csX75::error_callback);

  //! Initialize GLFW
  if (!glfwInit())
    return -1;

  //We want OpenGL 4.0
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //This is for MacOSX - can be omitted otherwise
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
  //We don't want the old OpenGL 
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

  //! Create a windowed mode window and its OpenGL context
  window = glfwCreateWindow(512, 512, "CS475/CS675 Tutorial 3: Rotating  Colorcube", NULL, NULL);
  if (!window)
    {
      glfwTerminate();
      return -1;
    }
  
  //! Make the window's context current 
  glfwMakeContextCurrent(window);

  //Initialize GLEW
  //Turn this on to get Shader based OpenGL
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err)
    {
      //Problem: glewInit failed, something is seriously wrong.
      std::cerr<<"GLEW Init Failed : %s"<<std::endl;
    }

  //Print and see what context got enabled
  std::cout<<"Vendor: "<<glGetString (GL_VENDOR)<<std::endl;
  std::cout<<"Renderer: "<<glGetString (GL_RENDERER)<<std::endl;
  std::cout<<"Version: "<<glGetString (GL_VERSION)<<std::endl;
  std::cout<<"GLSL Version: "<<glGetString (GL_SHADING_LANGUAGE_VERSION)<<std::endl;

  //Keyboard Callback
  glfwSetKeyCallback(window, csX75::key_callback);
  //Framebuffer resize callback
  glfwSetFramebufferSizeCallback(window, csX75::framebuffer_size_callback);

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  //Initialize GL state
  csX75::initGL();
  initBuffersGL();

  // Loop until the user closes the window
  while (glfwWindowShouldClose(window) == 0)
    {
       
      // Render here
      renderGL();

      // Swap front and back buffers
      glfwSwapBuffers(window);
      
      // Poll for and process events
      glfwPollEvents();
    }
  
  glfwTerminate();
  return 0;
}

//-------------------------------------------------------------------------

