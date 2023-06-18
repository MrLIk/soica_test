#include "RawRGBImage.h"

namespace soica_test::backend {

RawRGBImage::RawRGBImage(std::unique_ptr<uint8_t[]>&& img_data,
                         std::size_t img_width,
                         std::size_t img_height,
                         std::size_t img_linesize)
    : img_data(std::move(img_data))
    , img_width(img_width)
    , img_height(img_height)
    , img_linesize(img_linesize)
{
}

} // namespace backend
