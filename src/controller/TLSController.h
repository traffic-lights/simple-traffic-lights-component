#include <nlohmann/json.hpp>
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <exception>
#include <cstdlib>

#include "../model/Model.h"
#include "../message/Message.h"
#include "../message/ValidMessage.h"
#include "../message/ErrorMessage.h"
#include "../message/DefaultMessage.h"
#include "../exceptions/Exceptions.h"

using json = nlohmann::json;

constexpr char CONFIG_PATH_VARIABLE[] {"JSON_CONFIG_PATH"};

class TLSController
{
public:
    TLSController();
    void run();

private:
    AmqpClient::Channel::ptr_t connection;

    std::string consumer_tag;
    std::string old_consumer_tag;

    std::string address;
    std::string vhost;
    std::string username;
    std::string password;
    std::string model_path;
    int input_size;
    std::string requests_queue;

    bool reconnected;

    Model *model;

    void connect();
    void create_queues();
};