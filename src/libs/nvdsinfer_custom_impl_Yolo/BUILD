load("@rules_cc//cc:defs.bzl", "cc_library")
load("@rules_cuda//cuda:defs.bzl", "cuda_library", "requires_cuda")

cc_binary(
    name = "nvdsinfer_custom_impl_Yolo",
    srcs = glob([
        "*.cpp",
        "layers/*.cpp",
    ]),
    copts = [
        "-Wall",
        "-std=c++11",
        "-fPIC",
        "-Wno-error=deprecated-declarations",
        "-DWITH_CUDA",
    ],
    linkopts = [
        "-shared",
        "-Wl,-z,defs",
        "-Wno-error=deprecated-declarations",
        "-Wl,--start-group",
        "-lnvinfer_plugin",
        "-lnvinfer",
        "-lnvonnxparser",
        "-L/usr/local/cuda-12.6/lib64",
        "-lcudart",
        "-lcublas",
        "-lstdc++fs",
        # Include nvparsers if available
        # "-lnvparsers",
        "-Wl,--end-group",
        # -lnvinfer_plugin -lnvinfer -lnvonnxparser -lcudart -lcublas -lstdc++fs
    ],
    deps=[
        ":nvdsinfer_custom_impl_Yolo_cuda_lib",
        "@deepstream_lib",
        "@deepstream_apps_common",
        "@NvDsBaseExt",
        "@NvDsInterfaceExt",
        "@StandardExtension",
        "@glib",
        "@gstreamer",
        "@opencv_linux//:opencv",
        "@local_cuda//:cuda_runtime",
    ],
    linkshared = True,
    visibility = ["//visibility:public"],
)

# cc_library(
#     name = "nvdsinfer_custom_impl_Yolo_lib",
#     # srcs = glob([
#     #     "*.cpp",
#     #     "layers/*.cpp",
#     # ]),
#     # Define the compilation flags similar to CFLAGS/CUFLAGS
#     # Adjust these paths, flags, and CUDA version as needed.
#     copts = [
#         "-Wall",
#         "-std=c++11",
#         "-fPIC",
#         "-Wno-error=deprecated-declarations",
#         "-DWITH_CUDA",
#     ],
#     # Add libraries. Adjust the library search paths as needed.
#     # If OPENCV=1, append `-lopencv_core -lopencv_highgui` etc. as needed.
#     deps=[
#         "@deepstream_lib",
#         "@deepstream_apps_common",
#         "@NvDsBaseExt",
#         "@NvDsInterfaceExt",
#         "@StandardExtension",
#         "@glib",
#         "@gstreamer",
#         "@opencv_linux//:opencv",
#         "@local_cuda//:cuda_runtime",
#     ],
#     # Make sure the library is visible to where it’s needed.
#     visibility = ["//visibility:public"],
# )

cuda_library(
    name = "nvdsinfer_custom_impl_Yolo_cuda_lib",
    srcs=glob([
        "*.cu",
    ]),
    copts=[
        "-std=c++11",
        "-Xcompiler=-fPIC",
        "-DWITH_CUDA",
    ],
    linkopts=[
      "-shared",
    ],
    deps = [
        ":nvdsinfer_custom_impl_Yolo_headers",
        "@deepstream_lib",
        "@local_cuda//:cuda_runtime",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "nvdsinfer_custom_impl_Yolo_headers",
    hdrs = glob([
        "*.h",
        "layers/*.h",
    ]),
    # deps=[
    #   "@local_cuda//:cuda_runtime",
    # ],
    visibility = ["//visibility:public"],
)
