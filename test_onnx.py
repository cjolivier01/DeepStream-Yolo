from rknnlite.api import RKNNLite
import numpy as np
from PIL import Image

# 1. Load the RKNN model
rknn = RKNNLite()
rknn.load_rknn("hm_output_yolox_s_coco300_ch80.pth.onnx.rknn")

# 2. Initialize the runtime (connects to NPU)
rknn.init_runtime()

# 3. Prepare input data (resize/normalize per your model requirements)
img = Image.open("test.jpg").resize((224, 224))
data = np.array(img, dtype=np.float32)
data = np.transpose(data, (2, 0, 1))  # if model expects NCHW
data = data[np.newaxis, ...]  # add batch dim

# 4. Run inference
outputs = rknn.inference(inputs=[data])

# 5. Process outputs
print(outputs)

# 6. Clean up
rknn.release()
