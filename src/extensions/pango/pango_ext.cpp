#include "gxf/std/extension_factory_helper.hpp"
#include "timeoverlay.hpp"

GXF_EXT_FACTORY_BEGIN()

GXF_EXT_FACTORY_SET_INFO(0xb8c238de21293a4f, 0x99d0ddb9432df5b2, "pango",
                          "Pango-based text rendering and overlay", "AUTHOR", "0.0.1",
                          "LICENSE");

GXF_EXT_FACTORY_ADD_GstTimeOverlay();

GXF_EXT_FACTORY_END()
