#pragma once

#include "ITensor.h"

#include <opencv2/core.hpp>

namespace soica_test::backend {

inline cv::Mat tensor_to_cv_mat_blob(std::shared_ptr<ITensor> tensor) {
    const auto& shape = tensor->shape();
    std::vector<int> shape_of_int(shape.size(), 0);
    for (std::size_t i = 0; i < shape.size(); ++i) {
        shape_of_int[i] = int(shape[i]);
    }
    cv::Mat tensor_cv(shape.size(),
                      shape_of_int.data(),
                      CV_32F,
                      reinterpret_cast<void*>(tensor->data()));

    return tensor_cv;
}

} // namesapce backend
