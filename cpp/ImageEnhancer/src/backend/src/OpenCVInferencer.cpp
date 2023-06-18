#include "OpenCVInferencer.h"

#include "OpenCVTensor.h"
#include "utils.h"

#include <iostream>
#include <opencv2/highgui.hpp>

namespace soica_test::backend {

OpenCVInferencer::OpenCVInferencer(const std::filesystem::path& net_path,
                                   BackendType backend_type)
{
    if (net_path.extension() != ".onnx") {
        throw std::runtime_error("OpenCVInferencer can use only onnx models currently!");
    }

    net = cv::dnn::readNetFromONNX(net_path);

    // OpenCV have several backends and targets
    // Choose the desirede one
    auto [backend_cv, targer_cv] = backend_map.at(backend_type);
    net.setPreferableBackend(backend_cv);
    net.setPreferableTarget(targer_cv);

    // TODO
    // It returns only one layer name, the second
    // It is approptiate for now
    output_names = net.getUnconnectedOutLayersNames();
}

std::unordered_map<std::string, std::shared_ptr<ITensor>> OpenCVInferencer::forward(std::shared_ptr<ITensor> tensor)
{
    auto tensor_cv = tensor_to_cv_mat_blob(tensor);

    net.setInput(tensor_cv);
    std::vector<cv::Mat> result_tensor_cv;
    net.forward(result_tensor_cv, output_names);

    std::unordered_map<std::string, std::shared_ptr<ITensor>> result_map;
    for (size_t i = 0; i < output_names.size(); ++i) {
        auto result_tensor = std::make_shared<OpenCVTensor>(result_tensor_cv[i]);
        result_map[output_names[i]] = result_tensor;
    }

    return result_map;
}

} // namespace backend
