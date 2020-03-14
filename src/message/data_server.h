//
// Created by zhihui on 12/24/19.
//

#ifndef TRAFFICINFODRIVER_DATA_SERVER_H
#define TRAFFICINFODRIVER_DATA_SERVER_H

#include <memory>
#include <thread>

#include <async_grpc/server.h>

#include "blocking_queue.h"

namespace server {
    class DataServer {
    public:
        DataServer();
        ~DataServer();

        void Start();

        void Shutdown();

        void WaitForShutdown();

        void initSlamThread();

        std::unique_ptr<std::thread> slam_thread_;
        std::unique_ptr<async_grpc::Server> grpc_server_;

//        core::BlockingQueue<std::unique_ptr<zhihui::sensor::proto::SensorDataRequest>> incoming_data_queue_;
    };
}

#endif //TRAFFICINFODRIVER_DATA_SERVER_H
