#pragma once

#include "ITensor.h"

#include <opencv2/core.hpp>

namespace soica_test::backend {

// Return const cv::Mat to show it is should be used as wrapper
inline const cv::Mat tensor_to_cv_mat_blob(std::shared_ptr<ITensor> tensor) {
    const auto& shape = tensor->shape();
    std::vector<int> shape_of_int(shape.size(), 0);
    for (std::size_t i = 0; i < shape.size(); ++i) {
        shape_of_int[i] = int(shape[i]);
    }

    // Have to use const_cast!
    cv::Mat tensor_cv(shape.size(),
                      shape_of_int.data(),
                      CV_32F,
                      reinterpret_cast<void*>(const_cast<float*>(tensor->data())));

    return tensor_cv;
}

} // namesapce backend
