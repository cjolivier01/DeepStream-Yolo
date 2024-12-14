#include "gxf/std/extension_factory_helper.hpp"
#include "dsexample.hpp"

GXF_EXT_FACTORY_BEGIN()

GXF_EXT_FACTORY_SET_INFO(0xc1de0ee4293731d6, 0x8720b5f074bb5b53, "nvdsgst_dsexample",
                          "NVIDIA example plugin for integration with DeepStream on DGPU", "AUTHOR", "0.0.1",
                          "LICENSE");

GXF_EXT_FACTORY_ADD_GstDsExample();

GXF_EXT_FACTORY_END()
