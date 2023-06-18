#include "OpenCVTensor2Image.h"

#include "OpenCVImage.h"
#include "utils.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/dnn.hpp>

namespace soica_test::backend {

OpenCVTensor2Image::OpenCVTensor2Image(float normalize_to_min,
                                       float normalize_to_max,
                                       float denormalize_value)
    : normalize_to_min(normalize_to_min)
    , normalize_to_max(normalize_to_max)
    , denormalize_value(denormalize_value)
{

}

std::shared_ptr<IImage> OpenCVTensor2Image::convert(std::shared_ptr<ITensor> tensor)
{
    auto blob = tensor_to_cv_mat_blob(tensor);

    // Normalize float values
    cv::normalize(blob,
                  blob,
                  normalize_to_min,
                  normalize_to_max,
                  cv::NORM_MINMAX);

    std::vector<cv::Mat> images_cv;
    cv::dnn::imagesFromBlob(blob, images_cv);

    // As we know that we expect only one image
    auto image_cv = images_cv[0];
    image_cv.convertTo(image_cv, CV_8UC3, denormalize_value);

    auto image = std::make_shared<OpenCVImage>(image_cv);

    return image;
}

} // namespace backend
