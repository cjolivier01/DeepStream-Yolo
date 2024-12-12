#include <gst/gst.h>
#include <gst/base/gstbasetransform.h>
#include <gst/gstplugin.h>

#include <NvInfer.h>

// G_BEGIN_DECLS

// #define GST_TYPE_CUSTOM_TENSORRT_PLUGIN (custom_tensorrt_plugin_get_type())
// G_DECLARE_FINAL_TYPE(CustomTensorRTPlugin, custom_tensorrt_plugin, CUSTOM, TENSORRT_PLUGIN, GstBaseTransform)

// struct _CustomTensorRTPlugin {
//     GstBaseTransform parent;
//     // Add TensorRT plugin members
//     nvinfer1::IPluginV2DynamicExt* trt_plugin;
// };

// struct _CustomTensorRTPluginClass {
//     GstBaseTransformClass parent_class;
// };

// G_END_DECLS

// G_DEFINE_TYPE(CustomTensorRTPlugin, custom_tensorrt_plugin, GST_TYPE_BASE_TRANSFORM)

// static gboolean custom_tensorrt_plugin_init(GstPlugin* plugin) {
//     return gst_element_register(plugin, "deepstream_yolo", GST_RANK_NONE, GST_TYPE_CUSTOM_TENSORRT_PLUGIN);
// }

// GST_PLUGIN_DEFINE(
//     GST_VERSION_MAJOR,
//     GST_VERSION_MINOR,
//     custom_tensorrt,
//     "DeepStream-Yolo Plugin",
//     custom_tensorrt_plugin_init,
//     "1.0",
//     "LGPL",
//     "DeepStream-Yolo",
//     "http://your.plugin.url"
// )



// GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
//     GST_VERSION_MINOR,
//     auparse,
//     "parses au streams", plugin_init, VERSION, "LGPL", GST_PACKAGE_NAME,
//     GST_PACKAGE_ORIGIN)
