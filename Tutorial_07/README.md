# Tutorial 7 - Hierarchical Modelling

<br>
<br>

## About

In this tutorial, hierarchical modelling is demonstrated. We have used the
elongated version of the colorcube to represent an arm in the hierarchy. There are three arms in total, connected to one another. Use the keys 1,2 and 3 to switch between arms. Use Arrow keys, Pgup and Pgdn to rotate the arms.

<br>
<br>

##Running the Code
If you have a driver supporting OpenGL 4.1 then running executable will do
the trick for you. But if your system supports OpenGL 3.2+. Then in order
to run the code the following changes need to be made in the 07 xyz.cpp. In
the line number 136 and 137,

```cpp

```

you need to change to

```cpp

```
Also in the 02 fshader.glsl and 02 vshader.glsl you need to modify the first
line to
```cpp

```
Once you make the above changes you can compile and run the executable.

<br>
<br>

## Understanding the code
The understanding for the code of basics is documented in the previous tutorials. This tutorial would not go into much detail about those again, but
explain all the new stuff.

### Hierarchy node
First, we’ll look at the new class in the file hierarchy node.cpp.

```cpp

```
This class is pretty simple, it stores the handles to the VAO and VBO of
the object and also its translation and rotation parameters. There are two
main functions : render and render tree. render function renders just the
object with the current matrix stack (we will discuss how the matrix stack
is updated later). render tree function renders the object along with its all
children recursivey. Apart from that there are functions to increment and
decrement the rotation parameters. Next we look at its constructor:

```cpp

```
It takes as arguments: its parent, the vertex arrays, and their respective
sizes. The allocation and initialization of the vao and vbo corresponding
to the object happens in the constructor itself. The initial translation and
rotation parameters are set to 0. The parent node’s child list is automatically
updated here. Now we look at the main functions of the class

```cpp

```
You’ll see that here, all the matrices in the matrix stack, which is a global
variable, are multiplied to one another and then passed to the vertex shader.
The basic code for rendering is the same as in all the previous tutorials.

```cpp

```
You will see that in the render tree function the translation and the rotation
matrices are pushed into the matrix stack. The recursion happens in a
depth-first fashion. Once the current node and all its children are rendered,
the matrices are popped of the stack.

### Initialisation
Now, we come to the changes in the main file.
```cpp

```
Here, the shaders are initialised, and three nodes are created, and are
linked according to their hierarchy. As mentioned before, the colorcube with
modified vertices is used for rendering.
```cpp

```
In the rendering function, The matrix stack is cleared, and the viewing
matrix is pushed in first. the creation of the viewing matrix is explained in
previous tutorials. After this, the render_tree function of the root node in
the hierarchy is called to render the full hierarchy.
<br>
<br>

## Shaders

#### Vertex Shaders
```cpp

```
The vertex shader is similar to the shader in the tutorial 4. It just multiplies
the modelview matrix to the vertex position.

#### Fragment Shaders

The fragment shader is quite similar to the code previous tutorial and you
can easily see that we are assigning the color of the fragment, from our input
from vertex shader.
<br>
<br>

##Keyboard Input
In the key_callback() function in gl framework.cpp we deal with various keyboard inputs. Various additional inputs have been added to handle the movement of the arms and also to switch between arms.
```cpp

```
