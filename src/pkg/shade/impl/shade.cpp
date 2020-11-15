#include "include/shade.h"


#include <cstring>
#include <fstream>
#include <iostream>

namespace shade {

Shader::Shader(Type t, std::string&& f)
  : type_(t)
  , shader_id_(0)
  , raw_ref_(nullptr)
  , file_(f)
{
  shader_id_ = glCreateShader(type_gl_map.at(type_));
  glShaderSource(shader_id_, 1, shader(), nullptr);
  glCompileShader(shader_id_);

  GLint success;
  GLchar log[512];
  glGetShaderiv(shader_id_, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader_id_, 512, NULL, log);
    throw std::runtime_error(log);
  }
}

Shader::~Shader() {
  glDeleteShader(shader_id_);
}

uint32_t Shader::id() const {
  return static_cast<uint32_t>(shader_id_);
}

const char* const* Shader::shader() {
  raw_ref_ = file_.mut_raw_bytes();
  return &raw_ref_;
}

std::string Shader::as_str() {
  return std::string(file_.raw_bytes());
}

Program::Program()
  : program_id_(0)
  , shader_ids_({})
{
  program_id_ = glCreateProgram();
}

uint32_t Program::id() const {
  return static_cast<uint32_t>(program_id_);
}

void Program::attach(const Shader& s) {
  shader_ids_.push_back(s.id());
  glAttachShader(program_id_, s.id());
}

void Program::link() {
  glLinkProgram(program_id_);

  GLint success;
  GLchar log[512];
  glGetProgramiv(program_id_, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program_id_, 512, nullptr, log);
    throw std::runtime_error(log);
  }
}

} // end namespace shade