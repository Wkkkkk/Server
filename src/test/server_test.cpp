//
// Created by zhihui on 12/25/19.
//

#include <memory>

#include <async_grpc/client.h>
#include <async_grpc/execution_context.h>
#include <async_grpc/retry.h>
#include <grpc++/grpc++.h>
#include <gtest/gtest.h>

#include "data_server.h"
#include "data_context_interface.h"
#include "handlers/add_sensordata_handler.h"
#include "handlers/add_odometry_handler.h"
#include "handlers/add_pointcloud_handler.h"

namespace server {
        using namespace message::handler;
        using namespace async_grpc;

class ServerTest : public ::testing::Test {
protected:
    void SetUp() override {
        server_ = std::make_unique<DataServer>();
        client_channel_ = ::grpc::CreateChannel(
                kTestingServerAddress, ::grpc::InsecureChannelCredentials());

        server_->Start();
    }

    void TearDown() override {
        server_->Shutdown();
    }

    std::unique_ptr<DataServer> server_;
    std::shared_ptr<::grpc::Channel> client_channel_;
};

TEST_F(ServerTest, StartAndStopServerTest) {}

TEST_F(ServerTest, ProcessUnaryRpcTest) {
    Client<AddSensorDataSignature> client(client_channel_);
    proto::SensorDataRequest request;
    request.mutable_sensor_metadata()->set_trajectory_id(11);
    EXPECT_TRUE(client.Write(request));
    EXPECT_EQ(client.response().output(), 12);
}

TEST_F(ServerTest, ProcessRpcStreamTest) {
    Client<AddOdometryDataSignature> client(client_channel_);
    for (int i = 0; i < 3; ++i) {
        proto::AddOdometryDataRequest request;
        request.mutable_sensor_metadata()->set_trajectory_id(i);
        EXPECT_TRUE(client.Write(request));
    }
    EXPECT_TRUE(client.StreamWritesDone());
    EXPECT_TRUE(client.StreamFinish().ok());
    EXPECT_EQ(client.response().GetDescriptor()->name(), "Empty");
}

TEST_F(ServerTest, RetryWithUnrecoverableError) {
    Client<AddSensorDataSignature> client(
            client_channel_, common::FromSeconds(5),
            CreateUnlimitedConstantDelayStrategy(common::FromSeconds(1),
                                                 {::grpc::INTERNAL}));
    proto::SensorDataRequest request;
    request.mutable_sensor_metadata()->set_trajectory_id(-11);
    EXPECT_FALSE(client.Write(request));
}

}