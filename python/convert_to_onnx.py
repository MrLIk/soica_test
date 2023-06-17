import numpy as np
import onnxruntime

import torch

from HINet.basicsr.models import create_model
from HINet.basicsr.train import parse_options
from HINet.basicsr.utils import img2tensor

def to_numpy(tensor: torch.tensor):
    return tensor.detach().cpu().numpy()

def main():
    opt = parse_options(is_train=False)

    onnx_output_path = opt['convert'].get('onnx_output_path')
    input_w = opt['convert'].get('input_w')
    input_h = opt['convert'].get('input_h')
    input_ch = opt['convert'].get('input_ch')

    model = create_model(opt)
    net_g = model.net_g.eval()

    dummy_input = torch.rand(1, input_ch, input_h, input_w)

    orig_res = net_g(dummy_input)

    torch.onnx.export(net_g,
                      dummy_input,
                      onnx_output_path,
                      input_names=['input'],
                      output_names=['output_1', 'output_2'],
                      dynamic_axes={'input' : {0 : 'batch_size',
                                               2 : 'height',
                                               3 : 'width'},
                                    'output_1' : {0 : 'batch_size',
                                                  2 : 'height',
                                                  3 : 'width'},
                                    'output_2' : {0 : 'batch_size',
                                                  2 : 'height',
                                                  3 : 'width'}})
    
    ort_session = onnxruntime.InferenceSession(onnx_output_path)
    ort_inputs = {ort_session.get_inputs()[0].name: to_numpy(dummy_input)}
    ort_outs = ort_session.run(None, ort_inputs)

    np.testing.assert_allclose(to_numpy(orig_res[0]), ort_outs[0], rtol=1e-03, atol=1e-05)
    np.testing.assert_allclose(to_numpy(orig_res[1]), ort_outs[1], rtol=1e-03, atol=1e-05)

    print('Done!')

if __name__ == '__main__':
    main()
