from rknn.api import RKNN

# 1. Create RKNN object
rknn = RKNN()

# 2. (Optional) Specify the target platform
rknn.config(target_platform="rk3588")

# 3. Load the ONNX model
rknn.load_onnx(model="hm_output_yolox_s_coco300_ch80.pth.onnx")

# 4. Build the RKNN model (with or without quantization)
#    For higher performance, supply a small representative dataset for INT8 quantization:
# rknn.build(do_quantization=True, dataset=['img1.jpg','img2.jpg',...])
rknn.build(do_quantization=False)

# 5. Export the compiled RKNN model
rknn.export_rknn("hm_output_yolox_s_coco300_ch80.pth.onnx.rknn")

# 6. Release resources
rknn.release()
