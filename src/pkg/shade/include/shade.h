#ifndef SHADE_H
#define SHADE_H

#include "traits.h"

#include "pkg/fs/file.hpp"

#include <GL/glew.h>

#include <map>
#include <string>
#include <vector>

namespace shade {

enum class Type {
  VRTX,
  FRAG
};

static const std::map<Type, int> type_gl_map = {
  {Type::VRTX, GL_VERTEX_SHADER},
  {Type::FRAG, GL_FRAGMENT_SHADER}
};

class Shader
  : public traits::Identifiable
  , public traits::Shadeable
{
  Type type_;
  GLuint shader_id_;
  char* raw_ref_;
  fs::file<char> file_;
public:
  Shader(Type t, std::string&& f);
  ~Shader();
  uint32_t id() const;
  const char* const* shader();
  std::string as_str();
};

class Program
  : public traits::Identifiable
{
  GLuint program_id_;
  std::vector<uint32_t> shader_ids_;
public:
  Program();
  uint32_t id() const;
  void attach(const Shader& s);
  void link();
};

} //end namespace shade

#endif