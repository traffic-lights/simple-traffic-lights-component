#pragma once

#include <torch/script.h>

class Model
{
public:
    Model(const std::string path)
    {
        try
        {
            model = torch::jit::load(path);
        }
        catch (const c10::Error &e)
        {
            std::cerr << "error loading the model\n";
            exit(-1);
        }

        model.eval();
        torch::NoGradGuard no_grad_;
    };

    ~Model();

    int forward(std::vector<float> input);

private:
    torch::jit::script::Module model;
};