# Tutorial 2 - Drawing a Colorcube

<br>
<br>

## About

This code is mostly same as the code of 01_triangle.cpp (Tutorial_01). So
you might see quite a lot similarities.
In this tutorial we draw a cube with OpenGL. When you untar the downloaded tgz file, you will find a cpp and a hpp file **02_colorcube.cpp** and **02_colorcube.hpp** and a file named **Makefile**. You will also find two shader files, **02_fshader.glsl** and **02_vshader.glsl**.

<br>
<br>

## Running the code

Same as Tutorial_01, compilation of the code can be done using `make`.
If you have a driver supporting OpenGL 4.1 then running `02_colorcube` will do the trick for you. But if your system supports OpenGL 3.2+. Then in order to run the code the following changes need to be made in the **02_colorcube.cpp**. In the line number 135 and 136,

```cpp
glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4);
glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1);
```

you need to change to

```cpp
glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3);
```

Also in the **02_fshader.glsl** and **02_vshader.glsl** you need to modify the first line to
```cpp
# version 330
```

Once you make the above changes you can compile and run the 02_colorcube file.

<br>
<br>

## Understanding the code

Now, expecting that you have already went throught the Tutorial_01, We won’t go through the most of the basic skeleton code that has already been explained in the Tutorial_01. For example the ```main()``` function, it is exactly the same and doesn’t need any further explanation.

To start with, we have defined several variables, which we will come to later.

```cpp
GLuint shaderProgram;
GLuint vbo, vao;
```

Next up, we declare positions of 8 vertices and colors for each of the
vertices, this is also pretty similar to the previous tutorial.

```cpp
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
```

But here, we declare num vertices=36, now this has a reason. Since each object is expressed as triangles, we will construct every single face of a cube using two triangles.

The next piece of code does exactly this thing. For every given value of a,b,c,d, it creates a face of a cube, by making two adjoint triangles looking as a square. Since we have specified the positions in a specific way, the calls made during the colorcube() command, create different faces of the cube, you can play around with these values to get a better understanding of this piece of code.

```cpp
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
```

<br>
<br>

## Shaders

The next piece of code void **initBuffersGL(void)**, is essentially what were the **initShadersGL()** and **initVertexBufferGL()** in the Tutorial_01.

```cpp
void initBuffersGL(void)
{
  colorcube();

  //Ask GL for a Vertex Attribute Object (vao)
  glGenVertexArrays (1, &vao);
  //Set it as the current array to be used by binding it
  glBindVertexArray (vao);

  //Ask GL for a Vertex Buffer Object (vbo)
  glGenBuffers (1, &vbo);
  //Set it as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo);
  //Copy the points into the current buffer
  glBufferData (GL_ARRAY_BUFFER, sizeof (v_positions) + sizeof(v_colors), NULL, GL_STATIC_DRAW);
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(v_positions), v_positions );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(v_positions), sizeof(v_colors), v_colors );

  // Load shaders and use the resulting shader program
  std::string vertex_shader_file("02_vshader.glsl");
  std::string fragment_shader_file("02_fshader.glsl");

  std::vector<GLuint> shaderList;
  shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  shaderProgram = csX75::CreateProgramGL(shaderList);
  glUseProgram( shaderProgram );

  // set up vertex arrays
  GLuint vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  
  GLuint vColor = glGetAttribLocation( shaderProgram, "vColor" ); 
  glEnableVertexAttribArray( vColor );
  glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(v_positions)) );
}
```

We initially call the colorcube() method to populate the two vec4 arrays
**v_positions** and **v_colors**. After generating vao and vbo, we create a buffer of size **(v_positions + v_colors)**. (Since our buffer is going to hold the color data as well as the position data for each vertex). In the next two lines,

```cpp
glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(v_positions), v_positions );
glBufferSubData( GL_ARRAY_BUFFER, sizeof(v_positions), sizeof(v_colors), v_colors );
```

We fill up the buffer using the two types of sub data, position and color. The syntax and semantics of these functions are already explained in Tutorial_01.

After this, we load the shaders and push them back into shaderlist vector, for the reference to the shaders.

Then we specify how position and color data are stored in VBO using the vao, now as you remember, vertex attrib pointer, VAO, describes how vertex attributes are stored in the vertex buffer object, VBO. But, in this case we have two things that are to be stored in VBO, colors and positions. So, first we get the location of the parameter(vPosition and vColor), using `glGetAttribLocation()`. As, you can see that we have defined the same variables in the shader as well. If you remember, we mentioned in the tutorial 0 that the declaration of vPosition and vColor, in shader code may also be done using `layout(location = 0) in vec4 vp;`, the return value of `glGetAttribLocation()`, is the same location value from the shader program. Now, we can use glVertexAttribPointer, using the location for color and position pointers.

<br>
<br>

### Vertex Shader

```cpp
attribute vec4 vPosition;
attribute vec4 vColor;
varying vec4 color;
```

Unlike the first Tutorial, here instead of only specifying gl_Postion, we are
also providing the color using the attributes vPosition and vColor, these
attributes are the ones referred by glGetAttribLocation().

<br>
<br>

### Fragment Shader

The fragment shader is quite similar to the code previous tutorial and you can easily see that we are assigning the color of the fragment, from our input from vertex shader.
