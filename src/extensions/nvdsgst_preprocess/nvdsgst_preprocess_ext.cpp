#include "gxf/std/extension_factory_helper.hpp"
#include "nvdspreprocess.hpp"

GXF_EXT_FACTORY_BEGIN()

GXF_EXT_FACTORY_SET_INFO(0x69afa92ff30c3ed2, 0xadb916e50946887d, "nvdsgst_preprocess",
                          "NVIDIA custom preprocessing plugin for integration with DeepStream on DGPU/Jetson", "AUTHOR", "0.0.1",
                          "LICENSE");

GXF_EXT_FACTORY_ADD_GstNvDsPreProcess();

GXF_EXT_FACTORY_END()
