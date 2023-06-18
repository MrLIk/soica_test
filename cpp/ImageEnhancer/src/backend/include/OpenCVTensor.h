#pragma once

#include "ITensor.h"

#include <opencv2/core.hpp>

namespace soica_test::backend {

class OpenCVTensor : public ITensor
{
public:
    OpenCVTensor(cv::Mat mat);
    ~OpenCVTensor() = default;

    float* data() override { return reinterpret_cast<float*>(mat.data); }
    const std::vector<std::size_t> shape() const override { return data_shape; }

private:
    cv::Mat mat;
    std::vector<std::size_t> data_shape;
};

} // namespace backend
