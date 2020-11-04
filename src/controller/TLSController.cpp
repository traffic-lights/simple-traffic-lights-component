#include "TLSController.h"

TLSController::TLSController(std::string model_name, std::string address)
{
    model = new Model(model_name);

    auto c = mqtt::make_sync_client(this->ioc, address, 1883);

    c->set_client_id("cl1");
    c->set_clean_session(true);

    c->set_connack_handler(
        [&c](bool sp, mqtt::connect_return_code connack_return_code) {
            std::cout << "Connack handler called" << std::endl;
            std::cout << "Session Present: " << std::boolalpha << sp << std::endl;
            std::cout << "Connack Return Code: "
                      << mqtt::connect_return_code_to_str(connack_return_code) << std::endl;
            if (connack_return_code == mqtt::connect_return_code::accepted)
            {
                c->subscribe("topic1", mqtt::qos::at_most_once);
            }
            return true;
        });
    c->set_close_handler(
        [] {
        });
    c->set_error_handler(
        [](boost::system::error_code const &ec) {
        });
    c->set_puback_handler(
        [](std::uint16_t packet_id) {
            return true;
        });
    c->set_pubrec_handler(
        [](std::uint16_t packet_id) {
            return true;
        });
    c->set_pubcomp_handler(
        [](std::uint16_t packet_id) {
            return true;
        });
    c->set_suback_handler(
        [&c](std::uint16_t packet_id, std::vector<mqtt::suback_return_code> results) {
            std::cout << "suback received. packet_id: " << packet_id << std::endl;
            for (auto const &e : results)
            {
                if (e != mqtt::suback_return_code::failure)
                {
                    std::cout << "subscribe success: " << mqtt::suback_return_code_to_str(e) << std::endl;
                }
                else
                {
                    std::cout << "subscribe failed" << std::endl;
                }
            }
            return true;
        });
    c->set_publish_handler(
        [&c, this](mqtt::optional<std::uint16_t> packet_id,
             mqtt::publish_options pubots,
             mqtt::buffer topic_name,
             mqtt::buffer contents) {
            if (packet_id)
                std::cout << "packet_id: " << *packet_id << std::endl;
            std::cout << "topic_name: " << topic_name << std::endl;
            std::cout << "contents: " << contents << std::endl;

            auto j_contents = json::parse(contents.to_string());
            auto input = j_contents.get<std::vector<float>>();
            std::cout << "input: " << input << " predition: " << model->forward(input) << std::endl;

            c->disconnect();
            return true;
        });

    c->connect();
}

void TLSController::run()
{
    this->ioc.run();
}