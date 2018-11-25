#pragma once

#ifndef __INITSHADER_H__
#define __INITSHADER_H__

#include <string>
using std::string;

string ReadShaderSource(const string& shaderFile);
GLuint InitShader(const string& vShaderFile, const string& fShaderFile);

#endif // !__INITSHADER_H__
