//
// Created by zhihui on 12/24/19.
//

#include "src/message/localization.pb.h"
#include "src/message/localization.grpc.pb.h"

#include "data_server.h"
#include "data_context_interface.h"

using namespace server;

DataContextInterface::DataContextInterface(DataServer *map_builder_server)
        : map_builder_server_(map_builder_server) {}

void DataContextInterface::EnqueueSensorData(std::unique_ptr<zhihui::sensor::proto::SensorDataRequest> data) {
    map_builder_server_->incoming_data_queue_.Push(std::move(data));
}
