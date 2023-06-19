#include "OpenCVImage2Tensor.h"

#include "OpenCVTensor.h"

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/highgui.hpp>

namespace soica_test::backend {

OpenCVImage2Tensor::OpenCVImage2Tensor(float normalize_value)
    : normalize_value(normalize_value)
{

}

std::shared_ptr<ITensor> OpenCVImage2Tensor::convert(std::shared_ptr<IImage> image)
{
    // Have to use const_cast!
    // We are sure that we will not change the data in this case
    cv::Mat image_cv(image->height(),
                     image->width(),
                     CV_8UC3,
                     const_cast<uchar*>(image->data()),
                     image->linesize());

    // Simply transpose image and add new dimension for batch
    // HWC -> NCHW
    auto blob = cv::dnn::blobFromImage(image_cv);
    blob *= normalize_value;
    auto tensor = std::make_shared<OpenCVTensor>(blob);

    return tensor;
}

} // namespace backend
