#include "Message.h"

class ValidMessage : public Message
{
public:
    ValidMessage(int code, int action, std::string timestamp)
    {
        json j;
        j["code"] = code;
        j["action"] = action;
        j["timestamp"] = timestamp;
        payload = j.dump();
    }

    ~ValidMessage() {}

    std::string getPayload() { return payload; }
};