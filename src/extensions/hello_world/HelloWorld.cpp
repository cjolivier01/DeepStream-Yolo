
#include "HelloWorld.hpp" // NOLINT

#include <cassert>

#include <unistd.h>
#include <iostream>

namespace sample {
namespace test {

gxf_result_t HelloWorld::start() {
  assert(false);
  #ifdef NDEBUG
  #error "not a debug build"
  #endif
  GXF_LOG_INFO("Starting this thing again...");
  GXF_LOG_INFO("HelloWorld::start");
  return GXF_SUCCESS;
}

gxf_result_t HelloWorld::tick() {
  GXF_LOG_INFO("HelloWorld::tick");
  return GXF_SUCCESS;
}

gxf_result_t HelloWorld::stop() {
  GXF_LOG_INFO("HelloWorld::stop");
  return GXF_SUCCESS;
}

} // namespace test
} // namespace sample
