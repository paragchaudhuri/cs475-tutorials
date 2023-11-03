#version 330

in vec4 vPosition;
in vec2 texCoord;
in vec3 vNormal;

out vec2 tex;
uniform mat4 uModelViewMatrix;
uniform mat3 normalMatrix;
uniform mat4 viewMatrix;

void main (void) 
{
  gl_Position = uModelViewMatrix * vPosition;
  tex = texCoord;
}
