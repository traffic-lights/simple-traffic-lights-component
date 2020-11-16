#include "Model.h"

int Model::forward(std::vector<float> input)
{
    auto size = static_cast<signed long>(input.size());

    if(size != input_size){
        throw InputSizeMissmatchException();
    }

    auto options = torch::TensorOptions().dtype(torch::kFloat32);
    auto tensor_input = torch::from_blob(input.data(), {1, size}, options);

    std::vector<torch::jit::IValue> inputs{tensor_input};
    auto ret = model.forward(inputs).toTensor().argmax().item().toInt();

    return ret;
}