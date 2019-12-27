//
// Created by zhihui on 12/26/19.
//

#ifndef TRAFFICINFODRIVER_COMMON_H
#define TRAFFICINFODRIVER_COMMON_H

#include <random>

#include "handlers/add_pointcloud_handler.h"
#include "handlers/add_odometry_handler.h"

namespace driver {
    using namespace zhihui::sensor;

    struct Vec3 {
        double x, y, z;

        Vec3() : x(.0), y(.0), z(.0) {}

        Vec3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}

        double length() {
            return std::sqrt(x * x + y * y + z * z);
        }

        void normalize() {
            double len = length();
            if (len <= 0.00001) return;

            x /= len;
            y /= len;
            z /= len;
        }

        Vec3 operator+(const Vec3 &other) {
            return Vec3(x + other.x, y + other.y, z + other.z);
        }
    };

    inline proto::SensorMetadata getMetadata(size_t i) {
        boost::posix_time::ptime current_date_microseconds = boost::posix_time::microsec_clock::local_time();
        long milliseconds = current_date_microseconds.time_of_day().total_milliseconds();

        proto::SensorMetadata metadata;
        metadata.set_trajectory_id(i);
        metadata.set_timestamp(milliseconds);

        return metadata;
    }

    class imu_odometry_generator {
    public:
        using RequestType = typename proto::AddOdometryDataRequest;

        explicit imu_odometry_generator(size_t id)
                : id_(id), e(id_), normal_dist(id_, 3) {
            auto get_vec3 = [this]() {
                return Vec3(normal_dist(e), normal_dist(e), normal_dist(e));
            };

            pos_ = get_vec3();
            velocity_ = get_vec3();
            velocity_.normalize();
        }

        void update() {
            pos_ = pos_ + velocity_;
        }

        RequestType getRequest() {
            RequestType request;

            *request.mutable_sensor_metadata() = getMetadata(id_);
            request.mutable_odometry_data()->mutable_pose()->mutable_translation()->set_x(pos_.x);
            request.mutable_odometry_data()->mutable_pose()->mutable_translation()->set_y(pos_.y);
            request.mutable_odometry_data()->mutable_pose()->mutable_translation()->set_z(pos_.z);

            return request;
        }

        size_t id_;
        Vec3 pos_;
        Vec3 velocity_;

    private:
        std::mt19937 e;
        std::normal_distribution<> normal_dist;
    };

    template < typename Generator>
    class common {
    public:
        using Request = typename Generator::RequestType;

        explicit common(size_t num) : devices_num_(num) {
            for (auto i = 0; i < devices_num_; i++) {
                devices_.emplace_back(i);
            }
        }

        void update() { for (auto &device : devices_) { device.update(); }}

        Request getData(size_t i ) {
            Request request = devices_[i % devices_num_].getRequest();
            return request;
        }

    private:
        const size_t devices_num_;
        std::vector<Generator> devices_;
    };

}
#endif //TRAFFICINFODRIVER_COMMON_H
