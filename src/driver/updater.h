//
// Created by zhihui on 12/26/19.
//

#ifndef TRAFFICINFODRIVER_UPDATER_H
#define TRAFFICINFODRIVER_UPDATER_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace driver {
    class Updater {
    public:
        explicit Updater(boost::asio::io_service& io, size_t interval);
        virtual ~Updater() = default;

        void Run();

        virtual void Update();

    protected:
        const boost::posix_time::milliseconds interval_;
        boost::asio::deadline_timer timer_;
    };

}

#endif //TRAFFICINFODRIVER_UPDATER_H
