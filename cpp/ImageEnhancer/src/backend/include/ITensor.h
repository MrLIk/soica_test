#pragma once

#include <vector>

namespace soica_test::backend {

class ITensor
{
public:
    virtual ~ITensor() = default;

    // We do not want to allow to change the data
    virtual const float* data() = 0;
    virtual const std::vector<std::size_t> shape() const = 0;
};

} // namespace backend
