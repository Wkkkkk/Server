//
// Created by zhihui on 12/25/19.
//


#include <grpc++/grpc++.h>
#include <async_grpc/client.h>
#include <async_grpc/retry.h>

#include "handlers/add_sensordata_handler.h"
#include "handlers/add_odometry_handler.h"
#include "handlers/add_pointcloud_handler.h"

#include "glog/logging.h"
#include "gtest/gtest.h"

namespace driver {
    namespace {

        const char *kWrongAddress = "wrong-domain-does-not-exist:50051";

        using namespace grpc;
        using namespace async_grpc;
        using namespace zhihui::sensor;

        TEST(ClientTest, TimesOut) {
            auto client_channel = CreateChannel(kWrongAddress, InsecureChannelCredentials());
            Client<message::handler::AddSensorDataSignature, ::grpc::internal::RpcMethod::NORMAL_RPC>
                    client(client_channel, common::FromSeconds(0.1));
            proto::SensorDataRequest request;
            Status status;
            EXPECT_FALSE(client.Write(request, &status));
        }

        TEST(ClientTest, TimesOutWithRetries) {
            auto client_channel = CreateChannel(kWrongAddress, ::grpc::InsecureChannelCredentials());
            Client<message::handler::AddSensorDataSignature, ::grpc::internal::RpcMethod::NORMAL_RPC>
                    client(client_channel, common::FromSeconds(0.5),
                           CreateLimitedBackoffStrategy(common::FromSeconds(0.1), 1, 3));
            proto::SensorDataRequest request;
            Status status;
            EXPECT_FALSE(client.Write(request, &status));
        }

    }  // namespace
}  // namespace async_grpc
