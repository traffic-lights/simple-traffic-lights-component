#include <nlohmann/json.hpp>
#include <SimpleAmqpClient/SimpleAmqpClient.h>

#include "../model/Model.h"

using json = nlohmann::json;

class TLSController
{
public:
    TLSController(std::string model_name, std::string address);
    void run();

private:
    struct Message
    {
        std::string sender;
        std::string queue_name;
        std::vector<float> input;

        Message(std::string content) : sender{}, input{}
        {
            auto j_content = json::parse(content);
            sender = j_content["sender"].get<std::string>();
            auto last_action = j_content["last_action"].get<float>();
            auto lanes_states = j_content["lanes_states"].get<std::vector<float>>();

            input.push_back(last_action);

            auto size = lanes_states.size();

            for (unsigned int i = 0; i < size; i += 2)
            {
                input.push_back(input[i] - input[i + 1]);
            }
        }
    };

    AmqpClient::Channel::ptr_t connection;

    std::string queue_key;

    Model *model;
};