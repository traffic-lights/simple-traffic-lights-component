#include "Message.h"

class ValidMessage : public Message
{
public:
    ValidMessage(int code, int action)
    {
        json j;
        j["code"] = code;
        j["action"] = action;
        payload = j.dump();
    }

    ~ValidMessage() {}

    std::string getPayload() { return payload; }
};