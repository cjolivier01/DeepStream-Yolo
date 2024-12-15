# Bazel is only available for amd64 and arm64.

cc_library(
  name = "deepstream_apps_common",
  srcs=[
      "sources/apps/apps-common/src/*.c",
      "sources/apps/apps-common/src/deepstream-yaml/*.c",
  ],
  hdrs = glob([
      "sources/apps/apps-common/includes/**/*.h*",
  ]),
  includes = [
      "sources/includes",
  ],
  # copts=[
  #   "-Isources/includes",
  # ],
  linkopts = [
    "-L/opt/nvidia/deepstream/deepstream-7.1/lib",
    "-l:libnvdsgst_meta.so",
    "-l:libnvbufsurface.so",
    "-l:libnvdsgst_inferbase.so",
    "-l:libnvdsgst_helper.so",
    "-l:libnvds_meta.so", 
    "-l:libnvbufsurftransform.so",
    "-l:libnvdsgst_customhelper.so",
    "-l:libnvds_nvtxhelper.so",
  ],
  deps=[
    "@deepstream_lib",
  ],
  visibility = ["//visibility:public"],
)
