#include "TLSController.h"

TLSController::TLSController(std::string config_path)
{
    std::ifstream file(config_path);
    std::stringstream buffer;
    buffer << file.rdbuf();

    auto j = json::parse(buffer.str());

    auto address = j["broker-address"].get<std::string>();
    auto model_path = j["model-path"].get<std::string>();
    auto input_size = j["input-size"].get<int>();
    queue_name = j["queue-name"].get<std::string>();

    model = new Model(model_path, input_size);

    connection = AmqpClient::Channel::Create(address, 5672, "test", "test", "test");
    std::cout << "connected to: " << address << " queue name: " << queue_name << std::endl;
}

void TLSController::run()
{
    connection->DeclareQueue(queue_name, false, false, false);

    connection->DeclareExchange("responses", AmqpClient::Channel::EXCHANGE_TYPE_DIRECT, false, false, false);

    auto consumer_tag = connection->BasicConsume(queue_name, "", true, false, false);

    while (true)
    {
        auto envelope = connection->BasicConsumeMessage(consumer_tag);
        auto content = envelope->Message()->Body();
        auto sender = envelope->Message()->ReplyTo();

        Message *message = new DefaultMessage();

        try
        {
            InputParser *parser = new InputParser(content);

            auto prediction = model->forward(parser->input);
            message = new ValidMessage(0, prediction);

            delete parser;
        }
        catch (SizeMissmatchException &e)
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

        auto response = AmqpClient::BasicMessage::Create(message->getPayload());

        connection->BasicPublish("responses", sender, response);
        connection->BasicAck(envelope->GetDeliveryInfo(), false);

        delete message;
    }
}
