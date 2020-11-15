#include "gtest/gtest.h"

#include "include/shade.h"

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);

  shade::Shader s(argv[1]);
  s.load();

  return RUN_ALL_TESTS();
}