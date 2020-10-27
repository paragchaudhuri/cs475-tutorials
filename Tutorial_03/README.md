# Tutorial 3 - Rotating a Colorcube

<br>
<br>

## About

This code is mostly same as the code of Tutorial 2. So you will see quite a lot similarities. In this tutorial we draw a cube with OpenGL and then add keyboard controls to allow the user to rotate the colorcube. 

When you untar the downloaded tgz file, you will find a cpp and a hpp file **03_colorcube.cpp** and **03_colorcube.hpp** and a file named **Makefile**. You will also find two shader files, **03_fshader.glsl** and **03_vshader.glsl**.

<br>
<br>

## Running the code

Similar to Tutorial 2, compilation of the code can be done using `make`. If you have a driver supporting OpenGL 3.3+ then running `03_colorcube` should work.

<br>
<br>

## Understanding the code

We assume here that you have already gone through the code for Tutorial 2. Therefore, we will not go through the code that has already been explained in the Tutorial 2.

<br>
<br>

## Rotation Matrix

```cpp
rotation_matrix = glm::rotate(glm::mat4(1.0f), xrot, glm::vec3(1.0f,0.0f,0.0f));
rotation_matrix = glm::rotate(rotation_matrix, yrot, glm::vec3(0.0f,1.0f,0.0f));
rotation_matrix = glm::rotate(rotation_matrix, zrot, glm::vec3(0.0f,0.0f,1.0f));

view_matrix = glm::lookAt(glm::vec3(0.0,0.0,-2.0),glm::vec3(0.0,0.0,0.0),glm::vec3(0.0,1.0,0.0));
  
ortho_matrix = glm::ortho(-2.0, 2.0, -2.0, 2.0, -20.0, 20.0);

modelviewproject_matrix = ortho_matrix * view_matrix * rotation_matrix;
```

In this piece of code we multiply the rotations about x,y and z axes to the rotation matrix. Initially we start with identity matrix `glm::mat4(1.0f)`, then multiply the *xrot*, *yrot* and *zrot* one by one, the third parameter in `glm::rotate()` is the axis, along which we have to rotate.

Then we setup the view_matrix using `glm::lookAt`. This creates the matrix that trasnforms the geometry from WCS to VCS. After this we create an orthographic frustum using `glm::ortho()`. This sets up our orthographic projection matrix to go from VCS to CCS/NDCS. The parameters given to it are our window co-ordinates in this order, left, right, bottom, top, z-near, z-far. Where, z-near and z-far are the location of near and far clipping plane. Finally, we multiply the ortho-matrix to the view matrix, to the rotation matrix, to get the composite modelviewproject_matrix.

```cpp
glUniformMatrix4fv(uModelViewProjectMatrix, 1, GL_FALSE, glm::value_ptr(modelviewproject_matrix));
```

The above statement modifies the value of *uModelViewMatrix*. The parameter ’1’ specifies that we are modifying one matrix and *GL_FALSE* specifies that there is no need to transpose the matrix.

<br>
<br>

## Keyboard Input

In the `key_callback()` function in **gl_framework.cpp** we deal with various keyboard inputs. You can see that here we increment/decrement our rotation parameters whenever their respective key is pressed.
