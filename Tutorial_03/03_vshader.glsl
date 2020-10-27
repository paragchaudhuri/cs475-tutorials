#version 330

in vec4 vPosition;
in vec4 vColor;
out vec4 color;
uniform mat4 uModelViewProjectMatrix;

void main (void) 
{
  gl_Position = uModelViewProjectMatrix * vPosition;
  color = vColor;
}
