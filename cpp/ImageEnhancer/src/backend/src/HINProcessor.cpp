#include "HINProcessor.h"

namespace soica_test::backend {

HINProcessor::HINProcessor(std::shared_ptr<IInferencer> inferencer,
                           std::shared_ptr<IImage2Tensor> image_to_tensor,
                           std::shared_ptr<ITensor2Image> tensor_to_image,
                           const std::string& output_name)
    : inferencer(inferencer)
    , image_to_tensor(image_to_tensor)
    , tensor_to_image(tensor_to_image)
    , output_name(output_name)
{

}

std::shared_ptr<IImage> HINProcessor::process(std::shared_ptr<IImage> image)
{
    auto tensor = image_to_tensor->convert(image);
    auto result_tensors_map = inferencer->forward(tensor);
    auto second_result_tensor = result_tensors_map[output_name];
    auto result_image = tensor_to_image->convert(second_result_tensor);

    return result_image;
}

} // namespace backend
