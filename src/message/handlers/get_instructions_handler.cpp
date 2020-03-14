//
// Created by zhihui on 3/13/20.
//

#include "get_instructions_handler.h"

using namespace message::handler;

void GetInstructionsHandler::OnRequest(const zhihui::test::proto::LoginRequest &request) {

    if (request.metadata().client_id().empty()) {
        Finish(::grpc::Status(::grpc::INTERNAL, "internal error"));
    }

    for (int i = 0; i < 3; ++i) {
        //TODO set position
        auto response = std::make_unique<proto::InstructionResponse>();
        response->set_timestamp(i);

        //TODO use Writer
        Send(std::move(response));
    }

    std::cout << "GetInstructionsHandler: " << request.metadata().client_id() << std::endl;
}
