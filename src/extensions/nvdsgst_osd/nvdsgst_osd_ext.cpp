#include "gxf/std/extension_factory_helper.hpp"
#include "nvdsosd.hpp"

GXF_EXT_FACTORY_BEGIN()

GXF_EXT_FACTORY_SET_INFO(0x72589cb9f267375d, 0x87aeb8a9a72f9cff, "nvdsgst_osd",
                          "Gstreamer plugin to draw rectangles and text", "AUTHOR", "0.0.1",
                          "LICENSE");

GXF_EXT_FACTORY_ADD_GstNvDsOsd();

GXF_EXT_FACTORY_END()
