//
// Created by zhihui on 12/24/19.
//
#include <async_grpc/completion_queue_pool.h>

#include "blocking_queue.h"
#include "data_context_interface.h"

#include "data_server.h"

using namespace server;
using namespace message::handler;

DataServer::DataServer() {
    async_grpc::Server::Builder server_builder;
    server_builder.SetServerAddress(kTestingServerAddress);
    server_builder.SetNumGrpcThreads(kNumThreads);
    server_builder.SetNumEventThreads(kNumThreads);

    server_builder.RegisterHandler<AddSensordataHandler>();
    server_builder.RegisterHandler<AddOdometryHandler>();
    server_builder.RegisterHandler<AddPointCloudHandler>();
    grpc_server_ = server_builder.Build();

    grpc_server_->SetExecutionContext(std::make_unique<DataContextInterface>(this));
}

DataServer::~DataServer() {
    if (slam_thread_) {
        slam_thread_->join();
    }

    async_grpc::CompletionQueuePool::Shutdown();
}


void DataServer::Start() {
    LOG(INFO) << "Server listening on " << kTestingServerAddress;
    grpc_server_->Start();
}

void DataServer::Shutdown() {
    grpc_server_->Shutdown();
}

void DataServer::WaitForShutdown() {
    grpc_server_->WaitForShutdown();
}

void DataServer::initSlamThread() {
    slam_thread_ = std::make_unique<std::thread>(
            [this]() {
                LOG(INFO) << "Starting SLAM thread.";
                auto now = std::chrono::system_clock::now();
                std::time_t t = std::chrono::system_clock::to_time_t(now);

                for (;;) {
                    std::unique_ptr<proto::SensorDataRequest> sensor_data =
                            incoming_data_queue_.PopWithTimeout(std::chrono::milliseconds(100)); // ms
                    if (sensor_data) {
                        proto::SensorMetadata metadata = sensor_data->sensor_metadata();
                        proto::PointCloudData pointcloud_data = sensor_data->point_cloud_data();

                        auto index = metadata.trajectory_id();
                        LOG(INFO) << "index: " << index << " size: " << pointcloud_data.point_data_size();
                    } else {
                        std::cout << "0" << std::endl;
                    }
                }
            });
}