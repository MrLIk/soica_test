#pragma ocne

#include "IImage.h"

#include <opencv2/core.hpp>

namespace soica_test::backend {

class OpenCVImage : public IImage
{
public:
    OpenCVImage(cv::Mat mat);
    ~OpenCVImage() override = default;

    std::size_t width() const override { return mat.cols; }
    std::size_t height() const override { return mat.rows; }
    std::size_t linesize() const override { return mat.step; }
    const uint8_t* data() const override { return mat.data; }

private:
    cv::Mat mat;
};

} // namespace backend
