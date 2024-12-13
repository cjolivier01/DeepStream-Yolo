#pragma once

#include <string>

#include "nvdsbase/nvds_io.hpp"
#include "nvinfer_interfaces.hpp"

namespace nvidia {
namespace deepstream {

class GstNvInfer : public INvDsElement {
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

    if (model_updated_.try_get()) {
      model_updated_.try_get().value()->set_element(this);
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

    auto p_process_mode = process_mode.try_get();
    if (p_process_mode && p_process_mode.value() != 1L) {
      gint64 propvalue = (gint64)p_process_mode.value();
      g_object_set(element_, "process-mode", propvalue, NULL);
    }

    auto p_config_file_path = config_file_path.try_get();
    if (p_config_file_path && p_config_file_path.value() != std::string{""}) {
      gchararray propvalue = (gchararray)p_config_file_path.value().c_str();
      g_object_set(element_, "config-file-path", propvalue, NULL);
    }

    auto p_infer_on_gie_id = infer_on_gie_id.try_get();
    if (p_infer_on_gie_id && p_infer_on_gie_id.value() != -1L) {
      gint propvalue = (gint)p_infer_on_gie_id.value();
      g_object_set(element_, "infer-on-gie-id", propvalue, NULL);
    }

    auto p_infer_on_class_ids = infer_on_class_ids.try_get();
    if (p_infer_on_class_ids &&
        p_infer_on_class_ids.value() != std::string{""}) {
      gchararray propvalue = (gchararray)p_infer_on_class_ids.value().c_str();
      g_object_set(element_, "infer-on-class-ids", propvalue, NULL);
    }

    auto p_filter_out_class_ids = filter_out_class_ids.try_get();
    if (p_filter_out_class_ids &&
        p_filter_out_class_ids.value() != std::string{""}) {
      gchararray propvalue = (gchararray)p_filter_out_class_ids.value().c_str();
      g_object_set(element_, "filter-out-class-ids", propvalue, NULL);
    }

    auto p_model_engine_file = model_engine_file.try_get();
    if (p_model_engine_file && p_model_engine_file.value() != std::string{""}) {
      gchararray propvalue = (gchararray)p_model_engine_file.value().c_str();
      g_object_set(element_, "model-engine-file", propvalue, NULL);
    }

    auto p_batch_size = batch_size.try_get();
    if (p_batch_size && p_batch_size.value() != 1UL) {
      guint propvalue = (guint)p_batch_size.value();
      g_object_set(element_, "batch-size", propvalue, NULL);
    }

    auto p_interval = interval.try_get();
    if (p_interval && p_interval.value() != 0UL) {
      guint propvalue = (guint)p_interval.value();
      g_object_set(element_, "interval", propvalue, NULL);
    }

    auto p_gpu_id = gpu_id.try_get();
    if (p_gpu_id && p_gpu_id.value() != 0UL) {
      guint propvalue = (guint)p_gpu_id.value();
      g_object_set(element_, "gpu-id", propvalue, NULL);
    }

    auto p_raw_output_file_write = raw_output_file_write.try_get();
    if (p_raw_output_file_write && p_raw_output_file_write.value() != false) {
      gboolean propvalue = (gboolean)p_raw_output_file_write.value();
      g_object_set(element_, "raw-output-file-write", propvalue, NULL);
    }

    /* auto p_raw_output_generated_callback =
    raw_output_generated_callback.try_get(); if (p_raw_output_generated_callback
    && p_raw_output_generated_callback.value() != None) { gpointer propvalue =
    (gpointer) p_raw_output_generated_callback.value(); g_object_set (element_,
    "raw-output-generated-callback", propvalue, NULL);
    } */

    /* auto p_raw_output_generated_userdata =
    raw_output_generated_userdata.try_get(); if (p_raw_output_generated_userdata
    && p_raw_output_generated_userdata.value() != None) { gpointer propvalue =
    (gpointer) p_raw_output_generated_userdata.value(); g_object_set (element_,
    "raw-output-generated-userdata", propvalue, NULL);
    } */

    auto p_output_tensor_meta = output_tensor_meta.try_get();
    if (p_output_tensor_meta && p_output_tensor_meta.value() != false) {
      gboolean propvalue = (gboolean)p_output_tensor_meta.value();
      g_object_set(element_, "output-tensor-meta", propvalue, NULL);
    }

    auto p_output_instance_mask = output_instance_mask.try_get();
    if (p_output_instance_mask && p_output_instance_mask.value() != false) {
      gboolean propvalue = (gboolean)p_output_instance_mask.value();
      g_object_set(element_, "output-instance-mask", propvalue, NULL);
    }

    auto p_input_tensor_meta = input_tensor_meta.try_get();
    if (p_input_tensor_meta && p_input_tensor_meta.value() != false) {
      gboolean propvalue = (gboolean)p_input_tensor_meta.value();
      g_object_set(element_, "input-tensor-meta", propvalue, NULL);
    }

    auto p_clip_object_outside_roi = clip_object_outside_roi.try_get();
    if (p_clip_object_outside_roi &&
        p_clip_object_outside_roi.value() != true) {
      gboolean propvalue = (gboolean)p_clip_object_outside_roi.value();
      g_object_set(element_, "clip-object-outside-roi", propvalue, NULL);
    }

    auto p_crop_objects_to_roi_boundary =
        crop_objects_to_roi_boundary.try_get();
    if (p_crop_objects_to_roi_boundary &&
        p_crop_objects_to_roi_boundary.value() != false) {
      gboolean propvalue = (gboolean)p_crop_objects_to_roi_boundary.value();
      g_object_set(element_, "crop-objects-to-roi-boundary", propvalue, NULL);
    }

    return GXF_SUCCESS;
  }

  GstElement *get_element_ptr() override { return element_; }

  gxf_result_t registerInterface(nvidia::gxf::Registrar *registrar) override {
    nvidia::gxf::Expected<void> result;
    result &= registrar->parameter(
        model_updated_, "model-updated-signal", "Model Updated Signal",
        "Handle to a NvDsModelUpdatedSignal component",
        gxf::Registrar::NoDefaultParameter(), GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(qos, "qos", "QoS",
                                   "Handle Quality-of-Service events", false,
                                   GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(unique_id, "unique-id", "Unique ID",
                                   "Unique ID for the element. Can be used to "
                                   "identify output of the element",
                                   15UL, GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(
        process_mode, "process-mode", "Process Mode",
        "Infer processing mode\nValid values:\n  1: primary\n  2: secondary",
        1L, GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(
        config_file_path, "config-file-path", "Config File Path",
        "Path to the configuration file for this instance of nvinfer",
        std::string{""}, GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(
        infer_on_gie_id, "infer-on-gie-id", "Infer on Gie ID",
        "Infer on metadata generated by GIE with this unique ID. Set to -1 to "
        "infer on all metadata.",
        -1L, GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(
        infer_on_class_ids, "infer-on-class-ids", "Operate on Class ids",
        "Operate on objects with specified class ids Use string with values of "
        "class ids in ClassID (int) to set the property. e.g. 0:2:3",
        std::string{""}, GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(
        filter_out_class_ids, "filter-out-class-ids",
        "Ignore metadata for class ids",
        "Ignore metadata for objects of specified class ids Use string with "
        "values of class ids in ClassID (int) to set the property. e.g. 0;2;3",
        std::string{""}, GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(
        model_engine_file, "model-engine-file", "Model Engine File",
        "Absolute path to the pre-generated serialized engine file for the "
        "model",
        std::string{""}, GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(batch_size, "batch-size", "Batch Size",
                                   "Maximum batch size for inference", 1UL,
                                   GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(
        interval, "interval", "Interval",
        "Specifies number of consecutive batches to be skipped for inference",
        0UL, GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(gpu_id, "gpu-id", "Set GPU Device ID",
                                   "Set GPU Device ID", 0UL,
                                   GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(
        raw_output_file_write, "raw-output-file-write", "Raw Output File Write",
        "Write raw inference output to file", false,
        GXF_PARAMETER_FLAGS_OPTIONAL);
    // result &= registrar->parameter(raw_output_generated_callback,
    // "raw-output-generated-callback", "Raw Output Generated Callback",
    // "Pointer to the raw output generated callback funtion (type:
    // gst_nvinfer_raw_output_generated_callback in 'gstnvdsinfer.h')", None,
    // GXF_PARAMETER_FLAGS_OPTIONAL); result &=
    // registrar->parameter(raw_output_generated_userdata,
    // "raw-output-generated-userdata", "Raw Output Generated UserData",
    // "Pointer to the userdata to be supplied with raw output generated
    // callback", None, GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(
        output_tensor_meta, "output-tensor-meta", "Output Tensor Meta",
        "Attach inference tensor outputs as buffer metadata", false,
        GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(
        output_instance_mask, "output-instance-mask", "Output Instance Mask",
        "Instance mask expected in network output and attach it to metadata",
        false, GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(
        input_tensor_meta, "input-tensor-meta", "Input Tensor Meta",
        "Use preprocessed input tensors attached as metadata instead of "
        "preprocessing inside the plugin",
        false, GXF_PARAMETER_FLAGS_OPTIONAL);
    result &=
        registrar->parameter(clip_object_outside_roi, "clip-object-outside-roi",
                             "Clip Object Outside Roi",
                             "Clip the object bounding-box which lies outside "
                             "the roi specified by nvdspreprosess plugin",
                             true, GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(
        crop_objects_to_roi_boundary, "crop-objects-to-roi-boundary",
        "Crop Object to Roi Boundary",
        "Clip the object bounding-box which lies outside the roi boundary",
        false, GXF_PARAMETER_FLAGS_OPTIONAL);
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

  nvidia::gxf::Parameter<nvidia::gxf::Handle<NvDsModelUpdatedSignal>>
      model_updated_;
  nvidia::gxf::Parameter<bool> qos;
  nvidia::gxf::Parameter<uint64_t> unique_id;
  nvidia::gxf::Parameter<int64_t> process_mode;
  nvidia::gxf::Parameter<std::string> config_file_path;
  nvidia::gxf::Parameter<int64_t> infer_on_gie_id;
  nvidia::gxf::Parameter<std::string> infer_on_class_ids;
  nvidia::gxf::Parameter<std::string> filter_out_class_ids;
  nvidia::gxf::Parameter<std::string> model_engine_file;
  nvidia::gxf::Parameter<uint64_t> batch_size;
  nvidia::gxf::Parameter<uint64_t> interval;
  nvidia::gxf::Parameter<uint64_t> gpu_id;
  nvidia::gxf::Parameter<bool> raw_output_file_write;
  // nvidia::gxf::Parameter<Unknown> raw_output_generated_callback;
  // nvidia::gxf::Parameter<Unknown> raw_output_generated_userdata;
  nvidia::gxf::Parameter<bool> output_tensor_meta;
  nvidia::gxf::Parameter<bool> output_instance_mask;
  nvidia::gxf::Parameter<bool> input_tensor_meta;
  nvidia::gxf::Parameter<bool> clip_object_outside_roi;
  nvidia::gxf::Parameter<bool> crop_objects_to_roi_boundary;
  nvidia::gxf::Parameter<nvidia::gxf::Handle<NvDsStaticOutput>> src_pad;
  nvidia::gxf::Parameter<nvidia::gxf::Handle<NvDsStaticInput>> sink_pad;

 protected:
  GstElement *element_;
  const char *GST_ELEMENT_NAME = "nvinfer";
};

#define GXF_EXT_FACTORY_ADD_GstNvInfer()                                       \
  do {                                                                         \
    GXF_EXT_FACTORY_ADD_VERBOSE(                                               \
        0x696e5af8f0793fbdUL, 0xad280618e223d6a5UL,                            \
        nvidia::deepstream::NvDsModelUpdatedSignal,                            \
        nvidia::deepstream::INvDsSignal,                                       \
        "model-updated placeholder display-name",                              \
        "model-updated placeholder brief",                                     \
        "model-updated placeholder description. This is a connector "          \
        "component. This component must be linked to 'GstNvInfer' which "      \
        "emits the signal and another component which will handle the signal " \
        "callback.");                                                          \
                                                                               \
    GXF_EXT_FACTORY_ADD_VERBOSE(                                               \
        0x41dd4396eda93671UL, 0xbad5bd355b4e57eeUL,                            \
        nvidia::deepstream::GstNvInfer, nvidia::deepstream::INvDsElement,      \
        "GstNvInfer placeholder display-name", "GstNvInfer placeholder brief", \
        "Nvidia DeepStreamSDK TensorRT plugin");                               \
  } while (0)

}  // namespace deepstream
}  // namespace nvidia
