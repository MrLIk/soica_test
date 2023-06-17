import argparse
import cv2

import numpy as np


def parse_args():
    parser = argparse.ArgumentParser(description='ONNX inferencer')
    parser.add_argument('--first_image_path',
                        type=str,
                        required=True)
    parser.add_argument('--second_image_path',
                        type=str,
                        required=True)
    args = parser.parse_args()
    return args


def main():
    args = parse_args()

    img_1 = cv2.imread(args.first_image_path)
    img_2 = cv2.imread(args.second_image_path)

    if (img_1.shape != img_2.shape):
        raise RuntimeError('Sizes of images does not match!')

    avg_diff = np.sum(np.abs(img_1.astype(np.float32) - img_2.astype(np.float32)), axis=(0, 1))
    avg_diff /= img_1.shape[0] * img_1.shape[1]
    print(f'Average channel pixel diff is: {avg_diff}')

if __name__ == '__main__':
    main()