# Bazel is only available for amd64 and arm64.

cc_library(
  name = "deepstream_lib",
  hdrs = glob([
      "sources/includes/**/*.h*",
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
  ],
  visibility = ["//visibility:public"],
)
