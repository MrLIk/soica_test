#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "ITensor.h"

namespace soica_test::backend {

class IInferencer
{
public:
    virtual ~IInferencer() = default;

    // <OUTPUT_LAYRS_NAME, CORRESPONDING_TENSOR>
    virtual std::unordered_map<std::string, std::shared_ptr<ITensor>> forward(std::shared_ptr<ITensor> tensor) = 0;
};

} // namespace backend
