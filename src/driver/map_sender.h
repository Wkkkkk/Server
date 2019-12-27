//
// Created by zhihui on 12/26/19.
//

#ifndef TRAFFICINFODRIVER_MAP_SENDER_H
#define TRAFFICINFODRIVER_MAP_SENDER_H

#include <memory>

#include <boost/asio.hpp>

#include "updater.h"

namespace driver {
    class MapSender {
    public:
        explicit MapSender(std::string server_address);

        void run();
    private:
        void handle_stop();

        const std::string server_address_;

        boost::asio::io_service io_;
        boost::asio::signal_set signals_;

        std::unique_ptr<Updater> imu_odometry_handler_;
        std::unique_ptr<Updater> pointcloud__handler;
    };
}


#endif //TRAFFICINFODRIVER_MAP_SENDER_H
