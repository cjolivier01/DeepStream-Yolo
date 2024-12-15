load("@rules_cc//cc:defs.bzl", "cc_library")


# copts = select({
#     "//conditions:default": [
#         "-Wall",
#         "-std=c++11",
#         ...
#     ],
#     "//my:opencv_enabled": [
#         "-DOPENCV",
#         # additional opencv includes...
#     ],
# })

cc_library(
    name = "nvdsinfer_custom_impl_Yolo",
    srcs = glob([
        "*.cpp",
        "layers/*.cpp",
        "*.cu",
    ]),
    hdrs = glob([
        "*.h",
        "layers/*.h",
    ]),
    # Define the compilation flags similar to CFLAGS/CUFLAGS
    # Adjust these paths, flags, and CUDA version as needed.
    copts = [
        "-Wall",
        "-std=c++11",
        "-fPIC",
        "-Wno-error=deprecated-declarations",
        "-I/usr/include/gstreamer-1.0",
        "-I/usr/include/glib-2.0",
        "-I/usr/lib/x86_64-linux-gnu/glib-2.0/include",
        "-I/opt/nvidia/deepstream/deepstream/sources/includes",
        "-I/usr/local/cuda-12.6/include",
        # If using OpenCV (uncomment if OPENCV=1 was intended)
        # "-DOPENCV",
        # Add OpenCV include paths if needed:
        # e.g. "-I/usr/include/opencv4",
        # If using GRAPH
        # "-DGRAPH"
    ],
    # Add libraries. Adjust the library search paths as needed.
    # If OPENCV=1, append `-lopencv_core -lopencv_highgui` etc. as needed.
    linkopts = [
        "-shared",
        "-Wl,--start-group",
        "-lnvinfer_plugin",
        "-lnvinfer",
        "-lnvonnxparser",
        "-L/usr/local/cuda-12.6/lib64",
        "-lcudart",
        "-lcublas",
        "-lstdc++fs",
        # Include nvparsers if available
        "-lnvparsers",
        "-Wl,--end-group",
    ],
    # Make sure the library is visible to where it’s needed.
    visibility = ["//visibility:public"],
    linkshared = True,
)
