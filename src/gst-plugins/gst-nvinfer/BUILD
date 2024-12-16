
load("@com_extension_dev//build:graph_extension.bzl", "graph_cc_extension")
load("@com_extension_dev//build:registry.bzl", "register_extension")
load("@rules_cuda//cuda:defs.bzl", "cuda_library", "requires_cuda")

exports_files(["LICENSE"])

cc_binary(
    name = "nvdsgst_infer",
    srcs = glob([
        "*.cpp",
        "*.h",
    ]),
    copts=[
      "-fPIC",
      "-Wno-error=deprecated-declarations",
    ],
    linkopts=[
      "-shared",
      "-l:/home/colivier/src/hmdeepstream/deepstream/sources/libs/nvdsinfer/libnvds_infer.so",
    ],
    deps = [
        "@deepstream_lib",
        "@deepstream_apps_common",
        "@NvDsBaseExt",
        "@NvDsInterfaceExt",
        "@StandardExtension",
        "@glib",
        "@gstreamer",
        "@yaml-cpp",
        "@local_cuda//:cuda_runtime",
    ],
    visibility = ["//visibility:public"],
    linkshared = True,
)
