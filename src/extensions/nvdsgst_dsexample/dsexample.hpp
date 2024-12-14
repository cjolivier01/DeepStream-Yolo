#pragma once

#include <string>

#include "nvdsbase/nvds_io.hpp"

namespace nvidia {
namespace deepstream {

class GstDsExample : public INvDsElement {
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

    auto p_unique_id = unique_id.try_get();
    if (p_unique_id && p_unique_id.value() != 15UL) {
      guint propvalue = (guint)p_unique_id.value();
      g_object_set(element_, "unique-id", propvalue, NULL);
    }

    auto p_processing_width = processing_width.try_get();
    if (p_processing_width && p_processing_width.value() != 640L) {
      gint propvalue = (gint)p_processing_width.value();
      g_object_set(element_, "processing-width", propvalue, NULL);
    }

    auto p_processing_height = processing_height.try_get();
    if (p_processing_height && p_processing_height.value() != 480L) {
      gint propvalue = (gint)p_processing_height.value();
      g_object_set(element_, "processing-height", propvalue, NULL);
    }

    auto p_full_frame = full_frame.try_get();
    if (p_full_frame && p_full_frame.value() != true) {
      gboolean propvalue = (gboolean)p_full_frame.value();
      g_object_set(element_, "full-frame", propvalue, NULL);
    }

    auto p_batch_size = batch_size.try_get();
    if (p_batch_size && p_batch_size.value() != 1UL) {
      guint propvalue = (guint)p_batch_size.value();
      g_object_set(element_, "batch-size", propvalue, NULL);
    }

    auto p_blur_objects = blur_objects.try_get();
    if (p_blur_objects && p_blur_objects.value() != false) {
      gboolean propvalue = (gboolean)p_blur_objects.value();
      g_object_set(element_, "blur-objects", propvalue, NULL);
    }

    auto p_gpu_id = gpu_id.try_get();
    if (p_gpu_id && p_gpu_id.value() != 0UL) {
      guint propvalue = (guint)p_gpu_id.value();
      g_object_set(element_, "gpu-id", propvalue, NULL);
    }

    return GXF_SUCCESS;
  }

  GstElement *get_element_ptr() override { return element_; }

  gxf_result_t registerInterface(nvidia::gxf::Registrar *registrar) override {
    nvidia::gxf::Expected<void> result;
    result &= registrar->parameter(qos, "qos", "QoS",
                                   "Handle Quality-of-Service events", false,
                                   GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(unique_id, "unique-id", "Unique ID",
                                   "Unique ID for the element. Can be used to "
                                   "identify output of the element",
                                   15UL, GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(processing_width, "processing-width",
                                   "Processing Width",
                                   "Width of the input buffer to algorithm",
                                   640L, GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(processing_height, "processing-height",
                                   "Processing Height",
                                   "Height of the input buffer to algorithm",
                                   480L, GXF_PARAMETER_FLAGS_OPTIONAL);
    result &=
        registrar->parameter(full_frame, "full-frame", "Full frame",
                             "Enable to process full frame or disable to "
                             "process objects detectedby primary detector",
                             true, GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(batch_size, "batch-size", "Batch Size",
                                   "Maximum batch size for processing", 1UL,
                                   GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(blur_objects, "blur-objects", "Blur Objects",
                                   "Enable to blur the objects detected in "
                                   "full-frame=0 modeby primary detector",
                                   false, GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(gpu_id, "gpu-id", "Set GPU Device ID",
                                   "Set GPU Device ID", 0UL,
                                   GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(
        src_pad, "video-out", "video-out",
        "Handle to a nvidia::deepstream::NvDsStaticOutput component. Supported "
        "formats - video(NV12, RGBA, I420)",
        gxf::Registrar::NoDefaultParameter(), GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(
        sink_pad, "video-in", "video-in",
        "Handle to a nvidia::deepstream::NvDsStaticInput component. Supported "
        "formats - video(NV12, RGBA, I420)",
        gxf::Registrar::NoDefaultParameter(), GXF_PARAMETER_FLAGS_OPTIONAL);
    return nvidia::gxf::ToResultCode(result);
  }

  nvidia::gxf::Parameter<bool> qos;
  nvidia::gxf::Parameter<uint64_t> unique_id;
  nvidia::gxf::Parameter<int64_t> processing_width;
  nvidia::gxf::Parameter<int64_t> processing_height;
  nvidia::gxf::Parameter<bool> full_frame;
  nvidia::gxf::Parameter<uint64_t> batch_size;
  nvidia::gxf::Parameter<bool> blur_objects;
  nvidia::gxf::Parameter<uint64_t> gpu_id;
  nvidia::gxf::Parameter<nvidia::gxf::Handle<NvDsStaticOutput>> src_pad;
  nvidia::gxf::Parameter<nvidia::gxf::Handle<NvDsStaticInput>> sink_pad;

 protected:
  GstElement *element_;
  const char *GST_ELEMENT_NAME = "dsexample";
};

#define GXF_EXT_FACTORY_ADD_GstDsExample()                                  \
  do {                                                                      \
    GXF_EXT_FACTORY_ADD_VERBOSE(                                            \
        0x8edc9cbff17d3adaUL, 0x9d1a1f00de624954UL,                         \
        nvidia::deepstream::GstDsExample, nvidia::deepstream::INvDsElement, \
        "GstDsExample placeholder display-name",                            \
        "GstDsExample placeholder brief",                                   \
        "Process a 3rdparty example algorithm on objects / full frame");    \
  } while (0)

}  // namespace deepstream
}  // namespace nvidia
