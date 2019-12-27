//
// Created by zhihui on 12/26/19.
//

#include "updater.h"

using namespace driver;

Updater::Updater(boost::asio::io_service &io, size_t interval)
        : interval_(interval), timer_(io, interval_) {
    Run();
}

void Updater::Run() {
    Update();

    timer_.expires_at(timer_.expires_at() + interval_);
    timer_.async_wait(boost::bind(&Updater::Run, this));
}

void Updater::Update() {}
