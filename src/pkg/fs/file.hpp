#pragma once

#include <atomic>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <string>

namespace fs {

/*
Target API:

fs::file f("some/path"):

*/

template <typename T>
class file {
  std::atomic<bool> is_ok_;
  std::string path_;
  std::vector<T> bytes_;
public:
  file(std::string path)
    : is_ok_(false)
    , path_(path)
    , bytes_({})
  {
    std::ifstream tmp(path_, std::ios::in | std::ios::binary);
    is_ok_ = tmp.is_open();
    if (!is_ok_)
      throw std::runtime_error("Error opening file: " + path);

    tmp.seekg(0, std::ios::end);
    std::streamsize sz = tmp.tellg();
    tmp.seekg(0, std::ios::beg);
    bytes_.resize(sz);

    if (!tmp.read(bytes_.data(), bytes_.size()))
      throw std::runtime_error("Error reading bytes into memory: " + path);
  }

  bool ok() {
    return is_ok_.load();
  }

  size_t size() {
    return bytes_.size();
  }

  const std::vector<T>& bytes() {
    return bytes_;
  }

  const T* raw_bytes() {
    return &bytes_[0];
  }

  T* mut_raw_bytes() {
    return &bytes_[0];
  }
};

} // end namespace fs