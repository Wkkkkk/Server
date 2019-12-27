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
    curl
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
    libc++-dev
    && rm -rf /var/lib/apt/lists/*

#gcc8
RUN add-apt-repository -y ppa:jonathonf/gcc && \
    apt-get update && \
    apt-get install -y gcc-8 g++-8 && \
    rm -rf /usr/bin/gcc /usr/bin/g++ && \
    ln -s /usr/bin/g++-8 /usr/bin/g++ && \
    ln -s /usr/bin/gcc-8 /usr/bin/gcc
    && rm -rf /var/lib/apt/lists/*

RUN apt-add-repository 'deb https://apt.kitware.com/ubuntu/ xenial main' && \
    apt-get update && \
    apt-get install -y --allow-unauthenticated cmake
    && rm -rf /var/lib/apt/lists/*
   
#3rd party
WORKDIR /home/zhihui/library

#grpc from local repository
RUN git clone https://gitee.com/Will_1989/grpc.git && \
    cd grpc && mkdir build && cd build \
    &&cmake .. && make -j6 && make install

#async_grpc
RUN git clone https://gitee.com/Will_1989/async_grpc.git && \
    cd async_grpc && mkdir build && cd build \
    && cmake .. && make -j6 && make install

ln -s /usr/local/lib/libz.a /usr/local/lib/libzlibstatic.a

#self
WORKDIR /home/zhihui/workspace
RUN git clone https://github.com/Wkkkkk/SpaceCloud.git && \
    cd SpaceCloud && mkdir build && cd build \
    && cmake .. && make -j6

##################################################
#pack here
ENV des=/home/zhihui/workspace/SpaceCloud/bin
ENV client_exe=$des/client
ENV server_exe=$des/server
RUN deplist=$(ldd $client_exe | awk  '{if (match($3,"/")){ printf("%s "),$3 } }') && \
    cp $deplist $des
RUN deplist=$(ldd $server_exe | awk  '{if (match($3,"/")){ printf("%s "),$3 } }') && \
    cp $deplist $des

RUN tar -czvf /home/zhihui/workspace/all.tar.gz $des
RUN cd /usr/local/lib64 && tar -czvf /home/zhihui/workspace/osgPlugins.tar.gz ./osgPlugins*/*

###################################################
FROM nvidia/opengl:1.0-glvnd-runtime-ubuntu16.04 as runtime
LABEL description="Run container"

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    qt5-default \
    qttools5-dev-tools \
    libqt5opengl5-dev

ENV bin_path=/home/zhihui/workspace/SpaceCloud/bin/
RUN mkdir -p $bin_path
COPY --from=builder /home/zhihui/workspace/*.tar.gz /home/zhihui/workspace/

RUN tar -xzvf /home/zhihui/workspace/all.tar.gz
RUN tar -xzvf /home/zhihui/workspace/osgPlugins.tar.gz -C $bin_path

#COPY --from=builder /usr/lib/x86_64-linux-gnu/libQt5DBus.so* /home/Demo/bin/
#some fixed environment variables
ENV LD_LIBRARY_PATH=/usr/local/lib64:$bin_path

CMD $bin_path/SpaceCloud

