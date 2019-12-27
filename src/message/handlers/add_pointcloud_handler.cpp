//
// Created by zhihui on 12/24/19.
//

#include "add_pointcloud_handler.h"

#include "data_context_interface.h"

using namespace message::handler;

void AddPointCloudHandler::OnRequest(const proto::AddPointCloudDataRequest &request) {
    auto sensor_data = std::make_unique<proto::SensorDataRequest>();
    *sensor_data->mutable_sensor_metadata() = request.sensor_metadata();
    *sensor_data->mutable_point_cloud_data() = request.point_cloud_data();

//    std::cout << "pointcloud: " << request.point_cloud_data().point_data_size() << std::endl;
    GetUnsynchronizedContext<server::DataContextInterface>()->EnqueueSensorData(std::move(sensor_data));
}

void AddPointCloudHandler::OnReadsDone() {
    this->Send(std::make_unique<google::protobuf::Empty>());
}
