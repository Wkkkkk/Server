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
#include "handlers/login_handler.h"
#include "handlers/update_position_handler.h"
#include "handlers/get_instructions_handler.h"
#include "handlers/bidirectional_test.h"

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
        Client<LoginSignature> client(client_channel_);
        proto::LoginRequest request;
        request.mutable_metadata()->set_client_id("test");
        request.set_log_information("Hello");
        EXPECT_TRUE(client.Write(request));
        EXPECT_EQ(client.response().check_information(), "Hello");
    }

    TEST_F(ServerTest, RetryWithUnrecoverableError) {
        Client<LoginSignature> client(
                client_channel_, common::FromSeconds(5),
                CreateUnlimitedConstantDelayStrategy(common::FromSeconds(1),
                                                     {::grpc::INTERNAL}));
        proto::LoginRequest request;
        request.mutable_metadata()->set_client_id("");
        EXPECT_FALSE(client.Write(request));
    }

    TEST_F(ServerTest, ProcessRpcClientStreamTest) {
        Client<UpdatePositionSignature> client(client_channel_);
        for (int i = 0; i < 3; ++i) {
            proto::UpdatePositionRequest request;
            EXPECT_TRUE(client.Write(request));
        }
        EXPECT_TRUE(client.StreamWritesDone());
        EXPECT_TRUE(client.StreamFinish().ok());
        EXPECT_EQ(client.response().GetDescriptor()->name(), "Empty");
    }

    TEST_F(ServerTest, ProcessRpcServerStreamTest) {
        Client<GetInstructionsSignature> client(client_channel_);
        proto::LoginRequest request;
        request.mutable_metadata()->set_client_id("Hello");
        EXPECT_TRUE(client.Write(request));

        int count = 0;
        proto::InstructionResponse response;
        for (int i = 0; i < 3; ++i) {
            EXPECT_TRUE(client.StreamRead(&response));
            EXPECT_EQ(response.timestamp(), count++);
        }
    }

    TEST_F(ServerTest, ProcessRpcBidirectionalTest) {
        Client<BidirectionalTestSignature> client(client_channel_);

        for (int i = 0; i < 10; ++i) {
            proto::testMessage request;
            request.set_index(i);
            EXPECT_TRUE(client.Write(request));

            proto::testMessage response;
            EXPECT_TRUE(client.StreamRead(&response));
            EXPECT_EQ(response.index(), i + 1);
        }
        EXPECT_TRUE(client.StreamWritesDone());
        EXPECT_TRUE(client.StreamFinish().ok());
    }
}