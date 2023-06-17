import numpy as np
import cv2

from HINet.basicsr.models import create_model
from HINet.basicsr.train import parse_options
from HINet.basicsr.utils import img2tensor

def main():
    opt = parse_options(is_train=False)

    img_path = opt['img_path'].get('input_img')
    output_path = opt['img_path'].get('output_img')

    img = cv2.imread(img_path).astype(np.float32) / 255.0
    img = img2tensor(img, bgr2rgb=True, float32=True)

    model = create_model(opt)
    model.single_image_inference(img, output_path)

    print('inference {} .. finished.'.format(img_path))

if __name__ == '__main__':
    main()
