#include <gflags/gflags.h>
#include <glog/logging.h>

#include "map_sender.h"

DEFINE_string(kServerAddress, "192.168.0.234:50000", "server host:port");
//DEFINE_string(kServerAddress, "0.0.0.0:50000", "server host:port");

int main(int argc, char* argv[])
{
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);
    LOG(INFO) << "connecting to: " << FLAGS_kServerAddress;

    driver::MapSender driver(FLAGS_kServerAddress);
    driver.run();

    return 0;
}
