//
// Created by zhihui on 12/25/19.
//

#ifndef TRAFFICINFODRIVER_ADD_SENSORDATA_HANDLER_H
#define TRAFFICINFODRIVER_ADD_SENSORDATA_HANDLER_H

#include <memory>

#include <async_grpc/rpc_handler.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/empty.pb.h>

#include "src/message/localization.pb.h"
#include "src/message/localization.grpc.pb.h"

namespace message {
    namespace handler {

        using namespace zhihui::sensor;

        DEFINE_HANDLER_SIGNATURE(
                AddSensorDataSignature, proto::SensorDataRequest,
                proto::SensorDataResponse,
                "/zhihui.sensor.proto.Localization/AddSensorData")

        class AddSensordataHandler : public async_grpc::RpcHandler<AddSensorDataSignature> {
        public:
            void OnRequest(const proto::SensorDataRequest &request) override;

        private:
            int sum_ = 0;
        };
    }
}

#endif //TRAFFICINFODRIVER_ADD_SENSORDATA_HANDLER_H
