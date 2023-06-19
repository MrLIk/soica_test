#pragma once

#include <cstdint>

namespace soica_test::backend {

class IImage
{
public:
    virtual ~IImage() = default;

    virtual std::size_t width() const = 0;
    virtual std::size_t height() const = 0;
    virtual std::size_t linesize() const = 0;
    // We want not to allow to change the image
    virtual const uint8_t* data() const = 0;
};

} // namespace backend
