#pragma once

#include <cstdint>

namespace shade {
namespace traits {

class Identifiable {
public:
  virtual uint32_t id() const = 0;
};

class Shadeable {
public:
  virtual const char* const* shader() = 0;
};

} // end namespace traits
} // end namespace shade