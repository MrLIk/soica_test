#pragma once

#include "IImage.h"

#include <memory>

namespace soica_test::backend {

class RawRGBImage : public IImage
{
public:
    RawRGBImage(std::unique_ptr<uint8_t[]>&& img_data,
                std::size_t img_width,
                std::size_t img_height,
                std::size_t img_linesize);
    ~RawRGBImage() override = default;

    std::size_t width() const override { return img_width; }
    std::size_t height() const override { return img_height; }
    std::size_t linesize() const override { return img_linesize; }
    const uint8_t* data() const override { return img_data.get(); }

private:
    std::unique_ptr<uint8_t[]> img_data;
    std::size_t img_width;
    std::size_t img_height;
    std::size_t img_linesize;
};

} // namespace
