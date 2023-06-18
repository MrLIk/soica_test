#pragma once

#include "IImageProcessor.h"
#include "IInferencer.h"
#include "ITensor2Image.h"
#include "IImage2Tensor.h"

#include <string_view>

namespace soica_test::backend {

class HINProcessor : public IImageProcessor
{
public:
    HINProcessor(std::shared_ptr<IInferencer> inferencer,
                 std::shared_ptr<IImage2Tensor> image_to_tensor,
                 std::shared_ptr<ITensor2Image> tensor_to_image,
                 const std::string& output_name);
    ~HINProcessor() override = default;

    std::shared_ptr<IImage> process(std::shared_ptr<IImage> image) override;

private:
    std::shared_ptr<IInferencer> inferencer;
    std::shared_ptr<IImage2Tensor> image_to_tensor;
    std::shared_ptr<ITensor2Image> tensor_to_image;
    const std::string output_name;
};

} // namespace backend
