//
// Created by zhihui on 12/24/19.
//

#include <string>
#include <thread>
#include <chrono>

#include <gtest/gtest.h>

#include "udp_client_server.h"

namespace core {
    namespace {

        const std::string kServerAddress = "0.0.0.0";
        const std::size_t kServerPort = 8888;
        const std::size_t kPacketSize = 1024;

        TEST(UDPTest, testClient) {
            udp_client client(kServerAddress, kServerPort);

            std::string message = "hello";
            auto result = client.send(message.data(), message.size());

            ASSERT_EQ(message.size(), result);
        }

        TEST(UDPTest, testServer) {
            udp_server server(kServerAddress, kServerPort);

            char data[1];
            auto result = server.timed_recv(data, 1, 20);

            ASSERT_EQ(-1, result);
        }

        TEST(UDPTest, testClientServer) {
            udp_server server(kServerAddress, kServerPort);
            udp_client client(kServerAddress, kServerPort);

            char data[kPacketSize];
            std::string message = "hello";

            std::thread client_thread([&client, &message]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
                auto result = client.send(message.data(), message.size());
                ASSERT_EQ(result, message.size());
            });

            auto length = server.recv(data, kPacketSize);
            std::string received_string(data, length);
            ASSERT_EQ(length, message.size());
            ASSERT_EQ(message, received_string);
            client_thread.join();
        }
    }
}