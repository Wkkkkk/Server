//
// Created by zhihui on 12/25/19.
//

#include "add_sensordata_handler.h"

#include "data_context_interface.h"

using namespace message::handler;

void AddSensordataHandler::OnRequest(const proto::SensorDataRequest &request) {
    if (request.sensor_metadata().trajectory_id() < 0) {
        Finish(::grpc::Status(::grpc::INTERNAL, "internal error"));
    }

    auto response = std::make_unique<proto::SensorDataResponse>();
    response->set_output(request.sensor_metadata().trajectory_id() + 1);
    Send(std::move(response));
}