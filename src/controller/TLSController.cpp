#include "TLSController.h"
#include "../parser/InputParser.hpp"

using namespace AmqpClient;
using namespace std;

TLSController::TLSController() : reconnected{false}
{
    auto config_path = std::getenv(CONFIG_PATH_VARIABLE);

    if (!config_path)
    {
        std::cerr << "missing environment variable: " << CONFIG_PATH_VARIABLE << std::endl;
        exit(1);
    }

    std::cout << "path to config file: " << config_path << std::endl;

    std::ifstream file(config_path);
    std::stringstream buffer;
    buffer << file.rdbuf();

    auto j = json::parse(buffer.str());

    address = j["broker-address"].get<std::string>();
    vhost = j["vhost-name"].get<std::string>();
    username = j["username"].get<std::string>();
    password = j["password"].get<std::string>();
    model_path = j["model-path"].get<std::string>();
    input_size = j["input-size"].get<int>();

    requests_queue = j["requests-queue"].get<std::string>();

    model = new Model(model_path, input_size);

    connect();
}

void TLSController::connect()
{
    std::cout << "connecting to: " << address << std::endl;
    int retries = 0;
    while (true)
    {
        if (retries > 5)
        {
            retries = 5;
        }
        int backoff = 10 * static_cast<int>(std::ceil(std::exp(retries)));
        std::chrono::milliseconds timespan(backoff);
        try
        {
            connection = AmqpClient::Channel::Create(address, 5672, username, password, vhost);
            create_queues();
            consumer_tag = connection->BasicConsume(requests_queue, "", true, false, false, 0);
            break;
        }
        catch (...)
        {
            retries += 1;
            std::cout << "unable to connect. retrying ..." << std::endl;
            std::this_thread::sleep_for(timespan);
        }
    }
    std::cout << "connected" << std::endl;
}

void TLSController::create_queues() {
    AmqpClient::Table qTable;
    qTable.insert(TableEntry(TableKey("x-queue-type"), TableValue("quorum")));
    connection->DeclareQueue(
        requests_queue,
        false, // passive
        true,  // durable
        false, // exclusive
        false, // auto_delete
        qTable);
}

void TLSController::run()
{
    while (true)
    {
        try
        {
            if (reconnected)
            {
                connection->BasicRecover(consumer_tag);
                reconnected = false;
            }

            auto envelope = connection->BasicConsumeMessage(consumer_tag);
            auto content = envelope->Message()->Body();
            auto sender = envelope->Message()->ReplyTo();

            Message *message = nullptr;

            try
            {
                InputParser *parser = new InputParser(content);

                auto prediction = model->forward(parser->input);
                message = new ValidMessage(0, prediction, parser->timestamp, parser->junction_id);

                delete parser;
            }
            catch (DifferentSizeException &e)
            {
                message = new ErrorMessage(1, e.what());
            }
            catch (InputSizeMissmatchException &e)
            {
                message = new ErrorMessage(2, e.what());
            }
            catch (nlohmann::detail::type_error &e)
            {
                message = new ErrorMessage(3, "input message data corrupted");
            }

            if (!message)
            {
                message = new DefaultMessage();
            }

            auto response = AmqpClient::BasicMessage::Create(message->getPayload());

            std::cout << "sending to: " << sender << std::endl;

            connection->BasicPublish("", sender, response);
            connection->BasicAck(envelope->GetDeliveryInfo(), false);

            delete message;
        }
        catch (...)
        {
            cout << "connection lost. reconnecting ..." << endl;
            connect();
            reconnected = true;
        }
    }
}
