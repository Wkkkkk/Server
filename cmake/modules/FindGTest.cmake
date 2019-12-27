# Copyright 2018 The Cartographer Authors
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

find_path(GMOCK_INCLUDE_DIRS gmock/gmock.h
        HINTS
        ENV GMOCK_DIR
        PATH_SUFFIXES include
        PATHS
        /usr
        )

# Find system-wide installed gmock.
find_library(GMOCK_MAIN_LIBRARY
        NAMES gmock_main
        HINTS
        ENV GMOCK_DIR
        PATH_SUFFIXES lib
        PATHS
        /usr
        )

# Find system-wide installed gmock.
find_library(GMOCK_LIBRARY
        NAMES gmock
        HINTS
        ENV GMOCK_DIR
        PATH_SUFFIXES lib
        PATHS
        /usr
        )

# Find system-wide gtest header.
find_path(GTEST_INCLUDE_DIRS gtest/gtest.h
        HINTS
        ENV GTEST_DIR
        PATH_SUFFIXES include
        PATHS
        /usr
        )

# Find system-wide installed gtest.
find_library(GTEST_MAIN_LIBRARY
        NAMES gtest_main
        HINTS
        ENV GTEST_DIR
        PATH_SUFFIXES lib
        PATHS
        /usr
        )

# Find system-wide installed gtest.
find_library(GTEST_LIBRARY
        NAMES gtest
        HINTS
        ENV GTEST_DIR
        PATH_SUFFIXES lib
        PATHS
        /usr
        )

set(GTEST_LIBRARIES "")
list(APPEND GTEST_LIBRARIES ${GMOCK_MAIN_LIBRARY} ${GMOCK_LIBRARY} ${GTEST_MAIN_LIBRARY} ${GTEST_LIBRARY})
list(APPEND GTEST_INCLUDE_DIRS ${GTEST_INCLUDE_DIRS})

# System-wide installed gmock library might require pthreads.
find_package(Threads REQUIRED)
list(APPEND GTEST_LIBRARIES ${CMAKE_THREAD_LIBS_INIT} )

#include(FindPackageHandleStandardArgs)
#find_package_handle_standard_args(GMock DEFAULT_MSG GMOCK_LIBRARIES
#        GMOCK_INCLUDE_DIRS)

