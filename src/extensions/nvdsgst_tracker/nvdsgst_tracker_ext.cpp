#include "gxf/std/extension_factory_helper.hpp"
#include "nvtracker.hpp"

GXF_EXT_FACTORY_BEGIN()

GXF_EXT_FACTORY_SET_INFO(0x34f3520868dc3f73, 0xbab390eceb896945, "nvdsgst_tracker",
                          "Gstreamer plugin to track the objects", "AUTHOR", "0.0.1",
                          "LICENSE");

GXF_EXT_FACTORY_ADD_GstNvTracker();

GXF_EXT_FACTORY_END()
