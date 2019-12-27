project(driver)

file(GLOB SOURCES_CXX *.cpp *.ui *.h)

add_executable(${PROJECT_NAME} ${SOURCES_CXX})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES}  gflags glog::glog core message)