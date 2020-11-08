#include "TLSController.h"

TLSController::TLSController(std::string model_name, std::string address) : queue_key{model_name}
{
    model = new Model(model_name);

    connection = AmqpClient::Channel::Create(address);
    std::cout << "conencting to: " << address << std::endl;
}

void TLSController::run()
{
    connection->DeclareQueue(queue_key, false, false, false, false);

    connection->DeclareExchange("responses", AmqpClient::Channel::EXCHANGE_TYPE_DIRECT);

    while (true)
    {
        auto consumer_tag = connection->BasicConsume(queue_key, "", true, true, false, 1);
        auto envelope = connection->BasicConsumeMessage(consumer_tag);
        auto content = envelope->Message()->Body();

        Message *message = new Message(content);

        auto prediction = model->forward(message->input);

        std::cout << "input: " << envelope->Message()->Body() << " prediction: " << prediction << std::endl;

        json j;
        j["action"] = prediction;

        auto response = AmqpClient::BasicMessage::Create(j.dump());

        connection->BasicPublish("responses", message->sender, response);
        delete message;
        connection->BasicAck(envelope);
    }
}
