//
// Created by zhihui on 12/24/19.
//

#ifndef TRAFFICINFODRIVER_ADD_ODOMETRY_HANDLER_H
#define TRAFFICINFODRIVER_ADD_ODOMETRY_HANDLER_H

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
                AddOdometryDataSignature, async_grpc::Stream<proto::AddOdometryDataRequest>,
                google::protobuf::Empty,
                "/zhihui.sensor.proto.Localization/AddOdometryData")

        class AddOdometryHandler : public async_grpc::RpcHandler<AddOdometryDataSignature> {
        public:
            void OnRequest(const proto::AddOdometryDataRequest &request) override;

            void OnReadsDone() override;

        private:
            int sum_ = 0;
        };
    }
}

#endif //TRAFFICINFODRIVER_ADD_ODOMETRY_HANDLER_H
