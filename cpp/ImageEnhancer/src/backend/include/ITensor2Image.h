#pragma once

#include "IImage.h"
#include "ITensor.h"

#include <memory>

namespace soica_test::backend {

class ITensor2Image
{
public:
    virtual ~ITensor2Image() = default;

    virtual std::shared_ptr<IImage> convert(std::shared_ptr<ITensor> tensor) = 0;
};

} // namespace backend
