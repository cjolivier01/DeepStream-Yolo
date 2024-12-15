#pragma once

#include <string>

#include "nvdsbase/nvds_io.hpp"

namespace nvidia {
namespace deepstream {

class GstNvTracker : public INvDsElement {
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

    auto p_tracker_width = tracker_width.try_get();
    if (p_tracker_width && p_tracker_width.value() != 960UL) {
      guint propvalue = (guint)p_tracker_width.value();
      g_object_set(element_, "tracker-width", propvalue, NULL);
    }

    auto p_tracker_height = tracker_height.try_get();
    if (p_tracker_height && p_tracker_height.value() != 544UL) {
      guint propvalue = (guint)p_tracker_height.value();
      g_object_set(element_, "tracker-height", propvalue, NULL);
    }

    auto p_gpu_id = gpu_id.try_get();
    if (p_gpu_id && p_gpu_id.value() != 0UL) {
      guint propvalue = (guint)p_gpu_id.value();
      g_object_set(element_, "gpu-id", propvalue, NULL);
    }

    auto p_ll_config_file = ll_config_file.try_get();
    if (p_ll_config_file && p_ll_config_file.value() != std::string{""}) {
      gchararray propvalue = (gchararray)p_ll_config_file.value().c_str();
      g_object_set(element_, "ll-config-file", propvalue, NULL);
    }

    auto p_ll_lib_file = ll_lib_file.try_get();
    if (p_ll_lib_file && p_ll_lib_file.value() != std::string{""}) {
      gchararray propvalue = (gchararray)p_ll_lib_file.value().c_str();
      g_object_set(element_, "ll-lib-file", propvalue, NULL);
    }

    auto p_tracking_surface_type = tracking_surface_type.try_get();
    if (p_tracking_surface_type && p_tracking_surface_type.value() != 0UL) {
      guint propvalue = (guint)p_tracking_surface_type.value();
      g_object_set(element_, "tracking-surface-type", propvalue, NULL);
    }

    auto p_compute_hw = compute_hw.try_get();
    if (p_compute_hw && p_compute_hw.value() != 0L) {
      gint64 propvalue = (gint64)p_compute_hw.value();
      g_object_set(element_, "compute-hw", propvalue, NULL);
    }

    auto p_display_tracking_id = display_tracking_id.try_get();
    if (p_display_tracking_id && p_display_tracking_id.value() != true) {
      gboolean propvalue = (gboolean)p_display_tracking_id.value();
      g_object_set(element_, "display-tracking-id", propvalue, NULL);
    }

    auto p_tracking_id_reset_mode = tracking_id_reset_mode.try_get();
    if (p_tracking_id_reset_mode && p_tracking_id_reset_mode.value() != 1UL) {
      guint propvalue = (guint)p_tracking_id_reset_mode.value();
      g_object_set(element_, "tracking-id-reset-mode", propvalue, NULL);
    }

    auto p_input_tensor_meta = input_tensor_meta.try_get();
    if (p_input_tensor_meta && p_input_tensor_meta.value() != false) {
      gboolean propvalue = (gboolean)p_input_tensor_meta.value();
      g_object_set(element_, "input-tensor-meta", propvalue, NULL);
    }

    auto p_tensor_meta_gie_id = tensor_meta_gie_id.try_get();
    if (p_tensor_meta_gie_id && p_tensor_meta_gie_id.value() != 0UL) {
      guint propvalue = (guint)p_tensor_meta_gie_id.value();
      g_object_set(element_, "tensor-meta-gie-id", propvalue, NULL);
    }

    auto p_user_meta_pool_size = user_meta_pool_size.try_get();
    if (p_user_meta_pool_size && p_user_meta_pool_size.value() != 32UL) {
      guint propvalue = (guint)p_user_meta_pool_size.value();
      g_object_set(element_, "user-meta-pool-size", propvalue, NULL);
    }

    auto p_sub_batches = sub_batches.try_get();
    if (p_sub_batches && p_sub_batches.value() != std::string{""}) {
      gchararray propvalue = (gchararray)p_sub_batches.value().c_str();
      g_object_set(element_, "sub-batches", propvalue, NULL);
    }

    auto p_sub_batch_err_recovery_trial_cnt =
        sub_batch_err_recovery_trial_cnt.try_get();
    if (p_sub_batch_err_recovery_trial_cnt &&
        p_sub_batch_err_recovery_trial_cnt.value() != 0L) {
      gint propvalue = (gint)p_sub_batch_err_recovery_trial_cnt.value();
      g_object_set(element_, "sub-batch-err-recovery-trial-cnt", propvalue,
                   NULL);
    }

    return GXF_SUCCESS;
  }

  GstElement *get_element_ptr() override { return element_; }

  gxf_result_t registerInterface(nvidia::gxf::Registrar *registrar) override {
    nvidia::gxf::Expected<void> result;
    result &= registrar->parameter(qos, "qos", "QoS",
                                   "Handle Quality-of-Service events", false,
                                   GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(
        tracker_width, "tracker-width", "Tracker Width",
        "Frame width at which the tracker should operate, in pixels", 960UL,
        GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(
        tracker_height, "tracker-height", "Tracker Height",
        "Frame height at which the tracker should operate, in pixels", 544UL,
        GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(gpu_id, "gpu-id", "Set GPU Device ID",
                                   "Set GPU Device ID", 0UL,
                                   GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(
        ll_config_file, "ll-config-file", "Low-level library config file",
        "Low-level library config file path", std::string{""},
        GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(
        ll_lib_file, "ll-lib-file", "Low-level library file path",
        "Low-level library file path", std::string{""},
        GXF_PARAMETER_FLAGS_OPTIONAL);
    result &=
        registrar->parameter(tracking_surface_type, "tracking-surface-type",
                             "Set Tracking Surface Type",
                             "Set Tracking Surface Type, default is ALL, (1) "
                             "=> SPOT Surface, (2) => AISLE Surface",
                             0UL, GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(
        compute_hw, "compute-hw", "compute-hw",
        "Compute Scaling HW\nValid values:\n  0: Default\n  1: GPU\n  2: VIC",
        0L, GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(display_tracking_id, "display-tracking-id",
                                   "Display tracking id in object text",
                                   "Display tracking id in object text", true,
                                   GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(
        tracking_id_reset_mode, "tracking-id-reset-mode",
        "Tracking ID reset mode",
        "Tracking ID reset mode when stream reset or EOS happens", 1UL,
        GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(
        input_tensor_meta, "input-tensor-meta",
        "Use preprocess tensormeta if available for tensor-meta-gie-id",
        "Use preprocess tensormeta if available for tensor-meta-gie-id", false,
        GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(tensor_meta_gie_id, "tensor-meta-gie-id",
                                   "Tensor Meta GIE ID to be used, property "
                                   "valid only if input-tensor-meta is TRUE",
                                   "Tensor Meta GIE ID to be used, property "
                                   "valid only if input-tensor-meta is TRUE",
                                   0UL, GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(user_meta_pool_size, "user-meta-pool-size",
                                   "User Meta Pool Size",
                                   "Tracker user meta buffer pool size", 32UL,
                                   GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(
        sub_batches, "sub-batches",
        "List of sub batces to be processed parallelly",
        "Configuration of sub-batches. Can be specified in 2 ways. Option 1 : "
        "List of sub-batches (seperated by ;). Each sub-batch contains list of "
        "source ids in that batch (seperated by ,) Option 2 : List of "
        "sub-batch sizes seperated by :",
        std::string{""}, GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(
        sub_batch_err_recovery_trial_cnt, "sub-batch-err-recovery-trial-cnt",
        "Number of max. trials to reinitialize the low level tracker library "
        "of a sub-batch in case of a processing failure",
        "Number of max. trials to reinitialize the low level tracker library "
        "of a sub-batch in case of a processing failure. -1 corresponds to "
        "infinite trials.",
        0L, GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(
        src_pad, "video-out", "video-out",
        "Handle to a nvidia::deepstream::NvDsStaticOutput component. Supported "
        "formats - video(I420, NV12, RGBA)",
        gxf::Registrar::NoDefaultParameter(), GXF_PARAMETER_FLAGS_OPTIONAL);
    result &= registrar->parameter(
        sink_pad, "video-in", "video-in",
        "Handle to a nvidia::deepstream::NvDsStaticInput component. Supported "
        "formats - video(I420, NV12, RGBA)",
        gxf::Registrar::NoDefaultParameter(), GXF_PARAMETER_FLAGS_OPTIONAL);
    return nvidia::gxf::ToResultCode(result);
  }

  nvidia::gxf::Parameter<bool> qos;
  nvidia::gxf::Parameter<uint64_t> tracker_width;
  nvidia::gxf::Parameter<uint64_t> tracker_height;
  nvidia::gxf::Parameter<uint64_t> gpu_id;
  nvidia::gxf::Parameter<std::string> ll_config_file;
  nvidia::gxf::Parameter<std::string> ll_lib_file;
  nvidia::gxf::Parameter<uint64_t> tracking_surface_type;
  nvidia::gxf::Parameter<int64_t> compute_hw;
  nvidia::gxf::Parameter<bool> display_tracking_id;
  nvidia::gxf::Parameter<uint64_t> tracking_id_reset_mode;
  nvidia::gxf::Parameter<bool> input_tensor_meta;
  nvidia::gxf::Parameter<uint64_t> tensor_meta_gie_id;
  nvidia::gxf::Parameter<uint64_t> user_meta_pool_size;
  nvidia::gxf::Parameter<std::string> sub_batches;
  nvidia::gxf::Parameter<int64_t> sub_batch_err_recovery_trial_cnt;
  nvidia::gxf::Parameter<nvidia::gxf::Handle<NvDsStaticOutput>> src_pad;
  nvidia::gxf::Parameter<nvidia::gxf::Handle<NvDsStaticInput>> sink_pad;

 protected:
  GstElement *element_;
  const char *GST_ELEMENT_NAME = "nvtracker";
};

#define GXF_EXT_FACTORY_ADD_GstNvTracker()                                  \
  do {                                                                      \
    GXF_EXT_FACTORY_ADD_VERBOSE(0x8db0f9f0ae2138c6UL, 0xb6eefa0d6deeff15UL, \
                                nvidia::deepstream::GstNvTracker,           \
                                nvidia::deepstream::INvDsElement,           \
                                "GstNvTracker placeholder display-name",    \
                                "GstNvTracker placeholder brief",           \
                                "Gstreamer object tracking element");       \
  } while (0)

}  // namespace deepstream
}  // namespace nvidia
