#pragma once

#include "/usr/local/include/mosquittopp.h"
#include <cstring>
#include <string>
#include <iostream>
#include <vector>

namespace MQTTW
{
    class MosqMqttWrapper : public mosqpp::mosquittopp
    {
    private:
        void on_message(const struct mosquitto_message *message);
        std::vector<std::string> mqttmessages;
    public:
        MosqMqttWrapper(char const  *id, const char *host, int port);
        std::string GetMessage();
        bool send_message(const  char * message);
    };
}
