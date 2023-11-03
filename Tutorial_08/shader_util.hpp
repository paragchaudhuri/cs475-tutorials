#ifndef _SHADER_UTIL_HPP_
#define _SHADER_UTIL_HPP_

#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>


namespace csX75
{
  GLuint LoadShaderGL(GLenum eShaderType, const std::string &strFilename);
  GLuint CreateShaderGL(GLenum eShaderType, const std::string &strShaderFile);
  GLuint CreateProgramGL(const std::vector<GLuint> &shaderList);
};

#endif
