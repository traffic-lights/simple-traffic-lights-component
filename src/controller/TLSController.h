#include <mqtt_client_cpp.hpp>
#include <mqtt/setup_log.hpp>

#include <nlohmann/json.hpp>

#include "../model/Model.h"

using json = nlohmann::json;

class TLSController {
public:
    TLSController(std::string model_name, std::string address);
    void run();

    Model *model;
private:
    boost::asio::io_context ioc;
};