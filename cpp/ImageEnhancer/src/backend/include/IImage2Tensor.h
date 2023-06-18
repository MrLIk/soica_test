#pragma once

#include "IImage.h"
#include "ITensor.h"

#include <memory>

namespace soica_test::backend {

class IImage2Tensor
{
public:
    virtual ~IImage2Tensor() = default;

    virtual std::shared_ptr<ITensor> convert(std::shared_ptr<IImage> image) = 0;
};

} // namespace backend
