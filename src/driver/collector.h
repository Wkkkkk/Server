//
// Created by zhihui on 12/26/19.
//

#ifndef TRAFFICINFODRIVER_COLLECTOR_H
#define TRAFFICINFODRIVER_COLLECTOR_H

#include <random>

#include <grpcpp/create_channel.h>
#include <async_grpc/client.h>

#include "updater.h"
#include "handlers/add_pointcloud_handler.h"
#include "handlers/add_imu_handler.h"
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

    struct Vec4 {
        double x, y, z, w;

        Vec4() : x(.0), y(.0), z(.0), w(.0) {}

        Vec4(double _x, double _y, double _z, double _w) : x(_x), y(_y), z(_z), w(_w) {}
    };

    class NormalNumberGenerator {
    public:
        explicit NormalNumberGenerator(double center = 0.0, double range = 3.0) : e(center), normal_dist(center, range) {}

        double getNumber() { return normal_dist(e); }
    private:
        std::mt19937 e;
        std::normal_distribution<> normal_dist;
    };

    class Car : public Updater {
    public:
        explicit Car(boost::asio::io_service& io, size_t interval, size_t id);

        void Update() final;

        proto::AddOdometryDataRequest getOdometryData();
        proto::AddImuDataRequest getImuData();

    private:
        size_t id_;
        int phase_;
        Vec3 pos_;
        Vec3 center_;
        Vec3 velocity_;
        Vec4 quaternion_;
        double radius_;

        NormalNumberGenerator generator;
    };

    class CarInfoCollector : public Updater {
    public:
        explicit CarInfoCollector(boost::asio::io_service& io, size_t interval, const std::string& server_address);
        ~CarInfoCollector();

        void Update() final;

    private:
        async_grpc::Client<message::handler::AddImuDataSignature> imu_client_;
        async_grpc::Client<message::handler::AddOdometryDataSignature> odometry_client_;

        const std::string& server_address_;
        std::vector<std::unique_ptr<Car>> cars_;
    };

    class PointCloudInfoCollector : public Updater  {
    public:
        class PointCloudCluster {
        public:
            explicit PointCloudCluster(double center, double range);

            proto::AddPointCloudDataRequest getPointCloudData();

        private:
            Vec3 getNewPoint();
            const size_t kPointCloudMaxNum = 10000;

            NormalNumberGenerator generator;
            Vec3 center_;
        };

        explicit PointCloudInfoCollector(boost::asio::io_service& io, size_t interval, const std::string& server_address);
        ~PointCloudInfoCollector();

        void Update() final;

    private:
        const std::string& server_address_;

        size_t index_ = 0;
        async_grpc::Client<message::handler::AddPointCloudDataSignature> client_;
    };
}

#endif //TRAFFICINFODRIVER_COLLECTOR_H
