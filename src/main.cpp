#include <iostream>
#include <memory>

#include "model/Model.h"

int main(int argc, const char *argv[])
{
    std::vector<float> input = {10.0f, 5.0f, 6.0f, 23.0f, 15.5f, 1.0f, 2.0f, 3.0f, 17.0f, 5.5f, 6.7f, 8.9f, 10.11f};
    Model *model = new Model("test_model.pt");
    std::cout << model->forward(input) <<" ok\n";
}
