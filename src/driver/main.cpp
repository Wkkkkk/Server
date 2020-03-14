#include <gflags/gflags.h>
#include <glog/logging.h>

#include "map_sender.h"

DEFINE_string(kServerAddress, "192.168.0.250:9898", "server host:port");

int main(int argc, char *argv[]) {
    google::ParseCommandLineFlags(&argc, &argv, true);
    fLB::FLAGS_logtostderr = true;
    google::InitGoogleLogging(argv[0]);
    LOG(INFO) << "connecting to: " << FLAGS_kServerAddress;

    driver::MapSender driver(FLAGS_kServerAddress);
    driver.run();

    return 0;
}
