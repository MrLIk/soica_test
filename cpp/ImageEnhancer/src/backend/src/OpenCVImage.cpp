#include "OpenCVImage.h"

namespace soica_test::backend {

OpenCVImage::OpenCVImage(cv::Mat mat)
    : mat(mat)
{
}

} // namespace backend
