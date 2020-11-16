#include <iostream>
#include <memory>

#include "controller/TLSController.h"


int main(int argc, const char *argv[])
{
    if (argc != 2)
    {
        std::cout << argv[0] << " json_config_path" << std::endl;
        return -1;
    }

    TLSController *controller = new TLSController(argv[1]);
    controller->run();

    return 0;
}
