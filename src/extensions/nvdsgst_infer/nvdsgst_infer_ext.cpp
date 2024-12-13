#include "gxf/std/extension_factory_helper.hpp"
#include "nvinfer.hpp"

GXF_EXT_FACTORY_BEGIN()

GXF_EXT_FACTORY_SET_INFO(0x1ca0f521371e3c65, 0x80fd0b6a7cac4082, "nvdsgst_infer",
                          "NVIDIA DeepStreamSDK TensorRT plugin", "AUTHOR", "0.0.1",
                          "LICENSE");

GXF_EXT_FACTORY_ADD_GstNvInfer();

GXF_EXT_FACTORY_END()
