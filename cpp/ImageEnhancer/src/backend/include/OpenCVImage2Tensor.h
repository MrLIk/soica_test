#pragma once

#include "IImage2Tensor.h"

namespace soica_test::backend {

class OpenCVImage2Tensor : public IImage2Tensor
{
public:
    OpenCVImage2Tensor(float normalize_value);
    ~OpenCVImage2Tensor() override = default;

    std::shared_ptr<ITensor> convert(std::shared_ptr<IImage> image) override;

private:
    // Image values multiplier
    float normalize_value;

};

} // namespace backend
