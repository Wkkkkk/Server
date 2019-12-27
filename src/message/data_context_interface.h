//
// Created by zhihui on 12/24/19.
//

#ifndef TRAFFICINFODRIVER_DATA_CONTEXT_INTERFACE_H
#define TRAFFICINFODRIVER_DATA_CONTEXT_INTERFACE_H

#include <memory>

#include <async_grpc/rpc_handler.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/empty.pb.h>

namespace zhihui::sensor::proto {
    class SensorDataRequest;
}

namespace server {

    const std::string kTestingServerAddress = "localhost:50051";
    const std::size_t kNumThreads = 1;

    class DataServer;

    class DataContextInterface : public async_grpc::ExecutionContext {
    public:
        explicit DataContextInterface(DataServer *map_builder_server);

        void EnqueueSensorData(std::unique_ptr<zhihui::sensor::proto::SensorDataRequest> data);

    private:
        DataServer *map_builder_server_;
    };
}


#endif //TRAFFICINFODRIVER_DATA_CONTEXT_INTERFACE_H
