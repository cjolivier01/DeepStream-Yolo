#!/bin/bash
set -e

EXP=yolox_s
WEIGHTS=pretrained/deepstream/yolox/test_output_yolox_s_coco300_ch80.pth
ONNX_FILE="hm_output_yolox_s_coco300_ch80.pth.onnx"

python utils/export_yolox.py -w "${WEIGHTS}" -c "${EXP}" -o "${ONNX_FILE}" --simplify
python utils/export_yolox_rknn.py $@
