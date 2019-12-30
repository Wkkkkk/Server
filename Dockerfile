FROM ubuntu:16.04 as builder
LABEL description="ubuntu"

#basic dependencies.
#compile dependencies
#software-properties-common for add-apt-repository
#ca-certificates for verification
RUN apt-get update && apt-get install -y \
    software-properties-common \
    ca-certificates \
    apt-transport-https \
    build-essential \
    mesa-utils \
    glmark2 \
    cmake \
    sudo \
    vim \
    git \
    tar \
    unzip \
    wget \
    curl \
    && rm -rf /var/lib/apt/lists/*

#project-related denpendencies
RUN apt-get update && apt-get install -y \
    clang \
    g++ \
    golang \
    git \
    google-mock \
    libboost-all-dev \
    libcairo2-dev \
    libcurl4-openssl-dev \
    libeigen3-dev \
    libgflags-dev \
    libgoogle-glog-dev \
    liblua5.2-dev \
    libsuitesparse-dev \
    ninja-build \
    python-sphinx \
    autoconf \
    libtool \
    pkg-config \
    libgtest-dev \
    clang-5.0 \
    libc++-dev \
    libprotobuf-dev \
    protobuf-compiler \
    libprotoc-dev \
    && rm -rf /var/lib/apt/lists/*

#gcc8
RUN add-apt-repository -y ppa:jonathonf/gcc && \
    apt-get update && \
    apt-get install -y gcc-8 g++-8 && \
    rm -rf /usr/bin/gcc /usr/bin/g++ && \
    ln -s /usr/bin/g++-8 /usr/bin/g++ && \
    ln -s /usr/bin/gcc-8 /usr/bin/gcc \
    && rm -rf /var/lib/apt/lists/*

RUN apt-add-repository 'deb https://apt.kitware.com/ubuntu/ xenial main' && \
    apt-get update && \
    apt-get install -y --allow-unauthenticated cmake \
    && rm -rf /var/lib/apt/lists/*
   
#3rd party
WORKDIR /home/zhihui/library

#grpc from local repository
#RUN git clone --depth 1 https://gitee.com/Will_1989/grpc.git && cd grpc \
#    && git submodule update --init --recursive --remote \
#    && mkdir build && cd build && cmake .. && make -j6 && make install

RUN git clone https://gitee.com/Will_1989/grpc.git && cd grpc \
    && git submodule update --init --recursive --remote \
    && cd /home/zhihui/library/grpc && make -j6 && make install && make clean && ldconfig

#other libraries
RUN cd /home/zhihui/library/grpc/third_party/googletest && mkdir build && cd build && cmake .. && make -j6 && make install
RUN cd /home/zhihui/library/grpc/third_party/gflags && mkdir build && cd build && cmake .. && make -j6 && make install
RUN cd /home/zhihui/library/grpc/third_party/glog && mkdir build && cd build && cmake .. && make -j6 && make install
RUN ln -s /usr/local/lib/libz.a /usr/local/lib/libzlibstatic.a

#3rd party
WORKDIR /home/zhihui/library
#async_grpc
RUN git clone https://gitee.com/Will_1989/async_grpc.git && \
    cd async_grpc && mkdir build && cd build \
    && cmake .. && make -j6 && make install

#self
WORKDIR /home/zhihui/workspace
RUN git clone https://github.com/Wkkkkk/TrafficInfoDriver.git && \
    cd TrafficInfoDriver && mkdir build && cd build \
    && cmake .. && make -j6

##################################################
FROM ubuntu:16.04 as runtime
LABEL description="Run container"

ENV bin_path=/home/zhihui/bin/
RUN mkdir -p $bin_path
COPY --from=builder /home/zhihui/workspace/TrafficInfoDriver/bin/driver $bin_path
COPY --from=builder /usr/local/lib/libz.so.1 /usr/local/lib/libz.so.1

CMD $bin_path/driver