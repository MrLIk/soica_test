#pragma once

#include "IInferencer.h"

#include <filesystem>
#include <string>

#include <opencv2/dnn.hpp>

namespace soica_test::backend {

class OpenCVInferencer : public IInferencer
{
public:
    // Our vision of the existing backends
    // Not all of those will work with the default opencv library from ubuntu repo
    // CPU will definetly works
    enum class BackendType
    {
        CPU,
        CUDA,
        CUDA_FP16,
        OPENVINO_CPU,
        OPENVINO_CL,
        OPENVINO_CL16
    };

    OpenCVInferencer(const std::filesystem::path& net_path,
                     BackendType backend_type);
    ~OpenCVInferencer() override = default;

    std::unordered_map<std::string, std::shared_ptr<ITensor>> forward(std::shared_ptr<ITensor> tensor) override;

private:
    cv::dnn::Net net;
    std::vector<std::string> output_names;

    // Conversation map of our backend type to opencv
    const std::unordered_map<BackendType, std::tuple<cv::dnn::Backend, cv::dnn::Target>> backend_map = {
        { BackendType::CPU, {cv::dnn::Backend::DNN_BACKEND_OPENCV, cv::dnn::Target::DNN_TARGET_CPU}},
        { BackendType::CUDA, {cv::dnn::Backend::DNN_BACKEND_CUDA, cv::dnn::Target::DNN_TARGET_CUDA}},
        { BackendType::CUDA_FP16, {cv::dnn::Backend::DNN_BACKEND_CUDA, cv::dnn::Target::DNN_TARGET_CUDA_FP16}},
        { BackendType::OPENVINO_CPU, {cv::dnn::Backend::DNN_BACKEND_INFERENCE_ENGINE, cv::dnn::Target::DNN_TARGET_CPU}},
        { BackendType::OPENVINO_CL, {cv::dnn::Backend::DNN_BACKEND_OPENCV, cv::dnn::Target::DNN_TARGET_OPENCL}},
        { BackendType::OPENVINO_CL16, {cv::dnn::Backend::DNN_BACKEND_OPENCV, cv::dnn::Target::DNN_TARGET_OPENCL_FP16}},
    };
};

} // namespace backend
