//
// Created by zhihui on 12/26/19.
//

#include <cmath>

#include "collector.h"

using namespace driver;

inline proto::SensorMetadata getMetadata(size_t i) {
    boost::posix_time::ptime current_date_microseconds = boost::posix_time::microsec_clock::local_time();
    long milliseconds = current_date_microseconds.time_of_day().total_milliseconds();

    proto::SensorMetadata metadata;
    metadata.set_trajectory_id(i);
    metadata.set_timestamp(milliseconds);

    return metadata;
}

Car::Car(boost::asio::io_service &io, size_t interval, size_t id)
        : Updater(io, interval)
        , id_(id)
        , generator(id_, 3)
{
    phase_ = generator.getNumber() * 10;
    radius_ = 5 ;
    center_ = Vec3(generator.getNumber(), generator.getNumber(), 0.0);
}

void Car::Update() {
    int theta = phase_++ % 360;
    double r = theta / 180.0 * M_PI;

    {
        double sin = std::sin(r);
        double cos = std::cos(r);
        double x = sin * radius_;
        double y = cos * radius_;

        pos_ = center_ + Vec3(y, x, 0.0);
        velocity_ = Vec3(-sin, cos, 0.0);
    }

    {
        double sin2 = std::sin(r / 2);
        double cos2 = std::cos(r / 2);

//        quaternion_ = Vec4(cos2, 0.0, 0.0, sin2);
        quaternion_ = Vec4(0.0, 0.0, sin2, cos2);
//        LOG(INFO) <<  "r: " << r << " theta: " << theta << " sin2: " << sin2 << " cos2: " << cos2;
    }

}

proto::AddOdometryDataRequest Car::getOdometryData() {
    proto::AddOdometryDataRequest request;

    *request.mutable_sensor_metadata() = getMetadata(id_);
    request.mutable_odometry_data()->mutable_pose()->mutable_translation()->set_x(pos_.x);
    request.mutable_odometry_data()->mutable_pose()->mutable_translation()->set_y(pos_.y);
    request.mutable_odometry_data()->mutable_pose()->mutable_translation()->set_z(pos_.z);

    request.mutable_odometry_data()->mutable_pose()->mutable_rotation()->set_x(quaternion_.x);
    request.mutable_odometry_data()->mutable_pose()->mutable_rotation()->set_y(quaternion_.y);
    request.mutable_odometry_data()->mutable_pose()->mutable_rotation()->set_z(quaternion_.z);
    request.mutable_odometry_data()->mutable_pose()->mutable_rotation()->set_w(quaternion_.w);

    return request;
}

proto::AddImuDataRequest Car::getImuData() {
    proto::AddImuDataRequest request;

    *request.mutable_sensor_metadata() = getMetadata(id_);
    request.mutable_imu_data()->mutable_linear_acceleration()->set_x(velocity_.x);
    request.mutable_imu_data()->mutable_linear_acceleration()->set_x(velocity_.y);
    request.mutable_imu_data()->mutable_linear_acceleration()->set_x(velocity_.z);

    return request;
}

CarInfoCollector::CarInfoCollector(boost::asio::io_service &io, size_t interval, const std::string &server_address)
        : Updater(io, interval)
        , imu_client_(::grpc::CreateChannel(server_address, ::grpc::InsecureChannelCredentials()))
        , odometry_client_(::grpc::CreateChannel(server_address, ::grpc::InsecureChannelCredentials()))
        , server_address_(server_address)
{
    for(size_t i = 0; i < 10; i++) {
        cars_.emplace_back(new Car(io, 10, i));
    }
}

CarInfoCollector::~CarInfoCollector() { imu_client_.StreamWritesDone(); odometry_client_.StreamWritesDone(); }

void CarInfoCollector::Update() {
    for(const auto& car : cars_) {
        auto imu = car->getImuData();
        auto odo = car->getOdometryData();

        imu_client_.Write(imu);
        odometry_client_.Write(odo);
    }

    LOG_EVERY_N(INFO, 50) <<  "Send Car   info to" << server_address_ << " every " << interval_.total_milliseconds() << "ms.";
}

PointCloudInfoCollector::PointCloudInfoCollector(boost::asio::io_service &io, size_t interval,
                                                 const std::string &server_address)
        : Updater(io, interval)
        , client_(::grpc::CreateChannel(server_address, ::grpc::InsecureChannelCredentials()))
        , server_address_(server_address)
{}

PointCloudInfoCollector::~PointCloudInfoCollector() { client_.StreamWritesDone(); }

void PointCloudInfoCollector::Update() {
    double center = index_ * 10 % 100;
    PointCloudCluster cluster(center, 20);
    auto pc = cluster.getPointCloudData();
    *pc.mutable_sensor_metadata() = getMetadata(index_++);

    client_.Write(pc);

    LOG_EVERY_N(INFO, 10) <<  "Send Pointcloud to" << server_address_ << " every " << interval_.total_milliseconds() << "ms.";
}

PointCloudInfoCollector::PointCloudCluster::PointCloudCluster(double center, double range) : generator(center, range) {
    center_ = getNewPoint();
}

proto::AddPointCloudDataRequest PointCloudInfoCollector::PointCloudCluster::getPointCloudData() {
    proto::AddPointCloudDataRequest request;

    for(auto i = 0; i < kPointCloudMaxNum; i++) {
        Vec3 p = getNewPoint();
        Vec3 point = p + center_;
        zhihui::transform::proto::Vector3f v3;
        v3.set_x(point.x);
        v3.set_y(point.y);
        v3.set_z(point.z);

        *request.mutable_point_cloud_data()->add_point_data() = v3;
    }

    return request;
}

Vec3 PointCloudInfoCollector::PointCloudCluster::getNewPoint() {
    return Vec3(generator.getNumber(), generator.getNumber(), generator.getNumber());
}
