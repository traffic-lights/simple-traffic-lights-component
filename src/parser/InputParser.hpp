#include <vector>
#include <nlohmann/json.hpp>

#include "../exceptions/Exceptions.h"

using json = nlohmann::json;

struct InputParser
{
    std::vector<float> input;
    std::string timestamp;
    std::string junction_id;

    InputParser(std::string content) : input{}
    {
        auto j_content = json::parse(content);
        auto last_action = j_content["last_action"].get<float>();
        auto in_states = j_content["in_states"].get<std::vector<float>>();
        auto out_states = j_content["out_states"].get<std::vector<float>>();

        input.push_back(last_action);

        if (in_states.size() != out_states.size())
        {
            throw DifferentSizeException();
        }

        auto size = in_states.size();

        for (unsigned int i = 0; i < size; ++i)
        {
            input.push_back(in_states[i] - out_states[i]);
        }
        timestamp = j_content["timestamp"].get<std::string>();
        junction_id = j_content["junction_id"].get<std::string>();

    }
};