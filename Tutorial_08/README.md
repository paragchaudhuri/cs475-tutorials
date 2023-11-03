# Tutorial 8 - Saving the Framebuffer

<br>
<br>

## About

The code is very similar to most of the tutorials before, you can run the code simply by running 

    ./08_fbsave

<br>
<br>

## Understanding the code

The "save_fb_toimage" function in "gl_framework.cpp" is called after "renderGL" in the main GLFW loop to capture the back buffer into a jpg file. The jpg write function is from the STB image library (https://github.com/nothings/stb). 

The save is initiated by pressing the "Z" key and is always saved to a "saved_frame.jpg" file.

## References

1. [STB Image](https://github.com/nothings/stb)
