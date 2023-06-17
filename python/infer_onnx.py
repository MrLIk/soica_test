import argparse
import cv2
import onnxruntime

import numpy as np


def parse_args():
    parser = argparse.ArgumentParser(description='ONNX inferencer')
    parser.add_argument('--onnx_path',
                        help='Path to the onnx model',
                        type=str,
                        required=True)
    parser.add_argument('--input_image_path',
                        help='Input image path',
                        type=str,
                        required=True)
    parser.add_argument('--output_image_path',
                        help='Output image path',
                        type=str,
                        required=True)
    args = parser.parse_args()
    return args
    

def img2tensor(img_bgr: np.array) -> np.array:
    img_rgb = cv2.cvtColor(img_bgr, cv2.COLOR_BGR2RGB)
    img_tensor = img_rgb.transpose(2, 0, 1).astype(np.float32) / 255.0
    img_tensor = np.expand_dims(img_tensor, axis=0)
    return img_tensor


def tensor2img(tensor: np.array) -> np.array:
    res_tensor = np.clip(tensor.squeeze(0), 0, 1)
    res_img = (res_tensor.transpose(1, 2, 0) * 255.0).round().astype(np.uint8)
    res_img = cv2.cvtColor(res_img, cv2.COLOR_RGB2BGR)
    return res_img


def main():
    args = parse_args()

    ort_session = onnxruntime.InferenceSession(args.onnx_path)

    img = cv2.imread(args.input_image_path)
    img_tensor = img2tensor(img)

    ort_inputs = {ort_session.get_inputs()[0].name: img_tensor}
    ort_outs = ort_session.run(None, ort_inputs)

    res_img = tensor2img(ort_outs[1])

    cv2.imwrite(args.output_image_path, res_img)

    print('Done!')

if __name__ == '__main__':
    main()