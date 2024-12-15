# Bazel is only available for amd64 and arm64.

cc_library(
  name = "deepstream_apps_common",
  srcs=[
    "src/deepstream_preprocess.c",
    "src/deepstream_secondary_gie_bin.c",
    "src/deepstream_dsanalytics.c",
    "src/deepstream_source_bin.c",
    "src/deepstream_dewarper_bin.c",
    "src/deepstream_sink_bin.c",
    "src/deepstream_tracker_bin.c",
    "src/deepstream_common.c",
    "src/deepstream_tiled_display_bin.c",
    "src/deepstream_primary_gie_bin.c",
    "src/deepstream_streammux.c",
    "src/deepstream_dsexample.c",
    "src/deepstream_osd_bin.c",
    "src/deepstream_audio_classifier_bin.c",
    # "src/deepstream_c2d_msg_util.c",
    "src/deepstream_segvisual_bin.c",
    "src/deepstream_perf.c",
    "src/deepstream_config_file_parser.c",
    "src/deepstream_secondary_preprocess.c",
    "src/deepstream_c2d_msg.c",
    "src/deepstream-yaml/deepstream_tracker_yaml.cpp",
    "src/deepstream-yaml/deepstream_source_yaml.cpp",
    "src/deepstream-yaml/deepstream_streammux_yaml.cpp",
    "src/deepstream-yaml/deepstream_segvisual_yaml.cpp",
    "src/deepstream-yaml/deepstream_dewarper_yaml.cpp",
    "src/deepstream-yaml/deepstream_preprocess_yaml.cpp",
    "src/deepstream-yaml/deepstream_tiled_display_yaml.cpp",
    "src/deepstream-yaml/deepstream_config_yaml.cpp",
    "src/deepstream-yaml/deepstream_gie_yaml.cpp",
    "src/deepstream-yaml/deepstream_dsanalytics_yaml.cpp",
    "src/deepstream-yaml/deepstream_osd_yaml.cpp",
    "src/deepstream-yaml/deepstream_sink_yaml.cpp",
    "src/deepstream-yaml/deepstream_msgconsumer_yaml.cpp",
    "src/deepstream-yaml/deepstream_msgconv_yaml.cpp",
    "src/deepstream-yaml/deepstream_image_save_yaml.cpp",
    "src/deepstream-yaml/deepstream_dsexample_yaml.cpp",    
  ],
  copts=[
    "-Wno-error=write-strings",
  ],
  hdrs = [
    "includes/deepstream_gie.h",
    "includes/deepstream_c2d_msg.h",
    "includes/deepstream_config_file_parser.h",
    "includes/deepstream_secondary_preprocess.h",
    "includes/deepstream_primary_gie.h",
    "includes/deepstream_dewarper.h",
    "includes/deepstream_c2d_msg_util.h",
    "includes/deepstream_common.h",
    "includes/deepstream_config.h",
    "includes/deepstream_dsanalytics.h",
    "includes/deepstream_secondary_gie.h",
    "includes/deepstream_audio_classifier.h",
    "includes/deepstream_sinks.h",
    "includes/deepstream_image_save.h",
    "includes/deepstream_dsexample.h",
    "includes/deepstream_app_version.h",
    "includes/deepstream_streammux.h",
    "includes/deepstream_tracker.h",
    "includes/deepstream_segvisual.h",
    "includes/deepstream_config_yaml.h",
    "includes/deepstream_tiled_display.h",
    "includes/deepstream_sources.h",
    "includes/deepstream_preprocess.h",
    "includes/deepstream_perf.h",
    "includes/deepstream_osd.h",

  ],
  includes = [
      "includes",
  ],
  # copts=[
  #   "-Iincludes",
  # ],
  linkopts = [
    # "-L/opt/nvidia/deepstream/deepstream-7.1/lib",
    # "-l:libnvdsgst_meta.so",
    # "-l:libnvbufsurface.so",
    # "-l:libnvdsgst_inferbase.so",
    # "-l:libnvdsgst_helper.so",
    # "-l:libnvds_meta.so", 
    # "-l:libnvbufsurftransform.so",
    # "-l:libnvdsgst_customhelper.so",
    # "-l:libnvds_nvtxhelper.so",
  ],
  deps=[
    "@deepstream_lib",
    "@glib",
    "@gstreamer",
    "@local_cuda//:cuda_runtime",
  ],
  visibility = ["//visibility:public"],
)
