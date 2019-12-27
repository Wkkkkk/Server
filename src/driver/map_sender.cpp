//
// Created by zhihui on 12/26/19.
//

#include "map_sender.h"
#include "collector.h"

using namespace driver;

MapSender::MapSender(std::string server_address)
        : server_address_(std::move(server_address))
        , signals_(io_)
        , imu_odometry_handler_(new CarInfoCollector(io_, 20, server_address_))
        , pointcloud__handler(new PointCloudInfoCollector(io_, 100, server_address_))
{
    // Register to handle the signals that indicate when the server should exit.
    // It is safe to register for the same signal multiple times in a program,
    // provided all registration for the specified signal is made through Asio.
    signals_.add(SIGINT);
    signals_.add(SIGTERM);
#if defined(SIGQUIT)
    signals_.add(SIGQUIT);
#endif // defined(SIGQUIT)
    signals_.async_wait(std::bind(&MapSender::handle_stop, this));
}

void MapSender::run() { io_.run(); }

void MapSender::handle_stop() {
    // The server is stopped by cancelling all outstanding asynchronous
    // operations. Once all operations have finished the io_context::run()
    // call will exit.
    io_.stop();

    LOG(INFO) <<  "MapSender is gracefully closed.";
}
