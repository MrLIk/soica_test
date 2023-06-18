#pragma once

#include "ITensor2Image.h"

namespace soica_test::backend {

class OpenCVTensor2Image : public ITensor2Image
{
public:
    OpenCVTensor2Image(float normalize_to_min,
                       float normalize_to_max,
                       float denormalize_value);
    ~OpenCVTensor2Image() override = default;

    std::shared_ptr<IImage> convert(std::shared_ptr<ITensor> tensor) override;

private:
    // this will be the min value of the tensor
    float normalize_to_min;
    // this will be the max value of the tensor
    float normalize_to_max;
    // the result image multiplier
    float denormalize_value;
};

} // namespace backend
