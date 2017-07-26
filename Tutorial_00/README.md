# Tutorial 0 - Getting Started with OpenGL 

<br>
<br>
## Before you begin

### For Ubuntu:

* To install OpenGL libraries, install drivers for your graphics card from the `NVIDIA` or `AMD` websites or the Ubuntu repository.
* If you do not have a graphics card or these do not work, install `libgl?-mesa-(dri, glx, dev)`. Ubuntu will automatically install the `Nouveau/Gallium` drivers.
* To install GLEW, install `libglew?.XX` and `libglew-dev` packages.
* To install GLFW, install `libglfw3` and `libglfw3-dev` packages. 

### For OSX:

* OpenGL is preinstalled on OSX.
* To install GLEW and GLFW, use [Homebrew] (https://brew.sh/). All relevant instructions can be found on the Homebrew site. The packages are called glew and glfw3 
    
<br>
<br>
## About the tutorial

This tutorial sets up a OpenGL context and queries for the available GL version. When you untar the downloaded tgz file, you will find a cpp file **00_glInit.cpp** and a file named **Makefile**.

<br>
<br>

## Running the code
Assuming you have everything set up (all relevant libraries etc.) you can compile the code by running

	make

This will produce a single executable file **00_glInit**. To run it, do 

    ./00_glInit

When the code is run, it will print the name of the renderer on your laptop and the highest version of OpenGL supported by your laptop. Here is how the output should look like:

	> naman@rastogi:~/Downloads/00_glInit$ make
	g++ -I/usr/local/include 00_glInit.cpp -o 00_glInit -L/usr/local/lib  -lGL -lGLEW -lglfw
	
	> naman@rastogi:~/Downloads/00_glInit$ ./00_glInit 
	Renderer: Mesa DRI Intel(R) Ivybridge Mobile 
	OpenGL version supported 3.3 (Core Profile) Mesa 17.0.3
	
	> naman@rastogi:~/Downloads/00_glInit$ optirun ./00_glInit 
	Renderer: GeForce GT 540M/PCIe/SSE2
	OpenGL version supported 4.4.0 NVIDIA 331.38

Note that if you have an Nvidia graphics card, you might need to explicitly force it to run the program. Instead of running **./00 glInit** you should run **optirun ./00 glInit**. See the above example for reference. Without the graphics card, the PC only supports OpenGL 3.3 and below. With it, it
supports OpenGL 4.4 and below.

<br>
<br>

## Understanding the code

Now let us understand what exactly the code is doing, line-by-line.
To start with, we include the **GLEW** library

```cpp
#include <GL/glew.h> // include GLEW and new version of GL on Windows
```

Many graphics card vendors make extensions to the OpenGL specification by adding functions and constants or slightly modifying the behaviour of core functions. They do this so that OpenGL performs better – that is, it runs faster and consumes less memory. GLEW’s job is to find out which extensions to the OpenGL API are supported on the current machine, and load them if possible.

Next, we include the GLFW library.

```cpp
# include <GLFW/glfw3.h> // GLFW helper library
```

GLFW is a utility library for OpenGL. OpenGL is a very bare-bones library, and it does not concern itself with the creation and management of windows. As far as it is concerned, it is the job of a programmer to set up windows and manage their dimensions and so on. It only cares about having someplace that it can draw onto.

This is where GLFW comes in. It makes the job of creating and managing windows easy. It also helps in receiving input from the keyboard, mouse buttons and even joysticks.

We need to output stuff to the terminal, so we pull in iostream.

```cpp
# include <iostream>
```

Now we enter the main function.

```cpp
int main (int argc, char** argv) 
{
  if (!glfwInit ()) 
    {
      std::cerr<<"ERROR: could not start GLFW3"<<std::endl;
      return 1;
    } 
```

**glfwInit()** attempts to initialize the **GLFW** library. Needless to say, it fails if something is wrong with your installation of GLFW, or if you lack some other important component. If it fails, it calls the **glfwTerminate()** function. If it succeeds, it is the job of the programmer to call glfwTerminate() before the main program ends.

Next, we give some hints to GLFW about what version of OpenGL we want to use, whether we want forward compatibility or not, and what OpenGL profile to use. If you are not using the OS X operating system, you can safely comment out these lines.

```cpp
glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR , 3);
glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR , 2);
glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT , GL_TRUE );
glfwWindowHint( GLFW_OPENGL_PROFILE , GLFW_OPENGL_CORE_PROFILE );
```

To understand the meaning of the last line, you need to know the history of OpenGL. When OpenGL 3.0 came out, it was decided for the first time that some parts of the OpenGL API could be marked as deprecated, meaning that in future versions they would cease to be part of the spec. However, in OpenGL 3.1, an extension named ARB compatibility was introduced. If the machine running an OpenGL program supports this extension, it is capable of using the deprecated OpenGL features.

So what do the above lines do? Quoting from the [FAQ](http://www.glfw.org/faq.html) on the GLFW site:
*“The only OpenGL 3.x and 4.x contexts currently supported by OS X are forward-compatible, core profile contexts. The supported versions are 3.2 on 10.7 Lion and 3.3 and 4.1 on 10.9 Mavericks. In all cases, your GPU needs to support the specified OpenGL version for context creation to succeed.”*

Next, we create a window using GLFW. Note that we don’t have to worry about operating system-specific details. GLFW takes care of that for us.

```cpp
GLFWwindow* window = glfwCreateWindow (640, 480, "OpenGL Initialization Example", NULL, NULL);
  if (!window) 
    {
      std::cerr<<"ERROR: could not open window with GLFW3"<<std::endl;
      glfwTerminate();
      return 1;
    }
  glfwMakeContextCurrent (window);
```

The first two parameters to **glfwCreateWindow** are the width and height of the window. The third is the window name. The fourth parameter is NULL, indicating that we don’t want to use fullscreen mode. Ignore the fifth parameter for now (if you’re curious, look [here](http://www.glfw.org/docs/latest/group__window.html#ga5c336fddf2cbb5b92f65f10fb6043344)).

```cpp
glfwMakeContextCurrent( window );
```

This line tells GLFW to set the context contained by the recently created window as the current context. OpenGL has a context which encapsulates all render state, textures and shaders. You can’t use the OpenGL API unless you have a context. When we called glfwCreateWindow, it created an OpenGL context. This line tells OpenGL that this context should be made the current one. Remember, OpenGL won’t do any of this stuff for us. We are using the helper library GLFW to take care of these details involving the window/context.

```cpp
// start GLEW extension handler
glewExperimental = GL_TRUE;
glewInit() ;
```

These two lines initialise GLEW. **GL_TRUE** is an enum that resolves to true. **glewExperimental** is a global flag provided by GLEW. If set to true, it enables experimental extensions (along with regular extensions).

```cpp
// get version info
  const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
  const GLubyte* version = glGetString (GL_VERSION); // version as a string
  std::cout<<"Renderer: "<<renderer<<std::endl;
  std::cout<<"OpenGL version supported "<<version<<std::endl;
```

We read in the **GL_RENDERER** and **GL_VERSION** strings into **GLubyte*** (unsigned byte) variables, then print them out. **glGetString** can return a lot of information about your machine. Look [here](https://www.khronos.org/opengl/wiki/GLAPI/glGetString) for details.

```cpp
// close GL context and any other GLFW resources
  glfwTerminate();
  return 0;
}
```

As promised, we call **glfwTerminate()** and exit the main function after returning 0.
