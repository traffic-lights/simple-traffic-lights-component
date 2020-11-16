#include "Message.h"

class ErrorMessage : public Message
{
public:
    ErrorMessage(int code, std::string cause)
    {
        json j;
        j["code"] = code;
        j["cause"] = cause;
        payload = j.dump();
    }

    ~ErrorMessage() {}

    std::string getPayload() { return payload; }
};