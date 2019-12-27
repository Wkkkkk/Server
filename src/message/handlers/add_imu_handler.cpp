//
// Created by zhihui on 12/26/19.
//

#include "add_imu_handler.h"

#include "data_context_interface.h"

using namespace message::handler;

void AddImuHandler::OnRequest(const proto::AddImuDataRequest &request) {
    auto sensor_data = std::make_unique<proto::SensorDataRequest>();
    *sensor_data->mutable_sensor_metadata() = request.sensor_metadata();
    *sensor_data->mutable_imu_data() = request.imu_data();

    GetUnsynchronizedContext<server::DataContextInterface>()->EnqueueSensorData(std::move(sensor_data));
}

void AddImuHandler::OnReadsDone() {
    this->Send(std::make_unique<google::protobuf::Empty>());
}
