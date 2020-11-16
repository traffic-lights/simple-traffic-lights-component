#include "Message.h"

class DefaultMessage : public Message
{
public:
    DefaultMessage()
    {
        json j;
        j["code"] = -1;
        payload = j.dump();
    }

    ~DefaultMessage() {}

    std::string getPayload() { return payload; }
};