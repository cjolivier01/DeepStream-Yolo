#!/bin/bash
EXP=yolox_s
WEIGHTS=pretrained/deepstream/yolox/test_output_yolox_s_coco300_ch80.pth
python utils/export_yolox.py -w "${WEIGHTS}" -c "${EXP}"
