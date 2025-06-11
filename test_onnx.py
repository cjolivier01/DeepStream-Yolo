#!/usr/bin/env python3
"""
test_onnx_yolox.py

A simple ONNX Runtime test for a YOLOX model:
  1. Preprocess input image (letterbox + normalize)
  2. Run ONNX Runtime inference
  3. Decode YOLOX outputs (cx, cy, w, h, obj_conf, class_probs)
  4. Filter by confidence, apply NMS
  5. Draw boxes on the image and save/display
"""

import argparse
import cv2
import numpy as np
import onnxruntime as ort


def letterbox(img, new_shape=(640, 640), color=(114, 114, 114)):
    """Resize and pad image to new_shape, return padded image, scale, pad."""
    h0, w0 = img.shape[:2]
    r = min(new_shape[0] / h0, new_shape[1] / w0)
    nh, nw = int(round(h0 * r)), int(round(w0 * r))
    img_resized = cv2.resize(img, (nw, nh), interpolation=cv2.INTER_LINEAR)
    pad = ((new_shape[0] - nh) // 2, (new_shape[1] - nw) // 2)
    canvas = np.full((new_shape[0], new_shape[1], 3), color, dtype=np.uint8)
    canvas[pad[0] : pad[0] + nh, pad[1] : pad[1] + nw, :] = img_resized
    return canvas, r, pad


def preprocess(img_path, input_size):
    img = cv2.imread(img_path)
    assert img is not None, f"Image not found: {img_path}"
    img, scale, pad = letterbox(img, new_shape=input_size)
    img = img.astype(np.float32) / 255.0
    # HWC → CHW, add batch
    img = img.transpose(2, 0, 1)[None, ...]
    return img, scale, pad, cv2.imread(img_path)


def xywh2xyxy(x):
    # Convert [x_center, y_center, w, h] to [x1,y1,x2,y2]
    y = np.zeros_like(x)
    y[:, 0] = x[:, 0] - x[:, 2] / 2
    y[:, 1] = x[:, 1] - x[:, 3] / 2
    y[:, 2] = x[:, 0] + x[:, 2] / 2
    y[:, 3] = x[:, 1] + x[:, 3] / 2
    return y


def nms(boxes, scores, iou_thresh):
    """Pure-Python NMS."""
    x1, y1, x2, y2 = boxes[:, 0], boxes[:, 1], boxes[:, 2], boxes[:, 3]
    areas = (x2 - x1) * (y2 - y1)
    order = scores.argsort()[::-1]
    keep = []
    while order.size > 0:
        i = order[0]
        keep.append(i)
        xx1 = np.maximum(x1[i], x1[order[1:]])
        yy1 = np.maximum(y1[i], y1[order[1:]])
        xx2 = np.minimum(x2[i], x2[order[1:]])
        yy2 = np.minimum(y2[i], y2[order[1:]])
        w = np.maximum(0.0, xx2 - xx1)
        h = np.maximum(0.0, yy2 - yy1)
        inter = w * h
        ovr = inter / (areas[i] + areas[order[1:]] - inter)
        inds = np.where(ovr <= iou_thresh)[0]
        order = order[inds + 1]
    return keep


def postprocess(
    pred, scale, pad, orig_img_shape, conf_thres=0.3, iou_thres=0.45, num_classes=80
):
    """
    pred: (N, 85) array = [xc, yc, w, h, obj_conf, cls1, cls2, ...]
    """
    # split
    xywh = pred[:, :4]
    obj_conf = pred[:, 4]
    cls_conf = pred[:, 5:]
    cls_ids = np.argmax(cls_conf, axis=1)
    cls_scores = cls_conf[np.arange(len(pred)), cls_ids]
    scores = obj_conf * cls_scores

    # filter
    mask = scores > conf_thres
    xywh, scores, cls_ids = xywh[mask], scores[mask], cls_ids[mask]
    if xywh.shape[0] == 0:
        return []

    # to xyxy
    boxes = xywh2xyxy(xywh)

    # NMS
    keep = nms(boxes, scores, iou_thres)
    results = []
    for i in keep:
        x1, y1, x2, y2 = boxes[i]
        # undo padding & scale
        x1 = (x1 - pad[1]) / scale
        y1 = (y1 - pad[0]) / scale
        x2 = (x2 - pad[1]) / scale
        y2 = (y2 - pad[0]) / scale
        results.append(
            (int(x1), int(y1), int(x2), int(y2), float(scores[i]), int(cls_ids[i]))
        )
    return results


def draw(img, detections, class_names=None):
    for x1, y1, x2, y2, conf, cls_id in detections:
        cv2.rectangle(img, (x1, y1), (x2, y2), (0, 255, 0), 2)
        label = f"{cls_id}:{conf:.2f}"
        cv2.putText(
            img, label, (x1, y1 - 5), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 1
        )
    return img


def main(args):
    # 1. Create session
    sess = ort.InferenceSession(args.onnx_model, providers=["CPUExecutionProvider"])
    input_name = sess.get_inputs()[0].name
    # 2. Prepare input
    img, scale, pad, orig = preprocess(args.image, args.input_size)
    # 3. Inference
    outputs = sess.run(None, {input_name: img})
    pred = outputs[0][0]  # assume shape (1, N, 5+num_classes)
    # 4. Postprocess
    dets = postprocess(
        pred,
        scale,
        pad,
        orig.shape,
        conf_thres=args.conf_thres,
        iou_thres=args.iou_thres,
        num_classes=args.num_classes,
    )
    # 5. Draw & save/show
    out = draw(orig, dets)
    cv2.imwrite(args.out_path, out)
    print(f"Detections: {dets}")
    print(f"Result saved to {args.out_path}")


if __name__ == "__main__":
    p = argparse.ArgumentParser()
    p.add_argument("--onnx-model", required=True, help="path to YOLOX ONNX file")
    p.add_argument("--image", required=True, help="path to test image")
    p.add_argument("--out-path", default="out.jpg", help="where to save result")
    p.add_argument(
        "--input-size", type=int, nargs=2, default=[640, 640], help="model input (H W)"
    )
    p.add_argument("--conf-thres", type=float, default=0.3)
    p.add_argument("--iou-thres", type=float, default=0.45)
    p.add_argument("--num-classes", type=int, default=80)
    args = p.parse_args()
    main(args)
