#pragma once

#include "IImage.h"

#include <memory>

namespace soica_test::backend {

class IImageProcessor
{
public:
    virtual ~IImageProcessor() = default;

    virtual std::shared_ptr<IImage> process(std::shared_ptr<IImage> image) = 0;
};

} // namespace backend
