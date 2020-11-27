#include "TLSController.h"

TLSController::TLSController()
{
    auto config_path = std::getenv(CONFIG_PATH_VARIABLE);

    if(!config_path){
        std::cerr << "missing environment variable: " << CONFIG_PATH_VARIABLE << std::endl;
        exit(1);
    }
    
    std::cout << "path to config file: " << config_path << std::endl;

    std::ifstream file(config_path);
    std::stringstream buffer;
    buffer << file.rdbuf();

    auto j = json::parse(buffer.str());

    auto address = j["broker-address"].get<std::string>();
    auto vhost = j["vhost-name"].get<std::string>();
    auto username = j["username"].get<std::string>();
    auto password = j["password"].get<std::string>();
    auto model_path = j["model-path"].get<std::string>();
    auto input_size = j["input-size"].get<int>();

    requests_queue = j["requests-queue"].get<std::string>();

    model = new Model(model_path, input_size);

    connection = AmqpClient::Channel::Create(address, 5672, username, password, vhost);
    std::cout << "connected to: " << address << std::endl;
}

void TLSController::run()
{
    connection->DeclareQueue(requests_queue, false, false, false, false);

    auto consumer_tag = connection->BasicConsume(requests_queue, "", true, false, false);

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

        std::cout << "got message from: " << sender << std::endl;

        connection->BasicPublish("", sender, response);
        connection->BasicAck(envelope->GetDeliveryInfo(), false);

        delete message;
    }
}
