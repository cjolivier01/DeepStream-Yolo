#!/bin/bash
EXP=yolox_s
python utils/export_yolox.py -w pretrained/deepstream/yolox/test_output_yolox_s_coco300_ch80.pth -c "${EXP}" --simplify
