/*
 * SPDX-FileCopyrightText: Copyright (c) 2020-2021 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
 * SPDX-License-Identifier: LicenseRef-NvidiaProprietary
 *
 * NVIDIA CORPORATION, its affiliates and licensors retain all intellectual
 * property and proprietary rights in and to this material, related
 * documentation and any modifications thereto. Any use, reproduction,
 * disclosure or distribution of this material and related documentation
 * without an express license agreement from NVIDIA CORPORATION or
 * its affiliates is strictly prohibited.
 */

#ifndef __GST_DSEXAMPLE_H__
#define __GST_DSEXAMPLE_H__

#include <gst/base/gstbasetransform.h>
#include <gst/video/video.h>

/* Open CV headers */
#ifdef WITH_OPENCV
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#endif

#include <cuda.h>
#include <cuda_runtime.h>
#include "nvbufsurface.h"
#include "nvbufsurftransform.h"
#include "gst-nvquery.h"
#include "gstnvdsmeta.h"
#include "dsexample_lib/dsexample_lib.h"
#include "nvtx3/nvToolsExt.h"

#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

/* Package and library details required for plugin_init */
#define PACKAGE "dsexample"
#define VERSION "1.0"
#define LICENSE "Proprietary"
#define DESCRIPTION "NVIDIA example plugin for integration with DeepStream on DGPU/Jetson"
#define BINARY_PACKAGE "NVIDIA DeepStream 3rdparty IP integration example plugin"
#define URL "http://nvidia.com/"


G_BEGIN_DECLS
/* Standard boilerplate stuff */
typedef struct _GstDsExample GstDsExample;
typedef struct _GstDsExampleClass GstDsExampleClass;

/* Standard boilerplate stuff */
#define GST_TYPE_DSEXAMPLE (gst_dsexample_get_type())
#define GST_DSEXAMPLE(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_DSEXAMPLE,GstDsExample))
#define GST_DSEXAMPLE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_DSEXAMPLE,GstDsExampleClass))
#define GST_DSEXAMPLE_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), GST_TYPE_DSEXAMPLE, GstDsExampleClass))
#define GST_IS_DSEXAMPLE(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_DSEXAMPLE))
#define GST_IS_DSEXAMPLE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_DSEXAMPLE))
#define GST_DSEXAMPLE_CAST(obj)  ((GstDsExample *)(obj))

/** Maximum batch size to be supported by dsexample. */
#define NVDSEXAMPLE_MAX_BATCH_SIZE 1024

struct _GstDsExample
{
  GstBaseTransform base_trans;

  /** Context of the custom algorithm library */
  DsExampleCtx *dsexamplelib_ctx;

  /** Processing Queue and related synchronization structures. */

  /** Gmutex lock for against shared access in threads**/
  GMutex process_lock;

  /** Queue to send data to output thread for processing**/
  GQueue *process_queue;

  /** Gcondition for process queue**/
  GCond process_cond;

  /**Queue to receive processed data from output thread **/
  GQueue *buf_queue;

  /** Gcondition for buf queue **/
  GCond buf_cond;

  /** Output thread. */
  GThread *process_thread;

  /** Boolean to signal output thread to stop. */
  gboolean stop;

  /** Unique ID of the element. Used to identify metadata
   *  generated by this element. */
  guint unique_id;

  /** Frame number of the current input buffer */
  guint64 frame_num;

  /** CUDA Stream used for allocating the CUDA task */
  cudaStream_t cuda_stream;

  /** Temporary NvBufSurface for batched transformations. */
  NvBufSurface batch_insurf;

  /** the intermediate scratch buffer for conversions RGBA */
  NvBufSurface *inter_buf;

  /** Input video info (resolution, color format, framerate, etc) */
  GstVideoInfo video_info;

  /** Resolution at which frames/objects should be processed */
  gint processing_width;
  gint processing_height;

  /** Maximum batch size. */
  guint max_batch_size;

  /** GPU ID on which we expect to execute the task */
  guint gpu_id;

  /** Boolean indicating if entire frame or cropped objects should be processed */
  gboolean process_full_frame;

  /** Current batch number of the input batch. */
  gulong current_batch_num;

  /** GstFlowReturn returned by the latest buffer pad push. */
  GstFlowReturn last_flow_ret;

  /** Config params required by NvBufSurfTransform API. */
  NvBufSurfTransformConfigParams transform_config_params;

  /** Parameters to use for transforming buffers. */
  NvBufSurfTransformParams transform_params;

  /** NVTX Domain. */
  nvtxDomainHandle_t nvtx_domain;
};

typedef struct
{
  /** Ratio by which the frame / object crop was scaled in the horizontal
   * direction. Required when scaling co-ordinates/sizes in metadata
   * back to input resolution. */
  gdouble scale_ratio_x = 0.0;
  /** Ratio by which the frame / object crop was scaled in the vertical
   * direction. Required when scaling co-ordinates/sizes in metadata
   * back to input resolution. */
  gdouble scale_ratio_y = 0.0;
  /** NvDsObjectParams belonging to the object to be classified. */
  NvDsObjectMeta *obj_meta = nullptr;
  NvDsFrameMeta *frame_meta = nullptr;
  /** Index of the frame in the batched input GstBuffer. Not required for
   * classifiers. */
  guint batch_index = 0;
  /** Frame number of the frame from the source. */
  gulong frame_num = 0;
  /** The buffer structure the object / frame was converted from. */
  NvBufSurfaceParams *input_surf_params = nullptr;
} GstDsExampleFrame;

/**
 * Holds information about the batch of frames to be inferred.
 */
typedef struct
{
  /** Vector of frames in the batch. */
  std::vector < GstDsExampleFrame > frames;
  /** Pointer to the input GstBuffer. */
  GstBuffer *inbuf = nullptr;
  /** Batch number of the input batch. */
  gulong inbuf_batch_num = 0;
  /** Boolean indicating that the output thread should only push the buffer to
   * downstream element. If set to true, a corresponding batch has not been
   * queued at the input of NvDsExampleContext and hence dequeuing of output is
   * not required. */
  gboolean push_buffer = FALSE;
  /** Boolean marking this batch as an event marker. This is only used for
   * synchronization. The output loop does not process on the batch.
   */
  gboolean event_marker = FALSE;

#ifdef WITH_OPENCV
  /** OpenCV mat containing RGB data */
  cv::Mat * cvmat;
#else
  NvBufSurface *inter_buf;
#endif

  nvtxRangeId_t nvtx_complete_buf_range = 0;
} GstDsExampleBatch;

/** Boiler plate stuff */
struct _GstDsExampleClass
{
  GstBaseTransformClass parent_class;
};

GType gst_dsexample_get_type (void);

G_END_DECLS
#endif /* __GST_DSEXAMPLE_H__ */
