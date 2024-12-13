#pragma once

#include <string>

#include "nvdsbase/nvds_io.hpp"

namespace nvidia {
namespace deepstream {

class GstNvDsOsd : public INvDsElement {
 public:
  gxf_result_t initialize() override {
    GXF_LOG_INFO("initialize: %s %s\n", GST_ELEMENT_NAME, name());
    return GXF_SUCCESS;
  }

  gxf_result_t create_element() override {
    std::string ename = entity().name();
    ename += "/";
    ename += name();
    GXF_LOG_INFO("create_element: %s %s\n", GST_ELEMENT_NAME, name());
    element_ = gst_element_factory_make(GST_ELEMENT_NAME, ename.c_str());

    if (!element_) {
      GXF_LOG_ERROR("Could not create GStreamer element '%s'",
                    GST_ELEMENT_NAME);
      return GXF_FAILURE;
    }

    auto p_src_pad = src_pad.try_get();
    if (p_src_pad) {
      p_src_pad.value()->set_element(this);
      p_src_pad.value()->set_template_name("src");
    }

    auto p_sink_pad = sink_pad.try_get();
    if (p_sink_pad) {
      p_sink_pad.value()->set_element(this);
      p_sink_pad.value()->set_template_name("sink");
    }
    return GXF_SUCCESS;
  }

  gxf_result_t bin_add(GstElement *pipeline) override {
    GXF_LOG_INFO("bin_add: %s %s\n", GST_ELEMENT_NAME, name());
    if (!gst_bin_add(GST_BIN(pipeline), element_)) {
      return GXF_FAILURE;
    }

    auto p_qos = qos.try_get();
    if (p_qos && p_qos.value() != false) {
      gboolean propvalue = (gboolean)p_qos.value();
      g_object_set(element_, "qos", propvalue, NULL);
    }

    auto p_display_clock = display_clock.try_get();
    if (p_display_clock && p_display_clock.value() != false) {
      gboolean propvalue = (gboolean)p_display_clock.value();
      g_object_set(element_, "display-clock", propvalue, NULL);
    }

    auto p_display_text = display_text.try_get();
    if (p_display_text && p_display_text.value() != true) {
      gboolean propvalue = (gboolean)p_display_text.value();
      g_object_set(element_, "display-text", propvalue, NULL);
    }

    auto p_clock_font = clock_font.try_get();
    if (p_clock_font && p_clock_font.value() != std::string{""}) {
      gchararray propvalue = (gchararray)p_clock_font.value().c_str();
      g_object_set(element_, "clock-font", propvalue, NULL);
    }

    auto p_clock_font_size = clock_font_size.try_get();
    if (p_clock_font_size && p_clock_font_size.value() != 0UL) {
      guint propvalue = (guint)p_clock_font_size.value();
      g_object_set(element_, "clock-font-size", propvalue, NULL);
    }

    auto p_x_clock_offset = x_clock_offset.try_get();
    if (p_x_clock_offset && p_x_clock_offset.value() != 0UL) {
      guint propvalue = (guint)p_x_clock_offset.value();
      g_object_set(element_, "x-clock-offset", propvalue, NULL);
    }

    auto p_y_clock_offset = y_clock_offset.try_get();
    if (p_y_clock_offset && p_y_clock_offset.value() != 0UL) {
      guint propvalue = (guint)p_y_clock_offset.value();
      g_object_set(element_, "y-clock-offset", propvalue, NULL);
    }

    auto p_clock_color = clock_color.try_get();
    if (p_clock_color && p_clock_color.value() != 0UL) {
      guint propvalue = (guint)p_clock_color.value();
      g_object_set(element_, "clock-color", propvalue, NULL);
    }

    auto p_process_mode = process_mode.try_get();
    if (p_process_mode && p_process_mode.value() != 1L) {
      gint64 propvalue = (gint64)p_process_mode.value();
      g_object_set(element_, "process-mode", propvalue, NULL);
    }

    auto p_gpu_id = gpu_id.try_get();
    if (p_gpu_id && p_gpu_id.value() != 0UL) {
      guint propvalue = (guint)p_gpu_id.value();
      g_object_set(element_, "gpu-id", propvalue, NULL);
    }

    auto p_display_bbox = display_bbox.try_get();
    if (p_display_bbox && p_display_bbox.value() != true) {
      gboolean propvalue = (gboolean)p_display_bbox.value();
      g_object_set(element_, "display-bbox", propvalue, NULL);
    }

    auto p_display_mask = display_mask.try_get();
    if (p_display_mask && p_display_mask.value() != false) {
      gboolean propvalue = (gboolean)p_display_mask.value();
      g_object_set(element_, "display-mask", propvalue, NULL);
    }

    return GXF_SUCCESS;
  }

  GstElement *get_element_ptr() override { return element_; }

  gxf_result_t registerInterface(nvidia::gxf::Registrar *registrar) override {
    nvidia::gxf::Expected<void> result;
    result &= registrar->parameter(qos, "qos", "QoS",
                                   "Handle Quality-of-Service events", false,
                                   GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(display_clock, "display-clock", "clock",
                                   "Whether to display clock", false,
                                   GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(display_text, "display-text", "text",
                                   "Whether to display text", true,
                                   GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(clock_font, "clock-font", "clock-font",
                                   "Clock Font to be set", std::string{""},
                                   GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(clock_font_size, "clock-font-size",
                                   "clock-font-size", "font size of the clock",
                                   0UL, GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(x_clock_offset, "x-clock-offset",
                                   "x-clock-offset", "x-clock-offset", 0UL,
                                   GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(y_clock_offset, "y-clock-offset",
                                   "y-clock-offset", "y-clock-offset", 0UL,
                                   GXF_PARAMETER_FLAGS_OPTIONAL);
    result &=
        registrar->parameter(clock_color, "clock-color", "clock-color",
                             "clock-color", 0UL, GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(
        process_mode, "process-mode", "Process Mode",
        "Rect and text draw process mode, CPU_MODE only support RGBA "
        "format\nValid values:\n  0: MODE_CPU\n  1: MODE_GPU",
        1L, GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(gpu_id, "gpu-id", "Set GPU Device ID",
                                   "Set GPU Device ID", 0UL,
                                   GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(display_bbox, "display-bbox", "text",
                                   "Whether to display bounding boxes", true,
                                   GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(display_mask, "display-mask", "text",
                                   "Whether to display instance mask", false,
                                   GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(
        src_pad, "video-out", "video-out",
        "Handle to a nvidia::deepstream::NvDsStaticOutput component. Supported "
        "formats - video(NV12, RGBA)",
        gxf::Registrar::NoDefaultParameter(), GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(
        sink_pad, "video-in", "video-in",
        "Handle to a nvidia::deepstream::NvDsStaticInput component. Supported "
        "formats - video(NV12, RGBA)",
        gxf::Registrar::NoDefaultParameter(), GXF_PARAMETER_FLAGS_OPTIONAL);
    return nvidia::gxf::ToResultCode(result);
  }

  nvidia::gxf::Parameter<bool> qos;
  nvidia::gxf::Parameter<bool> display_clock;
  nvidia::gxf::Parameter<bool> display_text;
  nvidia::gxf::Parameter<std::string> clock_font;
  nvidia::gxf::Parameter<uint64_t> clock_font_size;
  nvidia::gxf::Parameter<uint64_t> x_clock_offset;
  nvidia::gxf::Parameter<uint64_t> y_clock_offset;
  nvidia::gxf::Parameter<uint64_t> clock_color;
  nvidia::gxf::Parameter<int64_t> process_mode;
  nvidia::gxf::Parameter<uint64_t> gpu_id;
  nvidia::gxf::Parameter<bool> display_bbox;
  nvidia::gxf::Parameter<bool> display_mask;
  nvidia::gxf::Parameter<nvidia::gxf::Handle<NvDsStaticOutput>> src_pad;
  nvidia::gxf::Parameter<nvidia::gxf::Handle<NvDsStaticInput>> sink_pad;

 protected:
  GstElement *element_;
  const char *GST_ELEMENT_NAME = "nvdsosd";
};

#define GXF_EXT_FACTORY_ADD_GstNvDsOsd()                                       \
  do {                                                                         \
    GXF_EXT_FACTORY_ADD_VERBOSE(                                               \
        0x6f4c209e60103b8eUL, 0xa203b0b06304cdb7UL,                            \
        nvidia::deepstream::GstNvDsOsd, nvidia::deepstream::INvDsElement,      \
        "GstNvDsOsd placeholder display-name", "GstNvDsOsd placeholder brief", \
        "Gstreamer bounding box draw element");                                \
  } while (0)

}  // namespace deepstream
}  // namespace nvidia
