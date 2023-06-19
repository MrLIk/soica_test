#include "OpenCVTensor.h"

#include <iterator>

#include <opencv2/imgproc.hpp>

namespace soica_test::backend {

OpenCVTensor::OpenCVTensor(cv::Mat mat)
{
    // Check for valid tensor and not an image
    auto mat_size = mat.size;
    if (mat.type() != CV_32F || mat_size.dims() < 4) {
        throw std::runtime_error("OpenCVTensor: Input mat should be float tensor!");
    }

    this->mat = mat;
    data_shape.assign(mat_size.p, std::next(mat_size.p, mat_size.dims()));
}

} // namespace backend
