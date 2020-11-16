#pragma once

#include <string>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Message
{
public:
    Message() : payload{} {};
    virtual ~Message(){};
    virtual std::string getPayload() = 0;

protected:
    std::string payload;
};