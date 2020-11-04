#include <iostream>
#include <memory>

#include "controller/TLSController.h"


int main(int argc, const char *argv[])
{
    if (argc != 3)
    {
        std::cout << argv[0] << " model host" << std::endl;
        return -1;
    }

    TLSController *controller = new TLSController(argv[1], argv[2]);
    controller->run();

    return 0;
}
