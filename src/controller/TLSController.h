#include <nlohmann/json.hpp>
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <exception>

#include "../model/Model.h"
#include "../message/Message.h"
#include "../message/ValidMessage.h"
#include "../message/ErrorMessage.h"
#include "../message/DefaultMessage.h"
#include "../exceptions/Exceptions.h"

using json = nlohmann::json;

class TLSController
{
public:
    TLSController(std::string config_path);
    void run();

private:
    struct InputParser
    {
        std::vector<float> input;

        InputParser(std::string content) : input{}
        {
            auto j_content = json::parse(content);
            auto last_action = j_content["last_actions"].get<float>();
            auto in_states = j_content["in_states"].get<std::vector<float>>();
            auto out_states = j_content["out_states"].get<std::vector<float>>();

            input.push_back(last_action);

            if (in_states.size() != out_states.size())
            {
                throw SizeMissmatchException();
            }

            auto size = in_states.size();

            for (unsigned int i = 0; i < size; ++i)
            {
                input.push_back(in_states[i] - out_states[i]);
            }
        }
    };

    AmqpClient::Channel::ptr_t connection;

    std::string queue_name;

    Model *model;
};