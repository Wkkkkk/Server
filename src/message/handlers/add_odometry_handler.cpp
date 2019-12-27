//
// Created by zhihui on 12/24/19.
//

#include "add_odometry_handler.h"

#include "data_context_interface.h"

using namespace message::handler;

void AddOdometryHandler::OnRequest(const proto::AddOdometryDataRequest &request) {
    auto sensor_data = std::make_unique<proto::SensorDataRequest>();
    *sensor_data->mutable_sensor_metadata() = request.sensor_metadata();
    *sensor_data->mutable_odometry_data() = request.odometry_data();

//    std::cout << "pointcloud: " << request.point_cloud_data().point_data_size() << std::endl;
    GetUnsynchronizedContext<server::DataContextInterface>()->EnqueueSensorData(std::move(sensor_data));
}

void AddOdometryHandler::OnReadsDone() {
    this->Send(std::make_unique<google::protobuf::Empty>());
}
