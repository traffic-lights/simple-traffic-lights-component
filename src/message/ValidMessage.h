#include "Message.h"

class ValidMessage : public Message
{
public:
    ValidMessage(int code, int action, std::string timestamp, std::string junction_id)
    {
        json j;
        j["code"] = code;
        j["action"] = action;
        j["timestamp"] = timestamp;
        j["junction_id"] = junction_id;
        payload = j.dump();
    }

    ~ValidMessage() {}

    std::string getPayload() { return payload; }
};